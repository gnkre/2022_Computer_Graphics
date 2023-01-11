#version 330 core

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
	vec4 ambient = ambientStrength * lgtColr * 0.07;

	vec4 color_bright;
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lgtPos - crntPos);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec4 diffuse = diffuseStrength * diff * lgtColr;

	float intensity = max(dot(normal, lightDirection), 0.0);


	if (intensity > 0.95) 
	{
		color_bright = vec4(1.0, 1.0, 1.0, 1.0);
	}
	else if (intensity > 0.75)
	{
		color_bright = vec4(0.8, 0.8, 0.8, 1.0);
	}
	else if (intensity > 0.50)
	{
		color_bright = vec4(0.6, 0.6, 0.6, 1.0);
	}
	else if (intensity > 0.25)
	{
		color_bright = vec4(0.4, 0.4, 0.4, 1.0);
	}
	else 
	{
		color_bright = vec4(0.2 ,0.2, 0.2, 1.0);
	}

	// specular lighting
	//float specularLight = 0.50f;
	//vec3 viewDirection = normalize(camPos - crntPos);
	//vec3 reflectionDirection = reflect(-lightDirection, normal);
	//float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	//vec4 specular = specAmount * specularStrength * lightColor;

	//return texture(diffuse0, texCoord) * color_bright;
	//return (ambient + texture(diffuse0, texCoord) * diffuse + texture(specular0, texCoord).r * specular) * color_bright;
	return (ambient + texture(diffuse0, texCoord) * diffuse) * color_bright * (color_bright * 1.5);
}

void main()
{
	FragColor = direcLight(lightPos, lightColor) + direcLight(lightPos1, lightColor1);
}