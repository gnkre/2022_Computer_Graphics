#include "DealWithShader.h"

int compileVertexShaderAndReturn0(const char* file_name_here, GLuint& vertexShader)
{
	// input shit from file
	std::ifstream input(file_name_here);
	if (!input.is_open())
	{
		printf("Shader file error!\n");
		exit(0);
	}
	std::string file_content;
	while (input.eof() == false)
	{
		std::string single_line;
		std::getline(input, single_line);
		file_content = file_content + "\n" + single_line;
	}
	// finish loadin shit

	const char* pointer_to_shader_content = file_content.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &pointer_to_shader_content, NULL);

	//finally compile 
	glCompileShader(vertexShader);
	return 0;
}


int compileFragmentShaderAndReturn0(const char* file_name_here, GLuint& fragmentShader)
{
	// input shit from file
	std::ifstream input(file_name_here);
	if (!input.is_open())
	{
		printf("Shader file error!\n");
		exit(0);
	}
	std::string file_content;
	while (input.eof() == false)
	{
		std::string single_line;
		std::getline(input, single_line);
		file_content = file_content + "\n" + single_line;
	}
	// finish loadin shit

	const char* pointer_to_shader_content = file_content.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &pointer_to_shader_content, NULL);

	//finally compile 
	glCompileShader(fragmentShader);
	return 0;
}