#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"


Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	textureTarget = TextureTarget;
	fileName = FileName;
}

Texture::Texture(GLenum TextureTarget, GLuint TextureObject) {
	textureTarget = TextureTarget;
	textureObj = TextureObject;
}

bool Texture::Load()
{
	//Use STBI to load to the texture into an array of data
	int width, height, n;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &n, 4); // request RGBA
	//generate the texture and set the parameters
	if (data != NULL)
	{
		glGenTextures(1, &textureObj);
		glBindTexture(textureTarget, textureObj);
		glTexEnvf(textureTarget, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(textureTarget); // Allocate the mipmaps 
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glBindTexture(textureTarget, 0);
		stbi_image_free(data);
		return true;
	}
	else {
		std::cout << "Could not load texture " << fileName << std::endl;
	}

	return false;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(textureTarget, textureObj);
}

void Texture::unBind(GLenum TextureUnit) {
	glActiveTexture(TextureUnit);
	glBindTexture(textureTarget, 0);
}