#pragma once

#include <GL/glew.h>
#include "Libraries/glm/glm.hpp"
#include "Texture.h"
#include <vector>

#define TOTAL_NUMBER_OF_ATTRIBUTES 4

class Mesh
{
private:
	std::vector<glm::vec2> uvs;							//UV's
	std::vector<glm::vec4> colours;						//Colours
	std::vector<glm::vec3> normals;						//Normals
	std::vector<GLuint> indices;						//Indices
	std::vector<glm::vec3> vertices;					//Vertices
	int numOfTriangles;									//Total number of triangles
	GLuint vao, vbo[TOTAL_NUMBER_OF_ATTRIBUTES], ebo;	//VAO, VBO's and EBO
	Texture *texture;									//Texture associated with the mesh (if any)
public:
	Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec4> colours, std::vector<glm::vec3> normals, std::vector<GLuint> indices, Texture *texture, int numOfTriangles) : vertices(vertices), uvs(uvs), colours(colours), normals(normals), texture(texture), indices(indices), numOfTriangles(numOfTriangles) {}
	~Mesh();

	/*
		Identifies if the mesh has a texture
	*/
	bool hasTexture() { return texture != NULL; }

	/*
		Returns mesh's texture
	*/
	Texture *getTexture() { return texture; }

	/*
		Returns indices for the mesh
	*/
	std::vector<GLuint> *getIndices() { return &indices; }

	/*
		Returns vertices for the mesh
	*/
	std::vector<glm::vec3> *getVertices() { return &vertices; }

	/*
		Returns number of triangles for the mesh
	*/
	int getNumOfTriangles() { return numOfTriangles; }

	/*
		Sets up the VBO's for the mesh
	*/
	void setupVBO();

	/*
		Sets up the VAO's for the mesh
	*/
	void setupVAO();

	/*
		Returns object's VAO
	*/
	GLuint getVAO() { return vao; }
};

