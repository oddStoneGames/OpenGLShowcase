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
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, TexCoord).rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_Light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, TexCoord).rgb;

    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, TexCoord).rgb;

    // emission
    vec3 emission = texture(u_Material.emission, TexCoord).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    
    FragColor = vec4(result, 1.0f);
}