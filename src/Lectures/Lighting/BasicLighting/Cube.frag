#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform float u_Shininess;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

void main()
{
    // Ambient
    vec3 ambient = u_AmbientStrength * u_LightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * u_LightColor;

    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Shininess);
    vec3 specular = u_SpecularStrength * spec * u_LightColor;  

    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;

    FragColor = vec4(result, 1.0f);
}