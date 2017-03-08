#pragma once

#include <GL/glew.h>
#include <string>

/*
	Class that loads shader from the file
	and passes them to OpenGL
	Supports loading vertex and fragment shaders only
*/
class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader();

	/*
		Loads the 2 shaders from the files: vertex shader and fragment shader
	*/
	GLuint loadShader(std::string vertexShaderPath, std::string fragmentShaderPath);

private:
	/*
		Reads contents from the file
	*/
	std::string readFile(const char * filename);
};
