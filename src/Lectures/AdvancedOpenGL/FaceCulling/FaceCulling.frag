#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_ViewPos;
uniform Material u_Material;

uniform DirectionalLight u_DirectionalLight;

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec3 ambient, vec3 diffuse, vec3 specular);

void main()
{
    // Ambient from material.
    vec3 ambient = texture(u_Material.diffuse, TexCoord).rgb;

    // Diffuse from material.
    vec3 diffuse = texture(u_Material.diffuse, TexCoord).rgb;

    // Specular from material.
    vec3 specular = texture(u_Material.specular, TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // Add Directional Light
    vec3 result = CalcDirLight(u_DirectionalLight, norm, viewDir, ambient, diffuse, specular);

    // Send as Output.
    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec3 ambient, vec3 diffuse, vec3 specular)
{
    // Directional Light Calculation.

    // Ambient Calculation
    ambient *= light.ambient;

    // Diffuse Calculation
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    diffuse *= diff * light.diffuse;

    // Specular Calculation
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
    specular *= light.specular * spec;

    return (ambient + diffuse + specular);
}