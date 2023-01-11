#ifndef DEALWITHSHADER_H
#define DEALWITHSHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>

int compileVertexShaderAndReturn0(const char* file_name_here, GLuint& vertexShader);
int compileFragmentShaderAndReturn0(const char* file_name_here, GLuint& fragmentShader);


#endif