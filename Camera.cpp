#include "Camera.h"
#include <iostream>


Camera::Camera()
{
	position = glm::vec3(0, 9, 0);
	up = glm::vec3(0, 1, 0);
	phi = 0;
	theta = 0;
	shouldBeMovedForward = false;
	shouldBeMovedBackwards = false;
	shouldBeMovedLeft = false;
	shouldBeMovedRight = false;
	cameraMovementSpeed = 15.0f;

	direction.x = cos(glm::radians(phi)) * cos(glm::radians(theta));
	direction.y = sin(glm::radians(phi));
	direction.z = cos(glm::radians(phi)) * sin(glm::radians(theta));
	direction = glm::normalize(direction);
}


Camera::~Camera()
{
}

void Camera::addAngles(float phi, float theta) {
	this->phi -= phi;
	this->theta += theta;

	//set limits to the view direction (in both horizontal and vertical directions)
	if (this->phi < -70) {
		this->phi = -70;
	}
	else if (this->phi > 70) {
		this->phi = 70;
	}

	direction.x = cos(glm::radians(this->phi)) * cos(glm::radians(this->theta));
	direction.y = sin(glm::radians(this->phi));
	direction.z = cos(glm::radians(this->phi)) * sin(glm::radians(this->theta));
	direction = glm::normalize(direction);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(
		position,
		position + direction,
		up
		);
}

void Camera::processMouseMovement(int dx, int dy) {
	//ignore sudden mouse movements
	if (dx > 200 || dy > 200 || dx < -200 || dy < -200) {
		return;
	}

	if (dx != 0 || dy != 0) {
		addAngles(dy * 0.1f, dx * 0.1f);
	}
}

void Camera::update(float dt) {
	//move the camera in the requested directions
	if (shouldBeMovedBackwards) {
		position -= direction * cameraMovementSpeed * dt;
	}

	if (shouldBeMovedForward) {
		position += direction * cameraMovementSpeed * dt;
	}

	if (shouldBeMovedLeft) {
		glm::vec3 left = -glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		position += left * cameraMovementSpeed * dt;
	}

	if (shouldBeMovedRight) {
		glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		position += right * cameraMovementSpeed  * dt;
	}
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getViewDirection() {
	return direction;
}
