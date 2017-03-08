#pragma once

#include "Libraries/assimp/scene.h"
#include "Libraries/assimp/Importer.hpp"
#include "Libraries/assimp/postprocess.h"
#include "Libraries/assimp/vector3.h"
#include "Libraries/assimp/vector2.h"
#include "Libraries/assimp/material.h"
#include <GL/glew.h>
#include "Libraries/glm/vec3.hpp"
#include "Libraries/glm/vec2.hpp"
#include "Texture.h"
#include <iostream>
#include <string>
#include "Object.h"

/*
	Loads an object from a file.
	Uses ASSIMP as its underlying library
*/
class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	/*
		Loads an object from the given file
	*/
	Object *loadFromFile(const char* fileName, Object *obj = NULL);

	/*
		Loads an object from the given file
	*/
	Object *loadFromFile(std::string fileName, Object *obj = NULL);

private:
	//directory where the file is stored
	std::string directory;

	/*
		Processes the scene retrieved from the file.
	*/
	Object *processScene(const aiScene *scene, Object *obj = NULL);
};

