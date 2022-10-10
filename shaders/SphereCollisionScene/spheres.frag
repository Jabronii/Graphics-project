#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 fragNormal;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float specularStrength = 0.5;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient  = light.ambient  * vec3(objectColor);
    vec3 diffuse  = light.diffuse  * diff * vec3(objectColor);
    vec3 specular = light.specular * spec * vec3(objectColor);
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * objectColor;
    vec3 diffuse  = light.diffuse  * diff * objectColor;
    vec3 specular = light.specular * spec * objectColor;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main()
{
    // properties
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //result += CalcPointLight(pointLights[0], norm, fragPos, viewDir);
    // phase 2: Point lights
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);    
    }
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
	FragColor = vec4(result, 1.0);
}