#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera3D.h"
#include "Object3D.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "global_defines.h"

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera3D camera;

bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

Object3D step1Quad;
glm::vec3 step1QuadPosition = glm::vec3(-4.0f, 2.0f, 0.0f);
Shader step1QuadShader;

Object3D step2Quad;
glm::vec3 step2QuadPosition = glm::vec3(-3.0f, 2.0f, 0.0f);
Shader step2QuadShader;

Object3D step3Quad;
glm::vec3 step3QuadPosition = glm::vec3(-2.0f, 2.0f, 0.0f);
Shader step3QuadShader;

Object3D step4Quad;
glm::vec3 step4QuadPosition = glm::vec3(-1.0f, 2.0f, 0.0f);
Shader step4QuadShader;

enum direction
{
	DIR_FORWARD,
	DIR_BACK,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	_DIR_LAST_
};
bool movement[_DIR_LAST_];

int main();

bool initWindow(int* code);
bool initGL(int* code);

void update();
void render();

void renderScene();

int main()
{
	int code;
	if (!initWindow(&code))
		return code;

	camera.init();

	if (!initGL(&code))
		return code;

	// Init Movements
	for (int i = 0; i < _DIR_LAST_; i++)
		movement[i] = false;

	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)) //The render loop
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		update();

		render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	printf("Closing.\n");

	step1QuadShader.deleteProgram();
	step2QuadShader.deleteProgram();
	step3QuadShader.deleteProgram();
	step4QuadShader.deleteProgram();

	step1Quad.clearGPU();
	step2Quad.clearGPU();
	step3Quad.clearGPU();
	step4Quad.clearGPU();

	glfwTerminate();
	window = NULL;

	return 0;
}

bool initWindow(int *code)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fire Shader Study", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window.");
		glfwTerminate();
		*code = -1;
		return false;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD.");
		*code = -1;
		return false;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //Origin is at lower left corner
	//Viewport coordinates are between [-1, 1]

	//We register the callback functions after we've created
	//the window and before the render loop is initiated.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	code = 0;
	return true;
}

bool initGL(int* code)
{
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	step1QuadShader.init("resources/shaders/plain_quad.vertex", "resources/shaders/step_1_noise.fragment");
	step1Quad.loadObjFromDisk("resources/objects/Quad.txt");
	step1Quad.setPosition(step1QuadPosition);
	step1Quad.setShader(&step1QuadShader);

	step2QuadShader.init("resources/shaders/plain_quad.vertex", "resources/shaders/step_2_noise.fragment");
	step2Quad.loadObjFromDisk("resources/objects/Quad.txt");
	step2Quad.setPosition(step2QuadPosition);
	step2Quad.setShader(&step2QuadShader);

	step3QuadShader.init("resources/shaders/plain_quad.vertex", "resources/shaders/step_3_noise_avg.fragment");
	step3Quad.loadObjFromDisk("resources/objects/Quad.txt");
	step3Quad.setPosition(step3QuadPosition);
	step3Quad.setShader(&step3QuadShader);

	step4QuadShader.init("resources/shaders/plain_quad.vertex", "resources/shaders/step_4_fbm.fragment");
	step4Quad.loadObjFromDisk("resources/objects/Quad.txt");
	step4Quad.setPosition(step4QuadPosition);
	step4Quad.setShader(&step4QuadShader);

	code = 0;
	return true;
}

void update()
{
	camera.update(movement);
}

void render()
{
	renderScene();
}

int UniformViewM;
int UniformProjectionM;

void renderScene()
{
	//glClearColor(sin(glfwGetTime()), 0.3f, 0.3f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	step1QuadShader.use();
	UniformViewM = glGetUniformLocation(step1QuadShader.getID(), "uView");
	UniformProjectionM = glGetUniformLocation(step1QuadShader.getID(), "uProjection");
	camera.setUniformProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, UniformProjectionM);
	camera.setUniformViewMatrix(UniformViewM);
	step1QuadShader.setFloat("uTime", glfwGetTime());
	step1Quad.render();

	step2QuadShader.use();
	UniformViewM = glGetUniformLocation(step2QuadShader.getID(), "uView");
	UniformProjectionM = glGetUniformLocation(step2QuadShader.getID(), "uProjection");
	camera.setUniformProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, UniformProjectionM);
	camera.setUniformViewMatrix(UniformViewM);
	step2QuadShader.setFloat("uTime", glfwGetTime());
	step2Quad.render();

	step3QuadShader.use();
	UniformViewM = glGetUniformLocation(step3QuadShader.getID(), "uView");
	UniformProjectionM = glGetUniformLocation(step3QuadShader.getID(), "uProjection");
	camera.setUniformProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, UniformProjectionM);
	camera.setUniformViewMatrix(UniformViewM);
	step3QuadShader.setFloat("uTime", glfwGetTime());
	step3Quad.render();

	step4QuadShader.use();
	UniformViewM = glGetUniformLocation(step4QuadShader.getID(), "uView");
	UniformProjectionM = glGetUniformLocation(step4QuadShader.getID(), "uProjection");
	camera.setUniformProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, UniformProjectionM);
	camera.setUniformViewMatrix(UniformViewM);
	step4QuadShader.setFloat("uTime", glfwGetTime());
	step4Quad.render();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movement[DIR_FORWARD] = true;
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		movement[DIR_FORWARD] = false;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movement[DIR_LEFT] = true;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		movement[DIR_LEFT] = false;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movement[DIR_BACK] = true;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		movement[DIR_BACK] = false;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movement[DIR_RIGHT] = true;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		movement[DIR_RIGHT] = false;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		movement[DIR_UP] = true;
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
		movement[DIR_UP] = false;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movement[DIR_DOWN] = true;
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
		movement[DIR_DOWN] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.setHeadTurn(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.setAddZoom(static_cast<float>(yoffset));
}
