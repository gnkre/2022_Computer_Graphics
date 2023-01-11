#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;


uniform vec4 lightColor0;
uniform vec4 lightColor1;
uniform vec4 lightColor2;
uniform vec4 lightColor3;

uniform vec3 lightPos0;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;



vec4 spotDirectLight()
{
	float outerCone = 0.50f;
	float innerCone = 0.45f;


	vec3 normal = normalize(Normal);
	vec3 lightDirection0 = normalize(lightPos0 - crntPos);
	vec3 lightDirection1 = normalize(lightPos1 - crntPos);
	float diffuse0 = max(dot(normal, lightDirection0), 0.0f);
	float diffuse1 = max(dot(normal, lightDirection1), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(vec3(0.0f, 0.0f, 150.0f) - crntPos);

	vec3 half0 = normalize(-lightDirection0 + viewDirection);
	float specAmount0 = pow(max(dot(half0, normal), 0.0f), 800);
	float specular0 = specAmount0 * specularLight;

	vec3 halfway1 = normalize(-lightDirection1 + viewDirection);
	float specAmount1 = pow(max(dot(halfway1, normal), 0.0f), 800);
	float specular1 = specAmount1 * specularLight;

	float angle0 = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection0);
	float angle1 = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection1);
	float inten0 = 20 * clamp((angle0 - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	float inten1 = 20 * clamp((angle1 - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);






	float ambient = 0.20f;


	vec3 lightDirection2 = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse2 = max(dot(normal, lightDirection2), 0.0f);

	vec3 lightDirection3 = normalize(vec3(-1.0f, -1.0f, 0.0f));
	float diffuse3 = max(dot(normal, lightDirection3), 0.0f);

	float specularLightDir = 0.35f;
	vec3 halfway2 = normalize(-lightDirection2 + viewDirection);
	float specAmount2 = pow(max(dot(halfway2, normal), 0.0f), 800);
	float specular2 = specAmount2 * specularLightDir;

	vec3 halfway3 = normalize(-lightDirection3 + viewDirection);
	float specAmount3 = pow(max(dot(halfway3, normal), 0.0f), 800);
	float specular3 = specAmount3 * specularLightDir;

	return vec4(0.7f, 0.6f, 0.5f, 1.0f) * lightColor0 * (inten0 * specular0 + diffuse0 + 0.2f) + 
		   vec4(0.7f, 0.6f, 0.5f, 1.0f) * lightColor1 * (inten1 * specular1 + diffuse1 + 0.2f) +
		   vec4(0.7f, 0.6f, 0.5f, 1.0f) * lightColor2 * (diffuse2 * 0.8 + ambient + specular2) +
		   vec4(0.7f, 0.6f, 0.5f, 1.0f) * lightColor3 * (diffuse3 * 0.8 + ambient + specular3);
}

void main()
{
	FragColor = spotDirectLight();
}