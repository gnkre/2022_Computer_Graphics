#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "DealWithShader.h"
#include "VAOVBOEBO_Shit.h"

#define WIDTH 900
#define HEIGHT 900
#define INTERVAL 15
#define NULL_VEC3 { 0, 0, 0 }

using namespace glm;


std::vector<Vertex> obj_vertex;
std::vector<GLuint> faces;
std::vector<Vertex> obj_vertex_final;

GLuint ID_of_shader;
GLuint VAO_ID;
GLuint VBO_ID;
GLuint EBO_ID;
GLuint vertexShader;
GLuint fragmentShader;

GLfloat light_pos0[] = { -10.0f, 10.0f, 0.00f, 1.0f };
GLfloat light_pos1[] = { 10.0f, -8.0f, 100.00f, 1.0f };
GLfloat light_pos2[] = { -10.0f, 8.0f, 100.00f, 1.0f };

GLFWwindow* window;

double mouse_X, mouse_Y;
vec3 old_p = { 0.0, 0.0, (float)(HEIGHT / 2) };
vec3 the_normal_vector_for_mouse = { 1, 0, 0 };
float thetha_radian = 0.0;
mat4 rotation_matrix = mat4(1.0f);
quat newRot = {1.0, 0.0, 0.0, 0.0};
quat Rot = { 1.0, 0.0, 0.0, 0.0 };

int x_old = 0, y_old = 0;
int current_scroll = 5;
float zoom_per_scroll;

bool first_click = true;

void loadAllShit();
void calculateNormalVectorFromVerteciesThatAreJustLoaded();
void importLightToShader(GLuint ID);
void trackballComputeNormalVector()
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (first_click)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
			first_click = false;
		}
		double mouse_X_new, mouse_Y_new, mouse_Z_new;
		glfwGetCursorPos(window, &mouse_X_new, &mouse_Y_new);
		mouse_X_new = (mouse_X_new - (WIDTH / 2));
		mouse_Y_new = (-(mouse_Y_new)+(HEIGHT / 2));
		double mouse_XY_new = mouse_X_new * mouse_X_new + mouse_Y_new * mouse_Y_new;
		if (mouse_XY_new <= (WIDTH / 2))
		{
			mouse_Z_new = sqrt((long long)(HEIGHT / 2) * (long long)(HEIGHT / 2) - mouse_XY_new);
		}
		else
		{
			mouse_Z_new = ((double)(WIDTH * WIDTH / 2)) / sqrt(mouse_XY_new);
		}
		vec3 new_p = { mouse_X_new, mouse_Y_new, mouse_Z_new };
		new_p = normalize(new_p);
		the_normal_vector_for_mouse = cross(new_p, old_p);
		double arc_shit = dot(old_p, new_p);
		double as = acos(arc_shit);
		//printf("%lf %lf %lf %lf %lf\n", mouse_X_new, mouse_Y_new, mouse_Z_new, arc_shit, as);
		thetha_radian = -(as * 7);
		if (thetha_radian == 0)
		{
			thetha_radian = 0.0;
			the_normal_vector_for_mouse = { 1.0, 0.0, 0.0 };
		}
		glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
	}
	else
	{
		first_click = true;
		thetha_radian = 0.0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void drawDino();



int main(int argc, char** argv)
{
	glfwInit();
	old_p = normalize(old_p);

	// some window and viewport shit
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "dino.obj", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -87;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadAllShit(); // load the file
	calculateNormalVectorFromVerteciesThatAreJustLoaded();
	//deal with shader
	compileVertexShaderAndReturn0("Vertex_Shader.vert", vertexShader);
	compileFragmentShaderAndReturn0("Fragment_Shader.frag", fragmentShader);
	ID_of_shader = glCreateProgram();
	glAttachShader(ID_of_shader, vertexShader);
	glAttachShader(ID_of_shader, fragmentShader);
	glLinkProgram(ID_of_shader);
	glValidateProgram(ID_of_shader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	importLightToShader(ID_of_shader);
	                      //WARNING : SHADER IS ACTIVATED HERE
	
	
	dealWithAllVAOVBOEBO(obj_vertex, faces, VAO_ID, VBO_ID, EBO_ID);
	glBindVertexArray(VAO_ID);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.70f, 0.4f, 0.63f, 1.0f);
		//glClearColor(0.65f, 0.02f, 0.43f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		trackballComputeNormalVector();
		drawDino();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteProgram(ID_of_shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void loadAllShit()
{
	std::string single_line;

	std::ifstream input("Dino.obj");
	if (!input.is_open())
	{
		printf("File Error!\n");
		return;
	}

	float vx, vy, vz;
	int fx, fy, fz;
	while (input.eof() == false)
	{
		std::getline(input, single_line);
		if (single_line[0] == 'v')
		{
			sscanf(single_line.c_str(), "v %f %f %f", &vx, &vy, &vz);
			obj_vertex.push_back({ { vx, vy, vz }, NULL_VEC3 });
		}
		else if (single_line[0] == 'f')
		{
			sscanf(single_line.c_str(), "f %d %d %d", &fx, &fy, &fz);
			faces.push_back(fx - 1);
			faces.push_back(fy - 1);
			faces.push_back(fz - 1);
		}
	}
	return;
}

void calculateNormalVectorFromVerteciesThatAreJustLoaded()
{
	obj_vertex_final.resize(faces.size());
	vec3 vertex_0;
	vec3 vertex_1;
	vec3 vertex_2;

	vec3 normal_temp;

	
	for (int i = 0; i < faces.size(); i += 3)
	{
		vertex_0 = obj_vertex[faces[i]].pos;  //{ obj_vertex[faces[i][0]].x, obj_vertex[faces[i][0]].y, obj_vertex[faces[i][0]].z };
		vertex_1 = obj_vertex[faces[i + 1]].pos;  // { obj_vertex[faces[i][1]].x, obj_vertex[faces[i][1]].y, obj_vertex[faces[i][1]].z };
		vertex_2 = obj_vertex[faces[i + 2]].pos;  // { obj_vertex[faces[i][2]].x, obj_vertex[faces[i][2]].y, obj_vertex[faces[i][2]].z };

		normal_temp = cross(vertex_1 - vertex_0, vertex_2 - vertex_0);
		obj_vertex[faces[i]].norm_vector += normal_temp;
		obj_vertex[faces[i + 1]].norm_vector += normal_temp;
		obj_vertex[faces[i + 2]].norm_vector += normal_temp;
	}

	for (int i = 0; i < obj_vertex.size(); i++)
	{
		obj_vertex[i].norm_vector = normalize(obj_vertex[i].norm_vector);
	}

	return;
}

void importLightToShader(GLuint ID)
{
	vec4 lightColor0 = { 1.0f, 1.0f, 1.0f, 1.0f };
	vec3 lightPos0 = { 165.0f, 169.5f, 30.5f };
	vec4 lightColor1 = { 0.86f, 0.86f, 0.86f, 1.0f };
	vec3 lightPos1 = { -120.0f, -120.0f, -30.0f };
	vec4 lightColor2 = { 0.94f, 0.23f, 0.77f, 1.0f };
	vec3 lightPos2 = { 100.0f, 120.5f, 20.5f };
	vec4 lightColor3 = { 0.64f, 0.23f, 0.83f, 1.0f };
	vec3 lightPos3 = { -90.0f, -100.0f, -30.0f };
	glUseProgram(ID_of_shader);
	glUniform4f(glGetUniformLocation(ID, "lightColor0"), lightColor0.x, lightColor0.y, lightColor0.z, lightColor0.w);
	glUniform3f(glGetUniformLocation(ID, "lightPos"), lightPos0.x, lightPos0.y, lightPos0.z);
	glUniform4f(glGetUniformLocation(ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
	glUniform3f(glGetUniformLocation(ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);
	glUniform4f(glGetUniformLocation(ID, "lightColor2"), lightColor2.x, lightColor2.y, lightColor2.z, lightColor2.w);
	glUniform3f(glGetUniformLocation(ID, "lightPos2"), lightPos2.x, lightPos2.y, lightPos2.z);
	glUniform4f(glGetUniformLocation(ID, "lightColor3"), lightColor3.x, lightColor3.y, lightColor3.z, lightColor3.w);
	glUniform3f(glGetUniformLocation(ID, "lightPos3"), lightPos3.x, lightPos3.y, lightPos3.z);

	return;
}

void drawDino()
{
	mat4 translation_matrix = mat4(1.0f);
	mat4 scale_matrix = mat4(1.0f);
	mat4 model_matrix = mat4(1.0f);



	rotation_matrix = rotate(rotation_matrix, thetha_radian, the_normal_vector_for_mouse);
	                   // PROJECTION * VIEW
	//printf("%f\n", thetha_radian);
	translation_matrix = glm::translate(translation_matrix, glm::vec3(0.0f, 0.0f, -10.0f));


	mat4 look = lookAt(glm::vec3(0.0f, 0.0f, 150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mat4 pers = perspective(glm::radians((float)50), (float)(10.0 / 10.0), 5.0f, 500.0f);
	mat4 camera_view_matrix = pers * look;


	// model_matrix * translation_matrix * rotation_matrix * scale_matrix
	mat4 pre_compute_matrix = model_matrix * translation_matrix * rotation_matrix * scale_matrix;
	
	glUniformMatrix4fv(glGetUniformLocation(ID_of_shader, "camera_matrix"), 1, GL_FALSE, value_ptr(camera_view_matrix));

	glUniformMatrix4fv(glGetUniformLocation(ID_of_shader, "pre_compute_matrix"), 1, GL_FALSE, value_ptr(pre_compute_matrix));

	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
	return;
}

