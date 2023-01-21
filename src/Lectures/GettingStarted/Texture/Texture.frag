#version 330 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec2 u_Tiling;
uniform vec3 u_Color;

void main()
{
    // Mix the Two Textures & Multiply the result by the choice of your color.
    FragColor = vec4(vec3(texture(u_Texture, vec2(vTexCoord.x * u_Tiling.x, vTexCoord.y * u_Tiling.y)).rgb * u_Color), 1.0f);
}