#include "ShaderLoader.h"
#include <vector>
#include <iostream>
#include <fstream>


ShaderLoader::ShaderLoader()
{
}


ShaderLoader::~ShaderLoader()
{
}

std::string ShaderLoader::readFile(const char * filename) {
	//create the stream
	std::ifstream ifs(filename);

	if (ifs.fail()) {
		std::cout << "Couldn't load the file " << filename << std::endl;
		return "";
	}

	//read file contents
	std::string fileContents = std::string((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	return fileContents;
}

GLuint ShaderLoader::loadShader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	GLint result = GL_FALSE;
	int logLength;

	//create two default shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders into strings
	std::string vertexShaderCode = readFile(vertexShaderPath.c_str());
	std::string fragmentShaderCode = readFile(fragmentShaderPath.c_str());
	const char *vertexShaderCodeChar = vertexShaderCode.c_str();
	const char *fragmentShaderCodeChar = fragmentShaderCode.c_str();

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertexShader, 1, &vertexShaderCodeChar, NULL);
	glCompileShader(vertexShader);

	// Check vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
		std::cout << "Error occurred when compiling vertex shader." << std::endl;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertexShaderErrorLog((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertexShader, logLength, NULL, &vertexShaderErrorLog[0]);

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragmentShader, 1, &fragmentShaderCodeChar, NULL);
	glCompileShader(fragmentShader);

	// Check fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
		std::cout << "Error occurred when compiling fragment shader." << std::endl;
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> fragmentShaderErrorLog((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragmentShaderErrorLog[0]);
	std::cout << &fragmentShaderErrorLog[0] << std::endl;

	// Link shaders together into a program
	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check linking
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
		std::cout << "Error occurred when linking shaders." << std::endl;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programErrorLog((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programErrorLog[0]);
	std::cout << &programErrorLog[0] << std::endl;

	// Clean up
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
