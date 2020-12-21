#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>

class Shader {
public:
	GLuint program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();
};

