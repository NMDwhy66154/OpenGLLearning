#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "HeadFile/shader.h"
#include "HeadFile/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "HeadFile/mesh.h"
#include "HeadFile/stb_image.h"
#include "HeadFile/model.h"
#include <map>

using namespace std;
using namespace glm;

unsigned int loadTexture(char const* path);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
 
unsigned int indices[] = { // 注意索引从0开始! 
	0,2,3,
	0,1,2, // 第一个三角形
};

float texCoords[] = {
	0.0f, 0.0f, // 左下角
	1.0f, 0.0f, // 右下角
	0.5f, 1.0f // 上中
};

unsigned int VBO;

unsigned int VAO;

unsigned int EBO;

float screenWidth = 800;

float screenHeight = 600;

vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float yaw1=-90, pitch1=0;
float fov = 30.0f;

Camera camera = Camera(cameraPos, cameraUp, yaw1, pitch1);

vec3 lightPos(-1.2f, -1.0f, -2.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow((int)screenWidth, (int)screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == nullptr) {
		cout << "Failed"<<endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed GLAD" << endl;
		return -1;
	}
	glViewport(0, 0, (int)screenWidth, (int)screenHeight);

	

	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture,0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	stbi_set_flip_vertically_on_load(true);

	unsigned int diffuseTex = loadTexture("Texture/container2.png");
	unsigned int specularTex = loadTexture("Texture/container2_specular.png");
	unsigned int creativesamTex = loadTexture("Texture/matrix.jpg");
	unsigned int grassTex = loadTexture("Texture/grass.png");
	unsigned int windowTex = loadTexture("Texture/blending_transparent_window.png");

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, creativesamTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassTex);*/


	Shader boxShader("Shader/boxShader/shader.vs", "Shader/boxShader/shader.fs");
	Shader lampShader("lampShader.vs", "lampShader.fs");
	Shader modelShader("Shader/modelShader/modelShader.vs", "Shader/modelShader/modelShader.fs");
	Shader grassShader("Shader/grassShader/grassShader.vert","Shader/grassShader/grassShader.frag");


	modelShader.use();
	Model sample("Model/nanosuit.obj");
	modelShader.setFloat("material.shininess", 32.0f);

	boxShader.use();
	boxShader.setVec3("objectColor",1.0f,0.5f,0.31f);
	boxShader.setVec3("viewPos",camera.Position);

	boxShader.setInt("material.texture_diffuse1", 0);
	boxShader.setInt("material.texture_specular1", 1);
	boxShader.setFloat("material.shininess", 32.0f);

#pragma region Light
	boxShader.setVec3("dirLight.direction", lightPos);
	boxShader.setVec3("dirLight.ambient", vec3(0.2f, 0.2f, 0.2f));
	boxShader.setVec3("dirLight.diffuse", vec3(0.5f, 0.5f, 0.5f));
	boxShader.setVec3("dirLight.specular", vec3(0.5f, 0.5f, 0.5f));

	int length = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
	for (int i = 0; i < length; i++) {
		boxShader.setVec3("pointLight[" + to_string(i) + "]" + ".position", pointLightPositions[i]);

		boxShader.setVec3("pointLight[" + to_string(i) + "]" + ".ambient", vec3(0.2f, 0.2f, 0.2f));
		boxShader.setVec3("pointLight[" + to_string(i) + "]" + ".diffuse", vec3(0.5f, 0.5f, 0.5f));
		boxShader.setVec3("pointLight[" + to_string(i) + "]" + ".specular", vec3(0.5f, 0.5f, 0.5f));

		boxShader.setFloat("pointLight[" + to_string(i) + "]" + ".constant", 1.0f);
		boxShader.setFloat("pointLight[" + to_string(i) + "]" + ".linear", 0.09f);
		boxShader.setFloat("pointLight[" + to_string(i) + "]" + ".quadratic", 0.032f);
	}

	boxShader.setVec3("spotLight.position", camera.Position);
	boxShader.setVec3("spotLight.direction", camera.Front);

	boxShader.setFloat("spotLight.cutOff", cos(radians(12.5f)));
	boxShader.setFloat("spotLight.outerCutOff", cos(radians(17.5f)));

	boxShader.setVec3("spotLight.ambient", vec3(0.2f, 0.2f, 0.2f));
	boxShader.setVec3("spotLight.diffuse", vec3(0.5f, 0.5f, 0.5f));
	boxShader.setVec3("spotLight.specular", vec3(0.5f, 0.5f, 0.5f));
	boxShader.setFloat("spotLight.constant", 1.0f);
	boxShader.setFloat("spotLight.linear", 0.09f);
	boxShader.setFloat("spotLight.quadratic", 0.032f);

	modelShader.use();
	modelShader.setVec3("dirLight.direction", lightPos);
	modelShader.setVec3("dirLight.ambient", vec3(0.2f, 0.2f, 0.2f));
	modelShader.setVec3("dirLight.diffuse", vec3(0.5f, 0.5f, 0.5f));
	modelShader.setVec3("dirLight.specular", vec3(0.5f, 0.5f, 0.5f));

	length = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
	for (int i = 0; i < length; i++) {
		modelShader.setVec3("pointLight[" + to_string(i) + "]" + ".position", pointLightPositions[i]);

		modelShader.setVec3("pointLight[" + to_string(i) + "]" + ".ambient", vec3(0.2f, 0.2f, 0.2f));
		modelShader.setVec3("pointLight[" + to_string(i) + "]" + ".diffuse", vec3(0.5f, 0.5f, 0.5f));
		modelShader.setVec3("pointLight[" + to_string(i) + "]" + ".specular", vec3(0.5f, 0.5f, 0.5f));

		modelShader.setFloat("pointLight[" + to_string(i) + "]" + ".constant", 1.0f);
		modelShader.setFloat("pointLight[" + to_string(i) + "]" + ".linear", 0.09f);
		modelShader.setFloat("pointLight[" + to_string(i) + "]" + ".quadratic", 0.032f);
	}

	modelShader.setVec3("spotLight.position", camera.Position);
	modelShader.setVec3("spotLight.direction", camera.Front);

	modelShader.setFloat("spotLight.cutOff", cos(radians(12.5f)));
	modelShader.setFloat("spotLight.outerCutOff", cos(radians(17.5f)));

	modelShader.setVec3("spotLight.ambient", vec3(0.2f, 0.2f, 0.2f));
	modelShader.setVec3("spotLight.diffuse", vec3(0.5f, 0.5f, 0.5f));
	modelShader.setVec3("spotLight.specular", vec3(0.5f, 0.5f, 0.5f));
	modelShader.setFloat("spotLight.constant", 1.0f);
	modelShader.setFloat("spotLight.linear", 0.09f);
	modelShader.setFloat("spotLight.quadratic", 0.032f);
#pragma endregion

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glDepthFunc(GL_LESS);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_ZERO, GL_KEEP);
	glStencilMask(0xFF);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		float distance = glm::length(camera.Position - vegetation[i]);
		sorted[distance] = vegetation[i];
	}

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		

		boxShader.use();

		mat4 view;
		view = camera.GetViewMatrix();

		mat4 projection;
		projection = perspective(radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		
		mat4 model = mat4(1.0);
		boxShader.setMat4("model", model);
		boxShader.setMat4("view", view);
		boxShader.setMat4("projection", projection);

	
		boxShader.setVec3("viewPos", camera.Position);
		boxShader.setVec3("spotLight.position", camera.Position);
		boxShader.setVec3("spotLight.direction", camera.Front);

		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTex);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTex);

		
		//不透明物体
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			boxShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		modelShader.use();
		model = scale(model, vec3(0.2, 0.2, 0.2));
		modelShader.setMat4("model", model);
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		
		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setVec3("spotLight.position", camera.Position);
		modelShader.setVec3("spotLight.direction", camera.Front);

		sample.Draw(modelShader);

		lampShader.use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		model = mat4(1.0f);
		model = translate(model, lightPos);
		model = scale(model, vec3(0.2f));
		lampShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//透明物体
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, windowTex);
		grassShader.use();
		grassShader.setMat4("view", view);
		grassShader.setMat4("projection", projection);
		grassShader.setInt("grass", 3);
		glBindVertexArray(VAO);
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4();
			model = glm::translate(model, it->second);
			grassShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &fbo);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
	lastX = (float)xpos;
	lastY = (float)ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD,deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

unsigned int loadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	cout << nrChannels << endl;
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else {
		cout << "Failed to load texture" << endl;
		stbi_image_free(data);
	}
	return textureID;

}
