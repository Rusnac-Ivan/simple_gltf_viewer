#ifdef GL_ES
    precision highp int;
    precision highp float;
#endif
out vec4 FragColor;

in vec3 Normal;

void main()
{
    FragColor = vec4(Normal, 1.0);
}
