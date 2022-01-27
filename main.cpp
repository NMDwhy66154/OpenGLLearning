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
#include <io.h>

using namespace std;
using namespace glm;

unsigned int loadTexture(char const* path);
unsigned int loadCubemap(vector<std::string> faces);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void RenderScene();
void RenderSimpleScene(const Shader& shader);
void RenderCube();
void RenderQuad();
void RenderQuadWithTangent();
void RenderCube(const Shader& shader, mat4 model);
void RenderQuad(const Shader& shader, mat4 model,bool hasTangent);

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

float quadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

float points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};

float quadVertices2[] = {
	// 位置          // 颜色
	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};

float planeVertices[] = {
	// positions            // normals         // texcoords
	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
};

unsigned int diffuseTex;
unsigned int specularTex;
unsigned int creativesamTex;
unsigned int grassTex;
unsigned int windowTex;
unsigned int cubemapTexture;
unsigned int woodTex;
unsigned int brickTex;
unsigned int brickNormal;
unsigned int brickHeight;
unsigned int brickRedTex;
unsigned int brickRedNormal;
unsigned int brickDepth;

unsigned int skyboxVAO, skyboxVBO;
unsigned int instancedVAO, instancedVBO;
unsigned int uboMatrices;
unsigned int planeVAO;

mat4 model;
mat4 view;
mat4 projection;

unsigned int amount = 10000;

std::map<float, glm::vec3> sorted;
#define ss SSSSSSS
Shader boxShader;
Shader reflectBoxShader;
Shader lampShader;
Shader modelShader;
Shader grassShader;
Shader screenShader;
Shader skyboxShader;
Shader geomShader;
Shader normalShader;
Shader instancedShader;
Shader rockShader;
Shader planetShader;
Shader debugDepthShader;
Shader debugDepthQuad;
Shader pointLightDepthShader;
Shader pointLightObjShader;
Shader wallShader;
Shader hdrShader;
Shader lightSourceShader;
Shader GBlurShader;

Model sample;
Model planet;
Model rock;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
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

	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	
	mat4* modelMatrices;
	modelMatrices = new mat4[amount];
	srand((unsigned int)glfwGetTime());
	float radius = 75.0f;
	offset = 10.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. 缩放：在 0.05 和 0.25f 之间缩放
		float scale = (float)(rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngle = (float)(rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}

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

	unsigned int screenVBO;
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices) , quadVertices, GL_STATIC_DRAW);
	unsigned int quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int geomVAO, geomVBO;
	glGenBuffers(1, &geomVBO);
	glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glGenVertexArrays(1, &geomVAO);
	glBindVertexArray(geomVAO);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	
	glGenBuffers(1, &instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices2), quadVertices2, GL_STATIC_DRAW);


	glGenVertexArrays(1,&instancedVAO);
	glBindVertexArray(instancedVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 100, &translations[0], GL_STATIC_DRAW);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
	};

	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

#pragma region FrameBuffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

	

	stbi_set_flip_vertically_on_load(true);

	

#pragma region CubeMap
	vector<string> textures_faces;
	string pathFore = "Texture/skybox/";
	string path = "Texture/skybox/*.jpg";
	intptr_t handle;
	struct _finddata_t fileinfo;
	handle = _findfirst(path.c_str(), &fileinfo);
	do
	{
		textures_faces.push_back(pathFore + fileinfo.name);
	} while (!_findnext(handle, &fileinfo));

	_findclose(handle);

	for(string var : textures_faces)
	{
		cout << var << endl;
	}

	cubemapTexture = loadCubemap(textures_faces);
#pragma endregion

#pragma region DepthCubeMap
	GLuint depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	//const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;Already Defined
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLuint newDepthFBO;
	glGenFramebuffers(1, &newDepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, newDepthFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region HDRFBO
	//颜色缓冲生成
	GLuint hdrColorBufferTexture;
	glGenTextures(1, &hdrColorBufferTexture);
	glBindTexture(GL_TEXTURE_2D, hdrColorBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint hdrColorBufferTexture1;
	glGenTextures(1, &hdrColorBufferTexture1);
	glBindTexture(GL_TEXTURE_2D, hdrColorBufferTexture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//深度模板附件(不写入纹理)
	GLuint hdrRbo;
	glGenRenderbuffers(1, &hdrRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

	
	GLuint hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//绑定多个颜色缓冲
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColorBufferTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hdrColorBufferTexture1, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hdrRbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
#pragma endregion
#pragma region Gaussian blur
	GLuint pingpongFBO[2];
	GLuint pingpongBuffer[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}
#pragma endregion



#define sins SINS
	boxShader = Shader("Shader/boxShader/shader.vert", "Shader/boxShader/shader.frag");
	reflectBoxShader = Shader("Shader/boxShader/reflectBox.vert", "Shader/boxShader/reflectBox.frag");
	lampShader = Shader("lampShader.vs", "lampShader.fs");
	modelShader = Shader("Shader/modelShader/modelShader.vert", "Shader/modelShader/modelShader.frag", "Shader/modelShader/modelShader.geom");
	grassShader = Shader("Shader/grassShader/grassShader.vert", "Shader/grassShader/grassShader.frag");
	screenShader = Shader("Shader/screenShader/screenShader.vert", "Shader/screenShader/screenShader.frag");
	skyboxShader = Shader("Shader/skyboxShader/skybox.vert", "Shader/skyboxShader/skybox.frag");
	geomShader = Shader("Shader/geomShader/geomShader.vert", "Shader/geomShader/geomShader.frag", "Shader/geomShader/geomShader.geom");
	normalShader = Shader("Shader/normalShader/normalShader.vert", "Shader/normalShader/normalShader.frag", "Shader/normalShader/normalShader.geom");
	instancedShader = Shader("Shader/instancedShader/instancedShader.vert", "Shader/instancedShader/instancedShader.frag", nullptr);
	rockShader = Shader("Shader/rockShader/rockShader.vert", "Shader/rockShader/rockShader.frag");
	planetShader = Shader("Shader/planetShader/planetShader.vert", "Shader/planetShader/planetShader.frag");
	debugDepthShader = Shader("Shader/depthShader/depthShader.vert", "Shader/depthShader/depthShader.frag");
	debugDepthQuad = Shader("Shader/debug_quad/debug_quad.vert","Shader/debug_quad/debug_quad.frag");
	pointLightDepthShader = Shader("Shader/pointLightDepth/pointLightDepthShader.vert", "Shader/pointLightDepth/pointLightDepthShader.frag", "Shader/pointLightDepth/pointLightDepthShader.geom");
	pointLightObjShader = Shader("Shader/pointLightObj/pointLightObjShader.vert", "Shader/pointLightObj/pointLightObjShader.frag");
	wallShader = Shader("Shader/wallShader/wallShader.vert", "Shader/wallShader/wallShader.frag");
	hdrShader = Shader("Shader/hdrShader/hdrShader.vert","Shader/hdrShader/hdrShader.frag");
	lightSourceShader = Shader("Shader/lightSourceShader/lightSourceShader.vert", "Shader/lightSourceShader/lightSourceShader.frag");
	GBlurShader = Shader("Shader/GBlurShader/GBlurShader.vert", "Shader/GBlurShader/GBlurShader.frag");

	sample = Model("Model/nanosuit_reflect/nanosuit.obj");
	planet = Model("Model/planet/planet.obj");
	rock = Model("Model/rock/rock.obj");

	diffuseTex = loadTexture("Texture/container2.png");
	specularTex = loadTexture("Texture/container2_specular.png");
	creativesamTex = loadTexture("Texture/matrix.jpg");
	grassTex = loadTexture("Texture/grass.png");
	windowTex = loadTexture("Texture/blending_transparent_window.png");
	woodTex = loadTexture("Texture/container2.png");
	brickTex = loadTexture("Texture/brickwall.jpg");
	brickNormal = loadTexture("Texture/brickwall_normal.jpg");
	brickHeight = loadTexture("Texture/parallax_mapping_height_map.png");
	brickDepth = loadTexture("Texture/parallax_mapping_depth_map.jpg");
	brickRedTex = loadTexture("Texture/parallax_mapping_diffuse_map.png");
	brickRedNormal = loadTexture("Texture/parallax_mapping_normal_map.jpg");

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock.meshes.size(); i++)
	{
		unsigned int VAO = rock.meshes[i].VAO;
		glBindVertexArray(VAO);
		GLsizei vec4Size = sizeof(vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2*vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3*vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	reflectBoxShader.use();
	reflectBoxShader.setInt("skybox",0);

	modelShader.use();
	modelShader.setFloat("material.shininess", 32.0f);
	modelShader.setInt("skybox",3);

	boxShader.use();
	boxShader.setVec3("objectColor",1.0f,0.5f,0.31f);
	boxShader.setVec3("viewPos",camera.Position);

	boxShader.setInt("material.texture_diffuse1", 0);
	boxShader.setInt("material.texture_specular1", 1);
	boxShader.setFloat("material.shininess", 32.0f);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

#pragma region Light
	boxShader.use();
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
	//glEnable(GL_STENCIL_TEST);

	glDepthFunc(GL_LEQUAL);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_ZERO, GL_KEEP);
	//glStencilMask(0xFF);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_FRAMEBUFFER_SRGB);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		float distance = glm::length(camera.Position - vegetation[i]);
		sorted[distance] = vegetation[i];
	}


	
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices,0,2*sizeof(mat4));//绑定到索引点0，目标必须是GL_TRANSFORM_FEEDBACK_BUFFER或GL_UNIFORM_BUFFER

	reflectBoxShader.setUniformBlock("Matrices", 0);
	boxShader.setUniformBlock("Matrices", 0);
	modelShader.setUniformBlock("Matrices", 0);
	grassShader.setUniformBlock("Matrices", 0);
	rockShader.setUniformBlock("Matrices", 0);
	planetShader.setUniformBlock("Matrices", 0);
	wallShader.setUniformBlock("Matrices", 0);
	lightSourceShader.setUniformBlock("Matrices", 0);

	vec3 lightPos = vec3(2.0f, 3.0f, 4.0f);

	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near = 1.0f;
	GLfloat far = 25.0f; 
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	pointLightDepthShader.use();
	for (int i = 0; i < 6; i++) {
		pointLightDepthShader.setMat4("shadowMatrices["+to_string(i)+"]", shadowTransforms[i]);
	}
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
#pragma region FillData(UniformBlock)
		view = camera.GetViewMatrix();
		projection = perspective(radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(projection));//填充数据
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(view));//填充数据
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
#pragma endregion
		
	/*
#pragma region DirectionalLightShadow
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		debugDepthShader.use();
		debugDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//仅深度缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		RenderSimpleScene(debugDepthShader);
		glCullFace(GL_BACK);

		view = camera.GetViewMatrix();
		projection = perspective(radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
		//hdr颜色缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glViewport(0, 0, screenWidth, screenHeight);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		debugDepthQuad.use();
		debugDepthQuad.setMat4("view", view);
		debugDepthQuad.setMat4("projection", projection);
		debugDepthQuad.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		debugDepthQuad.setVec3("lightPos", lightPos);
		debugDepthQuad.setVec3("viewPos", camera.Position);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		debugDepthQuad.setInt("shadowMap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodTex);
		debugDepthQuad.setInt("diffuseTexture", 1);
		RenderSimpleScene(debugDepthQuad);
		//默认帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		hdrShader.use();
		hdrShader.setFloat("exposure", 0.1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrColorBufferTexture);
		RenderQuad();
#pragma endregion
	*/	
	/*
#pragma region PointLightShadow
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, newDepthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//configure
		pointLightDepthShader.use();
		pointLightDepthShader.setVec3("lightPos", lightPos);
		pointLightDepthShader.setFloat("far_plane",far);
		//render
		RenderSimpleScene(pointLightDepthShader);

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//configure
		pointLightObjShader.use();
		pointLightObjShader.setMat4("view", view);
		pointLightObjShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodTex);
		pointLightObjShader.setInt("diffuseTexture", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		pointLightObjShader.setInt("shadowMap", 0);
		pointLightObjShader.setVec3("lightPos", lightPos);
		pointLightObjShader.setVec3("viewPos", camera.Position);
		pointLightObjShader.setFloat("far_plane", far);
		pointLightObjShader.setBool("shadows", true);
		//render
		RenderSimpleScene(pointLightObjShader);
#pragma region RenderWall
		wallShader.use();
		wallShader.setVec3("lightPos", lightPos);
		wallShader.setVec3("viewPos", camera.Position);
		wallShader.setFloat("height_scale", 0.14f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brickRedTex);
		wallShader.setInt("diffuseTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, brickRedNormal);
		wallShader.setInt("normalTexture", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brickDepth);
		wallShader.setInt("depthTexture", 2);
		mat4 model;
		wallShader.setMat4("model", model);
		RenderQuad(wallShader, model, true);
#pragma endregion

		
#pragma endregion
	*/
#pragma region Bloom
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		debugDepthShader.use();
		debugDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//仅深度缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		RenderSimpleScene(debugDepthShader);
		glCullFace(GL_BACK);

		view = camera.GetViewMatrix();
		projection = perspective(radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
		//hdr颜色缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glViewport(0, 0, screenWidth, screenHeight);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		debugDepthQuad.use();
		debugDepthQuad.setMat4("view", view);
		debugDepthQuad.setMat4("projection", projection);
		debugDepthQuad.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		debugDepthQuad.setVec3("lightPos", lightPos);
		debugDepthQuad.setVec3("viewPos", camera.Position);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		debugDepthQuad.setInt("shadowMap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodTex);
		debugDepthQuad.setInt("diffuseTexture", 1);
		RenderSimpleScene(debugDepthQuad);
		lightSourceShader.use();
		model = mat4();
		model = translate(model, vec3(-2, 3, -4));
		model = scale(model, vec3(0.5, 0.5, 0.5));
		lightSourceShader.setMat4("model", model);
		RenderCube();
		//高斯模糊缓冲
		GLboolean horizontal = true, first_iteration = true;
		GLuint amount = 10;
		GBlurShader.use();
		for (GLuint i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			GBlurShader.setBool("horizontal", horizontal);
			GBlurShader.setInt("image",0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? hdrColorBufferTexture1 : pingpongBuffer[!horizontal]
			);
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//默认帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrShader.use();
		hdrShader.setFloat("exposure", 0.1);
		hdrShader.setInt("scene", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrColorBufferTexture);
		hdrShader.setInt("bloomBlur",1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]);
		RenderQuad();
#pragma endregion

		
		
#pragma region PostProcessing
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		glDisable(GL_DEPTH_TEST);
//		glClearColor(1, 1, 1, 1);
//		glClear(GL_COLOR_BUFFER_BIT);
//		screenShader.use();
//		glBindVertexArray(quadVAO);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &fbo);
	glDeleteBuffers(1, &hdrFBO);

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

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void RenderScene() {
#pragma region Skybox

	glDepthMask(GL_FALSE);
	skyboxShader.use();
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(true);
#pragma endregion
	view = camera.GetViewMatrix();
	projection = perspective(radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
#pragma region UniformBlock
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(projection));//填充数据
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(view));//填充数据
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
#pragma endregion
#pragma region DrawScene
	boxShader.use();
	mat4 model = mat4(1.0);
	boxShader.setMat4("model", model);


	boxShader.setVec3("viewPos", camera.Position);
	boxShader.setVec3("spotLight.position", camera.Position);
	boxShader.setVec3("spotLight.direction", camera.Front);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex);


#pragma region OpaqueObject
	//不透明物体
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		boxShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubemapTexture);
	reflectBoxShader.use();
	reflectBoxShader.setVec3("cameraPos", camera.Position);
	reflectBoxShader.setInt("skybox", 0);
	model = mat4(1.0f);
	reflectBoxShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	modelShader.use();
	model = translate(model, vec3(-2, -3, -4));
	model = scale(model, vec3(0.2, 0.2, 0.2));
	modelShader.setMat4("model", model);

	modelShader.setVec3("viewPos", camera.Position);
	modelShader.setVec3("spotLight.position", camera.Position);
	modelShader.setVec3("spotLight.direction", camera.Front);
	//modelShader.setFloat("time", glfwGetTime());

	sample.Draw(modelShader);

	normalShader.use();
	normalShader.setMat4("model", model);
	sample.Draw(normalShader);

#pragma region Planet&Rock

	planetShader.use();
	model = mat4(1);
	model = translate(model, vec3(0.0f, -3.0f, 0.0f));
	model = scale(model, vec3(4.0f, 4.0f, 4.0f));
	planetShader.setMat4("model", model);
	planet.Draw(planetShader);
	rockShader.use();
	for (unsigned int i = 0; i < rock.meshes.size(); i++)
	{
		glBindVertexArray(rock.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
	}
#pragma endregion

#pragma endregion


#pragma region SnowHouse
	/*geomShader.use();
	glBindVertexArray(geomVAO);
	glDrawArrays(GL_POINTS, 0, 4);*/
#pragma endregion


#pragma region GPUInstanced
	instancedShader.use();
	glBindVertexArray(instancedVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
#pragma endregion



#pragma region LightSimulater
	//模拟光源
	/*lampShader.use();
	lampShader.setMat4("view", view);
	lampShader.setMat4("projection", projection);
	model = mat4(1.0f);
	model = translate(model, lightPos);
	model = scale(model, vec3(0.2f));
	lampShader.setMat4("model", model);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/
#pragma endregion


#pragma region TransparentObject
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
#pragma endregion

#pragma endregion
}

void RenderSimpleScene(const Shader& shader) {
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(120.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	RenderCube();
}

void RenderCube(const Shader& shader, mat4 model) {
	shader.setMat4("model", model);
	RenderCube();
}

void RenderQuad(const Shader& shader, mat4 model,bool hasTangent = false) {
	shader.setMat4("model", model);
	if (hasTangent) {
		RenderQuadWithTangent();
	}
	else {
		RenderQuad();
	}
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void RenderCube() {
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			 // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left             
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
unsigned int quadVAO = 0;
unsigned int quadVBO;
void RenderQuad() {
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
unsigned int quadVAO2 = 0;
unsigned int quadVBO2;
void RenderQuadWithTangent() {
	if (quadVAO2 == 0)
	{
		// positions
		glm::vec3 pos1(-1.0, 1.0, 0.0);
		glm::vec3 pos2(-1.0, -1.0, 0.0);
		glm::vec3 pos3(1.0, -1.0, 0.0);
		glm::vec3 pos4(1.0, 1.0, 0.0);
		// texture coordinates
		glm::vec2 uv1(0.0, 1.0);
		glm::vec2 uv2(0.0, 0.0);
		glm::vec2 uv3(1.0, 0.0);
		glm::vec2 uv4(1.0, 1.0);
		// normal vector
		glm::vec3 nm(0.0, 0.0, 1.0);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// - triangle 1
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// - triangle 2
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);


		GLfloat quadVertices[] = {
			// Positions            // normal         // TexCoords  // Tangent                          // Bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO2);
		glGenBuffers(1, &quadVBO2);
		glBindVertexArray(quadVAO2);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

