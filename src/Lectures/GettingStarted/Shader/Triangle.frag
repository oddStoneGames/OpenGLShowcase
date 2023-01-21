#version 330 core

in vec3 vColor;
out vec4 FragColor;

uniform vec3 u_Color;

void main()
{
    FragColor = vec4(vColor * u_Color, FragColor);
}