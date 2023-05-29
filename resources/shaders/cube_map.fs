#ifdef GL_ES
    precision highp int;
    precision highp float;
#endif
out vec4 FragColor;

in vec3 UV0;
in vec3 WorldPos;

uniform samplerCube uBaseColorMap;

void main()
{
    FragColor = texture(uBaseColorMap, UV0);
}
