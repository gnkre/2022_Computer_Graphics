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
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

float random(vec2 p)
{
   // e^pi (Gelfond's constant)
   // 2^sqrt(2) (Gelfond¡VSchneider constant)
     vec2 K1 = vec2( 23.14069263277926, 2.665144142690225 );

     return fract(cos(dot(p,K1)) * 12345.6789); // ver3
}

vec4 direcLight()
{
	// ambient lighting
	//float ambient = 0.20f;

	
	
	//ambient
	vec4 ambient = ambientStrength * lightColor * 0.07;
	vec3 lightVec = lightPos - crntPos;
	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec - crntPos);
	float diff = max(dot(normal, lightDirection), 0.0f);
	//vec4 diffuse = diff * texture(diffuse0, texCoord);
	vec4 diffuse = diffuseStrength * diff * lightColor;

	// specular lighting
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	vec4 specular = specularStrength * specAmount * lightColor;
	//float specular = specAmount * specularLight;


	//vec3 lightingColor = ambient + diffuse + specular;

	return (texture(diffuse0, texCoord) * diffuse + texture(specular0, texCoord).r * specular);
	//return (texture(diffuse0, texCoord) * (0.0f + 0.0f) + texture(specular0, texCoord).r * 0.0f) * lightColor;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	vec4 ambient = ambientStrength * lightColor * 0.1;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec4 diffuse = diffuseStrength * diff * lightColor;
	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	vec4 specular = specularStrength * specAmount * lightColor;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}


void main()
{
	// outputs final color
	float rnd = random(vec2(crntPos.x, lightPos.y + lightPos.x + lightPos.z));
	vec4 temp = direcLight();
	vec4 temp2;
	temp2.x = (temp.x + temp.y + temp.z) * (rnd);
	temp2.y = (temp.x + temp.y + temp.z) * (rnd);
	temp2.z = (temp.x + temp.y + temp.z) * (rnd);
	FragColor = temp2;
}