#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float specularStrength = 0.5;

void main()
{
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	
	//diffuse
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float rawDiffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = rawDiffuse * lightColor;

	//specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	
	float rawSpecular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * rawSpecular * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}