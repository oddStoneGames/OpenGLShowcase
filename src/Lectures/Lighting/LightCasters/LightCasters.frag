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

// 0 - Directional Light
// 1 - Point Light
// 2 - Spot Light
uniform int u_LightType;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLight;
uniform SpotLight u_SpotLight;

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

    if(u_LightType == 0)
    {
        // Directional Light Calculation.

        // Ambient Calculation
        ambient *= u_DirectionalLight.ambient;

        // Diffuse Calculation
        vec3 lightDir = normalize(-u_DirectionalLight.direction);
        float diff = max(dot(norm, lightDir), 0.0f);
        diffuse *= diff * u_DirectionalLight.diffuse;

        // Specular Calculation
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
        specular *= u_DirectionalLight.specular * spec;
    }
    else if(u_LightType == 1)
    {
        // Point Light Calculation.

        // Ambient Calculation
        ambient *= u_PointLight.ambient;

        // Diffuse Calculation
        vec3 lightDir = normalize(u_PointLight.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0f);
        diffuse *= diff * u_PointLight.diffuse;

        // Specular Calculation
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
        specular *= u_PointLight.specular * spec;

        // Attenuation Calculation
        float dist = length(u_PointLight.position - FragPos);
        float attenuation = 1.0 / (u_PointLight.constant + u_PointLight.linear * dist + u_PointLight.quadratic * (dist * dist));
        ambient  *= attenuation; 
        diffuse  *= attenuation;
        specular *= attenuation; 
    }
    else
    {
        // Spot Light Calculation.

        // Ambient Calculation
        ambient *= u_SpotLight.ambient;

        // Diffuse Calculation
        vec3 lightDir = normalize(u_SpotLight.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0f);
        diffuse *= diff * u_SpotLight.diffuse;

        // Specular Calculation
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
        specular *= u_SpotLight.specular * spec;

        // Spotlight (soft edges) Calculation
        float theta = dot(lightDir, normalize(-u_SpotLight.direction));
        float epsilon = (u_SpotLight.cutOff - u_SpotLight.outerCutOff);
        float intensity = clamp((theta - u_SpotLight.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;

        // Attenuation Calculation
        float dist = length(u_SpotLight.position - FragPos);
        float attenuation = 1.0 / (u_SpotLight.constant + u_SpotLight.linear * dist + u_SpotLight.quadratic * (dist * dist));
        ambient  *= attenuation; 
        diffuse  *= attenuation;
        specular *= attenuation; 
    }
    
    // Combine Result.
    vec3 result = ambient + diffuse + specular + emission;

    // Send as Output.
    FragColor = vec4(result, 1.0f);
}