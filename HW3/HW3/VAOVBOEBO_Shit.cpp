#include "VAOVBOEBO_Shit.h"
#include <vector>
void dealWithAllVAOVBOEBO(std::vector<Vertex>& obj_vertex, std::vector<GLuint>& faces, GLuint& VAO_ID_, GLuint& VBO_ID_, GLuint& EBO_ID_)
{
	GLuint VAO_ID, VBO_ID, EBO_ID;
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, obj_vertex.size() * sizeof(Vertex), obj_vertex.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), faces.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(VAO_ID);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);

	

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	VAO_ID_ = VAO_ID;
	VBO_ID_ = VBO_ID;
	EBO_ID_ = EBO_ID;

	return;
}