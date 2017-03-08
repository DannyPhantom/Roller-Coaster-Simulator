#pragma once

#include <string>
#include <iostream>
#include <GL/glew.h>
#include "Libraries/stb_image.h"

/*
	Reperesents an  OpenGL texture
*/
class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);
	Texture(GLenum TextureTarget, GLuint TextureObject);

	/*
		Loads the texture from the given path.
		Assumed that the instance was instantiated using
		the FileName way (i.e. fileName is set as one
		of the isntance variables)
	*/
	bool Load();

	/*
		Binds the texture to the given texture unit,
		i.e. GL_TEXTURE0, GL_TEXTURE1, etc...
	*/
	void Bind(GLenum TextureUnit);

	/*
		Unbinds the texture from the texture unit
	*/
	void unBind(GLenum TextureUnit);

//private:
	std::string fileName;					//name of the file where the texture is contained
	GLenum textureTarget;					//target of the texture (GL_TEXTURE_2D)
	GLuint textureObj;						//Id of the texture
};

