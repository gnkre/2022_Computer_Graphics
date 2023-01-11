#version 410 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength; 

// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor, lightColor1;
// Gets the position of the light from the main function
uniform vec3 lightPos, lightPos1;
// Gets the position of the camera from the main function
uniform vec3 camPos;


vec4 direcLight(vec3 lgtPos, vec4 lgtColr)
{
	// ambient lighting
	//float ambient = 0.20f;

	
	
	//ambient
	vec4 ambient = ambientStrength * lgtColr * 0.07;
	vec3 lightVec = lgtPos - crntPos;
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec - crntPos);
	float diff = max(dot(normal, lightDirection), 0.0f);
	//vec4 diffuse = diff * texture(diffuse0, texCoord);
	vec4 diffuse = diffuseStrength * diff * lgtColr;

	// specular lighting
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	vec4 specular = specularStrength * specAmount * lgtColr;
	//float specular = specAmount * specularLight;


	//vec3 lightingColor = ambient + diffuse + specular;

	return (ambient + texture(diffuse0, texCoord) * diffuse + texture(specular0, texCoord).r * specular);
	//return (texture(diffuse0, texCoord) * (0.0f + 0.0f) + texture(specular0, texCoord).r * 0.0f) * lightColor;
}


void main()
{
	// outputs final color
	FragColor = direcLight(lightPos, lightColor) + direcLight(lightPos1, lightColor1);
}