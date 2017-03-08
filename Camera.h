#pragma once

#include <iostream>
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/glm.hpp"

/*
	Represents a camera that
	is controlled by the user.
	Camera can move around the world
	and change the view direction.
	View direction is internally represented
	as a combination of two angles.
*/
class Camera
{
public:
	Camera();
	~Camera();

	/*
		Returns the view matrix for the camera
	*/
	glm::mat4 getViewMatrix();

	/*
		Add angles to the camera's view vector
	*/
	void addAngles(float phi, float theta);

	/*
		Processes the mouse movement by the given delta amounts.
		The mouse movements will be transformed into changes in
		view direction
	*/
	virtual void processMouseMovement(int dx, int dy);

	/*
		Issues a command to camera to move forward
	*/
	void setMoveCameraForward(bool move) { shouldBeMovedForward = move; }

	/*
		Issues a command to camera to move backwards
	*/
	void setMoveCameraBackward(bool move) { shouldBeMovedBackwards = move; }

	/*
	Issues a command to camera to move left
	*/
	void setMoveCameraLeft(bool move) { shouldBeMovedLeft = move; }

	/*
	Issues a command to camera to move right
	*/
	void setMoveCameraRight(bool move) { shouldBeMovedRight = move; }

	/*
		Updates the camera position based on the commands issued before
		using setMoveCamera*
	*/
	virtual void update(float dt);

	/*
		Returns camera's position
	*/
	glm::vec3 getPosition();

	/*
		Returns camera's view direction
	*/
	glm::vec3 getViewDirection();

	void setPosition(glm::vec3 pos) {position = pos;}
	void setViewDirection(glm::vec3 dir) {direction = dir;}
	void setUpVector(glm::vec3 up) {this -> up = up;}

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	float phi;
	float theta;

	bool shouldBeMovedForward;
	bool shouldBeMovedBackwards;
	bool shouldBeMovedLeft;
	bool shouldBeMovedRight;

	float cameraMovementSpeed;
};
