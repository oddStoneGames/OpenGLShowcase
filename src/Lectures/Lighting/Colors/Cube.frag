#version 330 core

out vec4 FragColor;

uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

void main()
{
    FragColor = vec4(u_LightColor * u_ObjectColor, 1.0f);
}