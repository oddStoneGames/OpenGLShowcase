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

struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 u_ViewPos;
uniform Material u_Material;

uniform PointLight u_PointLight;


vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos, vec3 ambient, vec3 diffuse, vec3 specular);

void main()
{
    // Ambient from material.
    vec3 ambient = texture(u_Material.texture_diffuse1, TexCoord).rgb;

    // Diffuse from material.
    vec3 diffuse = texture(u_Material.texture_diffuse1, TexCoord).rgb * u_Material.color;

    // Specular from material.
    vec3 specular = texture(u_Material.texture_specular1, TexCoord).rrr * u_Material.specularStrength;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // Calculate Point Light.
    vec3 result = CalcPointLight(u_PointLight, norm, viewDir, FragPos, ambient, diffuse, specular);

    // Send as Output.
    FragColor = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos, vec3 ambient, vec3 diffuse, vec3 specular)
{
    // Point Light Calculation.
    // Ambient Calculation
    ambient *= light.ambient;

    // Diffuse Calculation
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    diffuse *= diff * light.diffuse;

    // Specular Calculation
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
    specular *= light.specular * spec;

    // Attenuation Calculation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation; 

    return (ambient + diffuse + specular);
}