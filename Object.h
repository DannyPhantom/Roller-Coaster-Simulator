#pragma once

#include "Mesh.h"
#include <vector>
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtx/transform.hpp"

/*
	Represents an object in the scene.
	An object has a position, rotation, scale
	and a set of meshes.
*/
class Object
{
protected:
	std::vector<Mesh *> meshes;					//set of object's meshes
	bool vaoSetup = false;						//whether VAO's have already been setup
	bool vboSetup = false;						//whether VBO's have already been setup
	glm::vec3 position, rotation, scale;		//object's position, rotation and scale
	glm::mat4x4 modelMatrix;					//object's model matrix

public:

	Object();
	~Object();

	/*
		Adds a mesh for the object
	*/
	void addMesh(Mesh *mesh) { meshes.push_back(mesh); }

	/*
		Returns all the meshes associated with the object
	*/
	std::vector<Mesh *> *getMeshes() { return &meshes; }

	/*
		Sets up all the VAO's for the object
	*/
	void setupVAOs();

	/*
		Sets up all the VBO's for the object
	*/
	void setupVBOs();

	/*
		Draws the object to the screen
		@param program program to use when drawing the object
		@param viewMatrix view matrix used for the current frame
	*/
	void draw(GLuint program, glm::mat4 viewMatrix);

	/*
		Set a new position for the object. Model matrix will be recalculated.
	*/
	void setPosition(glm::vec3 pos) { position = pos; recalculateModelMatrix(); }

	glm::vec3 getPosition() { return position; }

	/*
		Set a new scale for the object. Model matrix will be recalculated.
	*/
	void setScale(glm::vec3 sc) { scale = sc; recalculateModelMatrix(); }

	/*
		Sets a new rotation for the object. Model matrix will be recalculated.
	*/
	void setRotation(glm::vec3 rot) { rotation = rot; recalculateModelMatrix(); }

	/*
		Updates the current rotation.
		New rotation = old rotation + delta rotation
		Model matrix will be recalculated
	*/
	void updateRotation(glm::vec3 dr) { rotation += dr; recalculateModelMatrix(); }

	/*
		Updates the current position.
		New position = old position + delta position
		Model matrix will be recalculated
	*/
	void updatePosition(glm::vec3 dp) { position += dp; recalculateModelMatrix(); }

	/*
		Updates the current scale.
		New scale = old scale + delta scale
		Model matrix will be recalculated
	*/
	void updateScale(glm::vec3 ds) { scale += ds; recalculateModelMatrix(); }

	/*
		Recalculates the model matrix for the object
	*/
	void recalculateModelMatrix();

	void setModelMatrix(glm::mat4 mm);

};

