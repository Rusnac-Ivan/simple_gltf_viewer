#ifdef GL_ES
	precision highp float;
    precision highp int;
    precision highp sampler2D;
#endif

#define INV_THREE 0.333333333333333333
#define TWO_INV_THREE 0.666666666666666666
#define EPSILON 0.00001

#define saturate(x) clamp(x, 0.0, 1.0)

out vec4 FragColor;

uniform vec4      uBaseColorFactor;
uniform sampler2D uBaseColorMap;
uniform int       uBaseColorMapSet;

uniform vec3 uEyePos;

in vec3 WorldPos;
in vec3 ViewPos;
in vec3 Normal;
in vec2 UV0;
in vec2 UV1;
in mat4 View;
in mat4 Model;
in vec3 Up;

struct AABB
{
    vec3 bounds[2];
};
AABB createAABB(const vec3 min, const vec3 max)
{
	AABB aabb;
	aabb.bounds[0] = min;
	aabb.bounds[1] = max;
	return aabb;
}
struct Ray
{
	vec3 dir;
	vec3 orig;
	vec3 inv_direction;
	int sign[3];
};
Ray createRay(vec3 o, vec3 d)
{
	Ray ray;
	ray.orig = o;
	ray.dir = d;
	ray.inv_direction = vec3(1./d.x, 1./d.y, 1./d.z);
	ray.sign[0] = ray.inv_direction.x < 0. ? 1 : 0;
	ray.sign[1] = ray.inv_direction.y < 0. ? 1 : 0;
	ray.sign[2] = ray.inv_direction.z < 0. ? 1 : 0;
	return ray;
}

vec2 intersectAABB0(const AABB aabb, Ray ray) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    
	tmin = (aabb.bounds[ray.sign[0]].x - ray.orig.x) * ray.inv_direction.x;
	tmax = (aabb.bounds[1 - ray.sign[0]].x - ray.orig.x) * ray.inv_direction.x;
	tymin = (aabb.bounds[ray.sign[1]].y - ray.orig.y) * ray.inv_direction.y;
	tymax = (aabb.bounds[1 - ray.sign[1]].y - ray.orig.y) * ray.inv_direction.y;
	
	if ( (tmin > tymax) || (tymin > tmax) )
		return vec2(0.);
		
    if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
		
    tzmin = (aabb.bounds[ray.sign[2]].z - ray.orig.z) * ray.inv_direction.z;
	tzmax = (aabb.bounds[1 - ray.sign[2]].z - ray.orig.z) * ray.inv_direction.z;
	
    if ( (tmin > tzmax) || (tzmin > tmax) )
		return vec2(0.);
		
    if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
		
    return vec2(tmin, tmax);
};


bool relativeEqual(float val, float x)
{
    return val - EPSILON <= x && x <= val + EPSILON;
}

mat3 rotateMat()
{
    vec3 u = Up;
    vec3 f = Normal;
    vec3 r = normalize(cross(f, u));
    u = normalize(cross(r, f));
    return transpose(mat3(r, u, f));
    
	vec3 q1 = dFdx(ViewPos);
	vec3 q2 = dFdy(ViewPos);
	vec2 st1 = dFdx(UV0);
	vec2 st2 = dFdy(UV0);

	vec3 N = normalize(Normal);
	vec3 T = normalize(q1 * st2.t - q2 * st1.t);
	vec3 B = -normalize(cross(N, T));//normalize(q2 * st1.t - q1 * st2.t);
	return mat3(T, B, N);
	
	vec3 n0 = vec3(0., 0., 1.); 
	vec3 n = normalize(Normal); 
	
	vec3 a = cross(n, n0);
	
	float sin = length(a);
	
	mat3 I;
	I[0] = vec3(1., 0., 0.);
	I[1] = vec3(0., 1., 0.);
	I[2] = vec3(0., 0., 1.);
	
	//I = transpose(I);
	
	//if(relativeEqual(sin, 0.))
        //return I;
	
	float cos = dot(n0, n);
	a = normalize(a);

	
	mat3 S;
	S[0] = vec3(0.,       a.z,    -a.y);
	S[1] = vec3(-a.z,      0.,     a.x);
	S[2] = vec3(a.y,     -a.x,     0.);
	
	mat3 S2;
	S2[0] = vec3(-a.y*a.y-a.z*a.z,     a.x*a.y,        a.x*a.z);
	S2[1] = vec3(      a.x*a.y,      -a.x*a.x-a.z*a.z,       a.y*a.z);
	S2[2] = vec3(      a.x*a.z,        a.y*a.z,     -a.x*a.x-a.y*a.y);
	
	mat3 res = I + sin * S + (1. - cos) * S2;
	
	return res;
}


void main()
{
	vec4 baseColor;
	
    if (uBaseColorMapSet > -1) 
    {
        vec2 uv = uBaseColorMapSet == 0 ? UV0 : UV1;
        vec3 v = vec3(ViewPos.x, ViewPos.y, ViewPos.z);
        
        vec3 dir = normalize(v); 
        
        mat3 rot = rotateMat();
        dir = rot * dir;
        
        vec3 orig = vec3(uv.x, uv.y, 0.);
        
        Ray ray = createRay(orig, dir);
        AABB aabb = createAABB(vec3(0., 0., 0.), vec3(1., 1., 1.));
        
        vec2 near_far = intersectAABB0(aabb, ray);
        float t = near_far[0];
        {
            vec3 inter = orig + t * dir;
            vec2 new_uv;
            if(relativeEqual(inter.x, 0.)) // left side
            {
                new_uv.x = INV_THREE * inter.z;
                new_uv.y = INV_THREE + INV_THREE * inter.y;
            }
            else if(relativeEqual(inter.x, 1.)) // right side
            {
                new_uv.x = TWO_INV_THREE + INV_THREE * (1. - inter.z);
                new_uv.y = INV_THREE + INV_THREE * inter.y;
            }
            else if(relativeEqual(inter.y, 0.)) // bottom side
            {
                new_uv.x = INV_THREE + INV_THREE * inter.x;
                new_uv.y = INV_THREE * inter.z;
            }
            else if(relativeEqual(inter.y, 1.)) // top side
            {
                new_uv.x = INV_THREE + INV_THREE * inter.x;
                new_uv.y = TWO_INV_THREE + INV_THREE * (1. - inter.z);
            }
            else if(relativeEqual(inter.z, 1.)) // back side
            {
                new_uv.x = INV_THREE + INV_THREE * inter.x;
                new_uv.y = INV_THREE + INV_THREE * inter.y;
            }
            else if(relativeEqual(inter.z, 0.)) // back side
            {
                new_uv.x = INV_THREE + INV_THREE * inter.x;
                new_uv.y = INV_THREE + INV_THREE * inter.y;
            }
            
            baseColor = texture(uBaseColorMap, new_uv);
            
            if(inter.z > 0.75)
            {
                float furn2_t = (0.75 - orig.z) / dir.z;
                //if(furn2_t <= t)
                {
                    vec2 furn2_uv = orig.xy + dir.xy * furn2_t;
                    furn2_uv.x = TWO_INV_THREE + INV_THREE * furn2_uv.x;
                    furn2_uv.y = INV_THREE * furn2_uv.y;
                    
                    vec4 furn2_col = texture(uBaseColorMap, furn2_uv);
                    if(furn2_col.a > 0.9)
                        baseColor = furn2_col;
                }
            }
            if(inter.z > 0.50)
            {
                float furn1_t = (0.50 - orig.z) / dir.z;
                //if(furn1_t <= t)
                {
                    vec2 furn1_uv = orig.xy + dir.xy * furn1_t;
                    furn1_uv.x = INV_THREE * furn1_uv.x;
                    furn1_uv.y = INV_THREE * furn1_uv.y;
                    
                    vec4 furn1_col = texture(uBaseColorMap, furn1_uv);
                    if(furn1_col.a > 0.9)
                        baseColor = furn1_col;
                }
            }
            
            if(inter.z > 0.25)
            {
                float people_t = (0.25 - orig.z) / dir.z;
                //if(people_t <= t)
                {
                    vec2 people_uv = orig.xy + dir.xy * people_t;
                    people_uv.x = TWO_INV_THREE + INV_THREE * people_uv.x;
                    people_uv.y = TWO_INV_THREE + INV_THREE * people_uv.y;
                    
                    vec4 people_col = texture(uBaseColorMap, people_uv);
                    if(people_col.a > 0.9)
                        baseColor = people_col;
                }
            }
            
            if(inter.z > 0.)
            {
                float curtain_t = (0. - orig.z) / dir.z;
                //if(curtain_t <= t)
                {
                    vec2 curtain_uv = orig.xy + dir.xy * curtain_t;
                    curtain_uv.x = INV_THREE * curtain_uv.x;
                    curtain_uv.y = TWO_INV_THREE + INV_THREE * curtain_uv.y;
                    
                    vec4 curtain_col = texture(uBaseColorMap, curtain_uv);
                    if(curtain_col.a > 0.9)
                        baseColor = curtain_col;
                }
            }
            
            //baseColor *= 0.0000001;
    
            //baseColor += vec4(vec3(inter.z, inter.z, inter.z), 1.);
            //baseColor += texture(uBaseColorMap, uv);
        }
        
    }
    else 
        baseColor = uBaseColorFactor;


    FragColor = baseColor;
}
