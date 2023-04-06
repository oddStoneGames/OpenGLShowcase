#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_diffuse5;

    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;

    vec3 color;
    float specularStrength;
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

vec4 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec4 ambient, vec4 diffuse, vec4 specular);

void main()
{
    // Ambient from material.
    vec4 ambient = texture(u_Material.texture_diffuse1, TexCoord);

    // Diffuse from material.
    vec4 diffuse = texture(u_Material.texture_diffuse1, TexCoord) * vec4(u_Material.color, 1.0f);

    // Specular from material.
    vec4 specular = texture(u_Material.texture_specular1, TexCoord).rrrr * u_Material.specularStrength;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // Calculate Point Light.
    vec4 result = CalcDirLight(u_DirectionalLight, norm, viewDir, ambient, diffuse, specular);

    // Send as Output.
    FragColor = result;
}

vec4 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec4 ambient, vec4 diffuse, vec4 specular)
{
    // Directional Light Calculation.

    // Ambient Calculation
    ambient *= vec4(light.ambient, 1.0f);

    // Diffuse Calculation
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    diffuse *= diff * vec4(light.diffuse, 1.0f);

    // Specular Calculation
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
    specular *= vec4(light.specular, 1.0f) * spec;

    return (ambient + diffuse + specular);
}