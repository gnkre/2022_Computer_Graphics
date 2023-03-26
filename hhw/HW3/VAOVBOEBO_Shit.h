#ifndef VAOVBOEBO_SHIT_H
#define VAOVBOEBO_SHIT_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm_vector;
};
void dealWithAllVAOVBOEBO(std::vector<Vertex>& obj_vertex, std::vector<GLuint>& faces, GLuint &VAO_ID_, GLuint &VBO_ID_, GLuint &EBO_ID_);



#endif
