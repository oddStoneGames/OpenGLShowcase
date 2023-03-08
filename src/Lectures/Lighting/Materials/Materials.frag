#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
    // Ambient
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_Light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * u_Material.specular);

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}