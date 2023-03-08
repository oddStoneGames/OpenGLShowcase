#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct SpotLight
{
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform vec3 u_ViewPos;
uniform Material u_Material;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[2];
uniform SpotLight u_SpotLight;

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir, vec3 ambient, vec3 diffuse, vec3 specular);
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos, vec3 ambient, vec3 diffuse, vec3 specular);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 fragPos, vec3 ambient, vec3 diffuse, vec3 specular);

void main()
{
    // Ambient from material.
    vec3 ambient = texture(u_Material.diffuse, TexCoord).rgb;

    // Diffuse from material.
    vec3 diffuse = texture(u_Material.diffuse, TexCoord).rgb;

    // Specular from material.
    vec3 specular = texture(u_Material.specular, TexCoord).rgb;

    // Emission
    vec3 emission = texture(u_Material.emission, TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // 1. Add Directional Light
    vec3 result = CalcDirLight(u_DirectionalLight, norm, viewDir, ambient, diffuse, specular);

    // 2. Add Point Light
    for(int i = 0; i < 2; i++)
        result += CalcPointLight(u_PointLights[i], norm, viewDir, FragPos, ambient, diffuse, specular);

    // 3. Add Spot Light
    result += CalcSpotLight(u_SpotLight,  norm, viewDir, FragPos, ambient, diffuse, specular);

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
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 fragPos, vec3 ambient, vec3 diffuse, vec3 specular)
{
    // Spot Light Calculation.

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

    // Spotlight (soft edges) Calculation
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // Attenuation Calculation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}