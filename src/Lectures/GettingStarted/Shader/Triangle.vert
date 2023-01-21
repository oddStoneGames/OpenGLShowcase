#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform int u_Invert;
uniform vec2 u_Offset;

void main()
{
    gl_Position = vec4(aPos.x + u_Offset.x, u_Invert * (aPos.y + u_Offset.y), aPos.z, 1.0f);
    vColor = aColor;
}