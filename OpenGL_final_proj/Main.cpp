#include <string>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;
#include "Libraries/include/glad/glad.h"
#include "Libraries/include/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.h"
bool yes = true;


float box_vert[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 900;

int shader_cursor = 0;
std::string frag_shaders[] = { "phong.frag", "gouraud.frag" };
std::string vert_shaders[] = { "phong.vert", "gouraud.vert" };

const unsigned int width = 1300;
const unsigned int height = 900;

bool is_phong_shader = true;
bool is_gouraud_shader = false;
bool is_flat_shader = false;
bool is_toon_shader = false;
bool is_rand_shader = false;
bool is_wire_shader = false;

bool show_child_model = false;
bool show_nintendo_model = true;

bool nycu_domitory = true;
bool gray_background = false;


float ambientStrength = 0.1f;
float diffuseStrength = 1.0f;
float specularStrength = 1.0f;

glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
glm::mat4 lightModel = glm::mat4(1.0f);

glm::vec4 lightColor1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec3 lightPos1 = glm::vec3(0.5f, 0.5f, 0.5f);
glm::mat4 lightModel1 = glm::mat4(1.0f);

glm::vec3 model_pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::quat model_rot = glm::quat(0.7071f, 0.0f, 0.7071f, 0.0f);
int model_rot_angle = 0;
bool model_turn = false;

glm::vec3 model0_pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::quat model0_rot = glm::quat(0.7071f, 0.0f, 0.7071f, 0.0f);
int model0_rot_angle = 0;
bool model0_turn = false;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "CG Final Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	gladLoadGL();
	glViewport(0, 0, width, height);


	//init the imgui
	const char* glsl_version = "#version 410";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);




	lightModel = glm::translate(lightModel, lightPos);
	lightModel1 = glm::translate(lightModel1, lightPos1);
	

	//shaderPhong.Activate();


	Shader skyboxShader("skybox.vert", "skybox.frag");
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/OpenGL_final_proj/there/scene.gltf";
	
	// Load in a model
	Model model((parentDir + modelPath).c_str());

	std::string parentDir0 = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath0 = "/OpenGL_final_proj/nintendo/scene.gltf";

	// Load in a model
	Model model0((parentDir0 + modelPath0).c_str());

	std::string facesCubemap[6] =
	{
		parentDir + "/OpenGL_final_proj/skybox/right.jpg",
		parentDir + "/OpenGL_final_proj/skybox/left.jpg",
		parentDir + "/OpenGL_final_proj/skybox/top.jpg",
		parentDir + "/OpenGL_final_proj/skybox/bottom.jpg",
		parentDir + "/OpenGL_final_proj/skybox/front.jpg",
		parentDir + "/OpenGL_final_proj/skybox/back.jpg"
	};

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_vert), &box_vert, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		
		// Draw a model
		

		if (is_phong_shader)
		{
			Shader shaderProgram("phong.vert", "phong.frag");
			shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);
			
			shaderProgram.setFloat("ambientStrength", ambientStrength);
			shaderProgram.setFloat("diffuseStrength", diffuseStrength);
			shaderProgram.setFloat("specularStrength", specularStrength);
			if (show_child_model) model.Draw(shaderProgram, camera, model_pos, model_rot);
			if (show_nintendo_model) model0.Draw(shaderProgram, camera, model0_pos, model0_rot);
			shaderProgram.Delete();
		}
		else if (is_gouraud_shader)
		{
			Shader shaderProgram("gouraud.vert", "gouraud.frag");
			shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);

			shaderProgram.setFloat("ambientStrength", ambientStrength);
			shaderProgram.setFloat("diffuseStrength", diffuseStrength);
			shaderProgram.setFloat("specularStrength", specularStrength);
			if (show_child_model) model.Draw(shaderProgram, camera, model_pos, model_rot);
			if (show_nintendo_model) model0.Draw(shaderProgram, camera, model0_pos, model0_rot);
			//model0.Draw(shaderProgram, camera);
			shaderProgram.Delete();
		}
		else if (is_flat_shader)
		{
			Shader shaderProgram("flat.vert", "flat.frag");
			shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);

			shaderProgram.setFloat("ambientStrength", ambientStrength);
			shaderProgram.setFloat("diffuseStrength", diffuseStrength);
			shaderProgram.setFloat("specularStrength", specularStrength);
			if (show_child_model) model.Draw(shaderProgram, camera, model_pos, model_rot);
			if (show_nintendo_model) model0.Draw(shaderProgram, camera, model0_pos, model0_rot);
			shaderProgram.Delete();
		}
		else if (is_toon_shader)
		{
			Shader shaderProgram("toon.vert", "toon.frag");
			shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);

			shaderProgram.setFloat("ambientStrength", ambientStrength);
			shaderProgram.setFloat("diffuseStrength", diffuseStrength);
			shaderProgram.setFloat("specularStrength", specularStrength);

			if (show_child_model) model.Draw(shaderProgram, camera, model_pos, model_rot);
			if (show_nintendo_model) model0.Draw(shaderProgram, camera, model0_pos, model0_rot);
			shaderProgram.Delete();
		}
		else if (is_rand_shader)
		{
			Shader shaderProgram("rand.vert", "rand.frag");
			shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);

			shaderProgram.setFloat("ambientStrength", ambientStrength);
			shaderProgram.setFloat("diffuseStrength", diffuseStrength);
			shaderProgram.setFloat("specularStrength", specularStrength);
			if (show_child_model) model.Draw(shaderProgram, camera, model_pos, model_rot);
			if (show_nintendo_model) model0.Draw(shaderProgram, camera, model0_pos, model0_rot);
			shaderProgram.Delete();
		}
		else if (is_wire_shader)
		{
			Shader shaderProgram("toon.vert", "toon.frag");
			shaderProgram.Activate();
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor1"), lightColor1.x, lightColor1.y, lightColor1.z, lightColor1.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos1"), lightPos1.x, lightPos1.y, lightPos1.z);

			shaderProgram.setFloat("ambientStrength", ambientStrength);
			shaderProgram.setFloat("diffuseStrength", diffuseStrength);
			shaderProgram.setFloat("specularStrength", specularStrength);
			if (show_child_model) model.Draw(shaderProgram, camera, model_pos, model_rot, true);
			if (show_nintendo_model) model0.Draw(shaderProgram, camera, model0_pos, model0_rot, true);
			shaderProgram.Delete();
		}
		if (nycu_domitory)
		{
			glDepthFunc(GL_LEQUAL);

			skyboxShader.Activate();
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
			projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glDepthFunc(GL_LESS);
		}
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Shaders");
		if (ImGui::Button("enable camera")) 
		{
			if (camera.enable_camera) 
			{
				camera.enable_camera = false;
			}
			else 
			{
				camera.enable_camera = true;
			}
		}
		if (camera.enable_camera)
		{
			ImGui::Text("Press space to unable camera\nWASD to move");
		}

		if (ImGui::RadioButton("Phong Shader", is_phong_shader)) 
		{
			is_gouraud_shader = false;
			is_phong_shader = true;
			is_flat_shader = false;
			is_toon_shader = false;
			is_rand_shader = false;
			is_wire_shader = false;
		}
		if (ImGui::RadioButton("Gouraud Shader", is_gouraud_shader))
		{
			is_gouraud_shader = true;
			is_phong_shader = false;
			is_flat_shader = false;
			is_toon_shader = false;
			is_rand_shader = false;
			is_wire_shader = false;
		}
		if (ImGui::RadioButton("Flat Shader", is_flat_shader))
		{
			is_gouraud_shader = false;
			is_phong_shader = false;
			is_flat_shader = true;
			is_toon_shader = false;
			is_rand_shader = false;
			is_wire_shader = false;
		}
		if (ImGui::RadioButton("Toon Shader", is_toon_shader))
		{
			is_gouraud_shader = false;
			is_phong_shader = false;
			is_flat_shader = false;
			is_toon_shader = true;
			is_rand_shader = false;
			is_wire_shader = false;
		}
		if (ImGui::RadioButton("Vintage Shader", is_rand_shader))
		{
			is_gouraud_shader = false;
			is_phong_shader = false;
			is_flat_shader = false;
			is_toon_shader = false;
			is_rand_shader = true;
			is_wire_shader = false;
		}
		if (ImGui::RadioButton("Show Wireframe", is_wire_shader))
		{
			is_gouraud_shader = false;
			is_phong_shader = false;
			is_flat_shader = false;
			is_toon_shader = false;
			is_rand_shader = false;
			is_wire_shader = true;
		}
		ImGui::SliderFloat("Ambient", &ambientStrength, 0.01f, 3.0f);
		ImGui::SliderFloat("Diffuse", &diffuseStrength, 0.01f, 3.0f);
		ImGui::SliderFloat("Specular", &specularStrength, 0.01f, 5.0f);


		ImGui::End();

		ImGui::Begin("Lighting");

		ImGui::Text("Light 0");
		ImGui::SliderFloat("Light 0 X pos", &lightPos.x, -30.01f, 30.0f);
		ImGui::SliderFloat("Light 0 Y pos", &lightPos.y, -30.01f, 30.0f);
		ImGui::SliderFloat("Light 0 Z pos", &lightPos.z, -30.01f, 30.0f);
		
		ImGui::Text("Light 1");
		ImGui::SliderFloat("Light 1 X pos", &lightPos1.x, -30.01f, 30.0f);
		ImGui::SliderFloat("Light 1 Y pos", &lightPos1.y, -30.01f, 30.0f);
		ImGui::SliderFloat("Light 1 Z pos", &lightPos1.z, -30.01f, 30.0f);
		ImGui::End();


		ImGui::Begin("Models");

		if (ImGui::RadioButton("Show Nintendo Model", show_nintendo_model))
		{
			show_nintendo_model ^= true;
			
		}
		ImGui::SliderFloat("Model 0 X pos", &model0_pos.x, -10.01f, 10.0f);
		ImGui::SliderFloat("Model 0 Y pos", &model0_pos.y, -10.01f, 10.0f);
		ImGui::SliderFloat("Model 0 Z pos", &model0_pos.z, -10.01f, 10.0f);
		if (ImGui::RadioButton("Model 0 Turn", model0_turn))
		{
			model0_turn ^= true;
		}
		ImGui::SliderInt("Model 0 angle", &model0_rot_angle, -30, 30);
		if (model0_turn)
		{
			glm::vec3 rot((float)model0_rot_angle* (float)3.1415 / 180.0, 0, 0);
			model0_rot = glm::normalize(model0_rot * glm::quat(rot));
		}
		//else
		//{
		//	model0_rot = glm::quat(0.7071f, 0.0f, 0.7071f, 0.0f);
		//}
		
		if (ImGui::RadioButton("Show Children Model", show_child_model))
		{
			show_child_model ^= true;
		}
		ImGui::SliderFloat("Model 1 X pos", &model_pos.x, -10.01f, 10.0f);
		ImGui::SliderFloat("Model 1 Y pos", &model_pos.y, -10.01f, 10.0f);
		ImGui::SliderFloat("Model 1 Z pos", &model_pos.z, -10.01f, 10.0f);
		if (ImGui::RadioButton("Model 1 Turn", model_turn))
		{
			model_turn ^= true;
		}
		ImGui::SliderInt("Model 1 angle", &model_rot_angle, -30, 30);
		if (model_turn)
		{
			glm::vec3 rot((float)model_rot_angle * (float)3.1415 / 180.0, 0, 0);
			model_rot = glm::normalize(model_rot * glm::quat(rot));
		}
		ImGui::End();

		ImGui::Begin("Backgrounds");

		if (ImGui::RadioButton("NYCU 8# Domitory", nycu_domitory))
		{
			nycu_domitory = true;
			gray_background = false;
		}
		if (ImGui::RadioButton("Gray Background", gray_background))
		{
			nycu_domitory = false;
			gray_background = true;
		}

		ImGui::End();

		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}



	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
