#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	vertexFile.exceptions(std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
   
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex);
    glAttachShader(this->program, fragment);
    glLinkProgram(this->program);
    
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use()
{
    glUseProgram(program);
}
