#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 vTexCoord;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0f);
    vTexCoord = aTexCoord;
}