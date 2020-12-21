#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float aCoef = 1.0f;
float bCoef = 1.0f;
float curAngle = 90;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		aCoef += 0.01;
		std::cout << "a - " << aCoef/bCoef << " ";
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		aCoef -= 0.01;
		std::cout << "a - " << aCoef / bCoef << " ";
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		bCoef -= 0.01;
		std::cout << "b - " << aCoef / bCoef << " ";
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		bCoef += 0.01;
		std::cout << "b - " << aCoef / bCoef << " ";
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		curAngle = 90.0f;
		std::cout << "current angle - " << curAngle << " ";
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		curAngle = 60.0f;
		std::cout << "current angle - " << curAngle << " ";
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		curAngle = 45.0f;
		std::cout << "current angle - " << curAngle << " ";
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		curAngle = 30.0f;
		std::cout << "current angle - " << curAngle << " ";
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		curAngle = 0.0f;
		std::cout << "current angle - " << curAngle << " ";
	}
}

GLfloat* LissajousCurve() {
	GLfloat* arr = (GLfloat*)malloc(sizeof(float) * 720);
	float x = -1.0f;
	float dx = 2.0f / 360.0f;

	for (int i = 0; i < 360; i++)
	{
		arr[i * 2] = 0.7f * (float)sin(1.0f * i + 70);
		arr[i * 2 + 1] = 0.4f * (float)sin(1.0f * i);
	}

	return arr;
}

void DynamicDraw(GLfloat * arr) {
	static float a = 1.0f;
	for (int i = 1; i < 360; i++) {
		arr[i * 2 - 2] = arr[i * 2];
		arr[i * 2 - 1] = arr[i * 2 + 1];
	}
	arr[718] = 0.5f * (float)sin(aCoef * glfwGetTime() * 100 + (curAngle * 100));
	arr[719] = 0.5f * (float)sin(bCoef * glfwGetTime() * 100);
}
int main() {
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "PhisicsModel", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "failed" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << glewInit() << std::endl;
	}
	glGetError();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, key_callback);

	Shader shaderProgram("C:\\Users\\dimon\\source\\repos\\OpenGlSolution\\shader.vert", "C:\\Users\\dimon\\source\\repos\\OpenGlSolution\\shader.frag");

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//GLfloat* arr = LissajousCurve();
	
	GLfloat* arr = (GLfloat*)malloc(sizeof(GLfloat) * 720);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 720, arr, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);

	glEnableVertexAttribArray(0);


	/*glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/


	//glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		/*GLfloat time = glfwGetTime();
		GLfloat greenValue = (sin(time) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

		glBindVertexArray(VAO);
		DynamicDraw(arr);
		glDrawArrays(GL_LINE_STRIP, 0, 360);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 720, arr, GL_STREAM_DRAW);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}