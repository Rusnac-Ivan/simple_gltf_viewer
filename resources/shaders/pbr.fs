#ifdef GL_ES
	precision highp float;
    precision highp int;
    precision highp sampler2D;
#endif

#define saturate(x) clamp(x, 0.0, 1.0)

out vec4 FragColor;



uniform vec4      uBaseColorFactor;
uniform sampler2D uBaseColorMap;
uniform int       uBaseColorMapSet;

uniform float     uRoughnessFactor;
uniform float     uMetalnessFactor;
uniform sampler2D uMetallicRoughnessMap;
uniform int       uMetallicRoughnessMapSet;

uniform sampler2D uNormalMap;
uniform int       uNormalMapSet;

uniform vec4      uEmissiveFactor;
uniform sampler2D uEmissiveMap;
uniform int       uEmissiveMapSet;

// only available when refraction is enabled
const float TRANSMISSION = 1.0;                         // default: 1.0
const vec3 ABSORPTION = vec3(0.0, 0.0, 0.0);            // default float3(0.0, 0.0, 0.0)
const float IOR = 1.5;                                  // default: 1.5
const float MICROTHICKNESS = 0.0;                       // default: 0.0, not available with refractionType "solid"

uniform samplerCube uCubeMap;

in vec3 WorldPos;
in vec3 Normal;
in vec2 UV0;
in vec2 UV1;

uniform vec3 uEyePos;
uniform vec3 uLightPos;

uniform int uDebugViewInputs;

float uExposure = 4.5;
float uGamma = 2.2;

const float M_PI = 3.1415926535897932;
const float c_MinRoughness = 0.04;

#define MANUAL_SRGB 1

vec3 Uncharted2Tonemap(vec3 color)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	return ((color*(A*color+C*B)+D*E)/(color*(A*color+B)+D*F))-E/F;
}

vec4 tonemap(vec4 color)
{
	vec3 outcol = Uncharted2Tonemap(color.rgb * uExposure);
	outcol = outcol * (1.0 / Uncharted2Tonemap(vec3(11.2)));	
	return vec4(pow(outcol, vec3(1.0 / uGamma)), color.a);
}

vec4 SRGBtoLINEAR(vec4 srgbIn)
{
	#ifdef MANUAL_SRGB
	#ifdef SRGB_FAST_APPROXIMATION
	vec3 linOut = pow(srgbIn.xyz,vec3(2.2));
	#else //SRGB_FAST_APPROXIMATION
	vec3 bLess = step(vec3(0.04045),srgbIn.xyz);
	vec3 linOut = mix( srgbIn.xyz/vec3(12.92), pow((srgbIn.xyz+vec3(0.055))/vec3(1.055),vec3(2.4)), bLess );
	#endif //SRGB_FAST_APPROXIMATION
	return vec4(linOut,srgbIn.w);;
	#else //MANUAL_SRGB
	return srgbIn;
	#endif //MANUAL_SRGB
}

// Find the normal for this fragment, pulling either from a predefined normal map
// or from the interpolated mesh normal and tangent attributes.
vec3 getNormal()
{
	// Perturb normal, see http://www.thetenthplanet.de/archives/1180
	vec3 tangentNormal = texture(uNormalMap, uNormalMapSet == 0 ? UV0 : UV1).xyz * 2.0 - 1.0;

	vec3 q1 = dFdx(WorldPos);
	vec3 q2 = dFdy(WorldPos);
	vec2 st1 = dFdx(UV0);
	vec2 st2 = dFdy(UV0);

	vec3 N = normalize(Normal);
	vec3 T = normalize(q1 * st2.t - q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

// Basic Lambertian diffuse
vec3 diffuseLambert(vec3 diffuseColor)
{
	return diffuseColor / M_PI;
}

vec3 diffuseOrenNayarBrdf(vec3 reflectance, vec3 normal, vec3 viewDir, vec3 lightDir, float NoV, float NoL, float roughness2) 
{
    float a = 1.0 - 0.5 * roughness2 / (roughness2 + 0.33);
    float b = 0.45 * roughness2 / (roughness2 + 0.09);
    float cosPhi = dot(normalize(viewDir - NoV * normal), normalize(lightDir - NoL * normal)); // cos(phi_v, phi_l)
    float sinNV = sqrt(1.0 - NoV * NoV);
    float sinNL = sqrt(1.0 - NoL * NoL);
    float s = NoV < NoL ? sinNV : sinNL; // sin(max(theta_v, theta_l))
    float t = NoV > NoL ? sinNV / NoV : sinNL / NoL; // tan(min(theta_v, theta_l))
    return (reflectance / M_PI) * (a + b * cosPhi * s * t);
}


// Implementation of fresnel
vec3 F_Schlick(float VdotH, vec3 f0, vec3 f90) 
{
    return f0 + (f90 - f0) * pow(saturate(1.0 - VdotH), 5.0);
}


float G_GeometricOcclusion(float NoV, float NoL, float roughness2) 
{
	float attenuationL = 2.0 * NoL / (NoL + sqrt(roughness2 * roughness2 + (1.0 - roughness2 * roughness2) * (NoL * NoL)));
	float attenuationV = 2.0 * NoV / (NoV + sqrt(roughness2 * roughness2 + (1.0 - roughness2 * roughness2) * (NoV * NoV)));
	return attenuationL * attenuationV;
}

float G_ShadowingSmithJoint(float NoV, float NoL, float roughness2) 
{
    float lv = 0.5 * (-1.0 + sqrt(1.0 + roughness2 * (1.0 / (NoV * NoV) - 1.0)));
    float ll = 0.5 * (-1.0 + sqrt(1.0 + roughness2 * (1.0 / (NoL * NoL) - 1.0)));
    return 1.0 / (1.0 + lv + ll);
}

//microfacet distribution 
float D_GGX(float NoH, float roughness2) 
{
    float f = (NoH * roughness2 - NoH) * NoH + 1.0;
    return roughness2 / (M_PI * f * f);
}

vec3 getReflection(vec3 baseColor, float roughness, float metallic, float NoV, vec3 n, vec3 r)
{
    float lod = 14.0 * roughness;
    vec3 reflectance = textureLod(uCubeMap, r, lod).rgb;
    vec3 f0 = 0.16 * reflectance * (1.0 - metallic) + baseColor * reflectance * metallic;
    //vec3 f90 = baseColor * reflectance * metallic + (1.0 - metallic) * baseColor;
    vec3 f90 = baseColor * reflectance * metallic + (1.0 - metallic) * baseColor;
	return F_Schlick(NoV, f0, f90) * (1.0 - roughness) + (baseColor / 3.0) * roughness;
}


void main()
{
    float perceptualRoughness;
	float metallic;
	vec3 diffuseColor;
	vec4 baseColor;
	vec4 base_Color;
	
    if (uBaseColorMapSet > -1) 
    {
        base_Color = texture(uBaseColorMap, uBaseColorMapSet == 0 ? UV0 : UV1);
        baseColor = base_Color;
        baseColor.a *= uBaseColorFactor.a;
    }   
    else 
    {
        baseColor = uBaseColorFactor;
        base_Color = uBaseColorFactor;
    }
    
    vec3 f0 = vec3(0.04);
	
	metallic = uMetalnessFactor;
	perceptualRoughness = uRoughnessFactor;
	
	if (uMetallicRoughnessMapSet > -1)
	{
        // Roughness is stored in the 'g' channel, metallic is stored in the 'b' channel.
        // This layout intentionally reserves the 'r' channel for (optional) occlusion map data
        vec4 mrSample = texture(uMetallicRoughnessMap, uMetallicRoughnessMapSet == 0 ? UV0 : UV1);
        perceptualRoughness = mrSample.g;
        metallic = mrSample.b;
	}
	
    perceptualRoughness = clamp(perceptualRoughness, c_MinRoughness, 1.0);
    metallic = saturate(metallic);
    
    diffuseColor = baseColor.rgb * (vec3(1.0) - f0);
	diffuseColor *= 1.0 - metallic;
	
	vec3 specularColor = mix(f0, baseColor.rgb, metallic);
	
	// Compute reflectance.
	float reflectance = max(max(specularColor.r, specularColor.g), specularColor.b);
	
	// For typical incident reflectance range (between 4% to 100%) set the grazing reflectance to 100% for typical fresnel effect.
	// For very low reflectance range on highly diffuse objects (below 4%), incrementally reduce grazing reflecance to 0%.
	float reflectance90 = saturate(reflectance * 25.0);
	
	vec3 n = (uNormalMapSet > -1) ? getNormal() : normalize(Normal);
	vec3 v = normalize(uEyePos - WorldPos);    // Vector from surface point to camera
	vec3 l = normalize(uLightPos - WorldPos);     // Vector from surface point to light
	vec3 h = normalize(l + v);                        // Half vector between both l and v
	vec3 r = -normalize(reflect(v, n));
	//r.y *= -1.0;
	
	float NdotL = clamp(dot(n, l), 0.001, 1.0);
	float NdotV = clamp(abs(dot(n, v)), 0.001, 1.0);
	float NdotH = saturate(dot(n, h));
	float LdotH = saturate(dot(l, h));
	float VdotH = saturate(dot(v, h));
	
	
	float roughness2 = perceptualRoughness * perceptualRoughness;
	
	// Calculate the shading terms for the microfacet specular shading model
	vec3 F = F_Schlick(VdotH, vec3(reflectance), vec3(reflectance90));
	float G = G_ShadowingSmithJoint(NdotV, NdotL, roughness2);
	float D = D_GGX(NdotH, roughness2);
	
	const vec3 u_LightColor = vec3(1.0);
	
	// Calculation of analytical lighting contribution
	vec3 diffuseContrib =  (1.0 - F) * diffuseLambert(diffuseColor);
	vec3 specContrib = F * G * D / (4.0 * NdotL * NdotV);
	// Obtain final intensity as reflectance (BRDF) scaled by the energy of the light (cosine law)
	vec3 color = NdotL * u_LightColor * (diffuseContrib + specContrib);
	
	// lightIntensity is the illuminance
    // at perpendicular incidence in lux
    float lightIntensity = 1.f; //lux
    float illuminance = lightIntensity * NdotL;
    color *= illuminance;
    
	
	const float u_EmissiveFactor = 1.0f;
	if (uEmissiveMapSet > -1) 
	{
		vec3 emissive = SRGBtoLINEAR(texture(uEmissiveMap, uEmissiveMapSet == 0 ? UV0 : UV1)).rgb * vec3(uEmissiveFactor);
		color += emissive;
	}
		
	//ambient
	color += 0.2 * baseColor.rgb;
	
	color +=  getReflection(baseColor.rgb, perceptualRoughness, metallic, NdotV, n, r);
	
	FragColor = vec4(color, baseColor.a);
	
	// Shader inputs debug visualization
	if (uDebugViewInputs > 0) 
	{
		switch (uDebugViewInputs) 
		{
			case 1:
				FragColor.rgba = base_Color; //base color of frag
				break;
			case 2:
				FragColor.rgba = vec4(n.rgb, 1.0); //normal of frag
				break;
			case 3:
				FragColor.rgba = vec4(vec3(perceptualRoughness), 1.0); //roughness of frag
				break;
			case 4:
				FragColor.rgba = vec4(vec3(metallic), 1.0); //metallic of frag
				break;
		}
	}
}





