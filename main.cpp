#define GLM_FORCE_RADIANS

#include <iostream>
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include "Track.h"
#include "Cart.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "CarFollowingCamera.h"
#include "StaticCamera.h"

GLFWwindow *window;
GLuint shaderProgram;
Track *track;
Cart *cart;
Camera *currentCam;
Camera *moveCamera;
CarFollowingCamera *followingCamera;
StaticCamera *staticCamera;
glm::mat4 projectionMatrix;
Object *skybox;
Object *environment;
Object *ship;

double oldX, oldY;

double lastFrame;
bool shipGoingUp = false;

void updateShip(double dt) {
	if (shipGoingUp) {
		ship->updatePosition(glm::vec3(0, dt, 0));
		if (ship->getPosition().y > -25) {
			shipGoingUp = false;
		}
	} else {
		ship->updatePosition(glm::vec3(0, -dt, 0));
		if (ship->getPosition().y < -30) {
			shipGoingUp = true;
		}
	}
}

void render () {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//projection matrix
	float viewAngle = 90.0f;
	if (currentCam == followingCamera) {
		float carSpeed = track->getCurrentCartSpeed();
		float carMaxSpeed = track->getMaxCartSpeed();
		viewAngle = 90.0f * (1 - carSpeed / (carMaxSpeed * 2.0f));
	}
	projectionMatrix = glm::perspective(
			glm::radians(viewAngle),
			1.0f * 860 / 640,
			0.01f,
			10000.0f
	);



	double currentFrame = glfwGetTime();
	double dt = currentFrame-lastFrame;
	lastFrame = currentFrame;


	currentCam->update(dt);
	track->update(dt);
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ProjectionMatrix"), 1, false, &(projectionMatrix[0][0]));
	track->draw(shaderProgram, currentCam->getViewMatrix());
	skybox->draw(shaderProgram, currentCam->getViewMatrix());
	environment->draw(shaderProgram, currentCam->getViewMatrix());
	updateShip(dt);
	ship->draw(shaderProgram, currentCam->getViewMatrix());
}

void keyboard (GLFWwindow *sender, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		currentCam->setMoveCameraLeft(true);
	} else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		currentCam->setMoveCameraForward(true);
	} else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		currentCam->setMoveCameraBackward(true);
	} else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		currentCam->setMoveCameraRight(true);
	}


	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		currentCam->setMoveCameraLeft(false);
	} else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		currentCam->setMoveCameraForward(false);
	} else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		currentCam->setMoveCameraBackward(false);
	} else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		currentCam->setMoveCameraRight(false);
	}

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		if (currentCam == staticCamera) {
			currentCam = followingCamera;
		} else if (currentCam == followingCamera) {
			currentCam = moveCamera;
		} else if (currentCam == moveCamera) {
			currentCam = staticCamera;
		}
	}
}

void mouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	currentCam->processMouseMovement(xpos - oldX, ypos - oldY);
	oldX = xpos;
	oldY = ypos;

	bool shouldChange = false;
	int w, h;
	glfwGetFramebufferSize (window, &w, &h);
	double newXPos = xpos, newYPos = ypos;
	if (xpos > w - 50) {
		newXPos = 50;
		shouldChange = true;
	} else if (xpos < 50) {
		newXPos = w - 50;
		shouldChange = true;
	}

	if (ypos > h - 50) {
		newYPos = 50;
		shouldChange = true;
	} else if (ypos < 50) {
		newYPos = h - 50;
		shouldChange = true;
	}

	if (currentCam == moveCamera && shouldChange) {
		glfwSetCursorPos(window, newXPos, newYPos);
	}
}

void loadShader() {
	ShaderLoader sl;
	shaderProgram = sl.loadShader("Shaders/phong.vert", "Shaders/phong.frag");
}

void setup() {
	loadShader();
	track = new Track();
	track->loadPoints("Tracks/mega2.con");
	ObjLoader loader;
	cart = new Cart();
	cart = static_cast<Cart *>(loader.loadFromFile("Models/car.obj", cart));
	cart->setScale(glm::vec3(0.5, 0.5, 0.5));
	environment = loader.loadFromFile("Models/Lost Jungle/Lost Jungle 1.obj");
	environment -> setPosition(glm::vec3(0, -20, 0));
	environment -> setRotation(glm::vec3(0, glm::radians(90.0f), 0));
	ship = loader.loadFromFile("Models/ship.obj");
	ship->setPosition(glm::vec3(120, -25, 60));
	ship->setScale(glm::vec3(0.0002, 0.0002, 0.0002));
	skybox = loader.loadFromFile("Models/Skybox.obj");
	skybox -> setScale (glm::vec3(200, 200, 200));
	Object *cylinder = loader.loadFromFile("Models/cylinder.obj");
	track->createTrack(cylinder);
	track->addLeadingCart(cart);
	track->addAdditionalCart(new Cart(*cart));
	track->addAdditionalCart(new Cart(*cart));
	track->addAdditionalCart(new Cart(*cart));
	track->calculateAdditionalCartIndexes();
	moveCamera = new Camera();
	moveCamera->setPosition(glm::vec3(40, 100, -50));
	moveCamera->setViewDirection(glm::vec3(10, -100, 80));
	followingCamera = new CarFollowingCamera(cart, track);
	staticCamera = new StaticCamera();
	staticCamera->setPosition(glm::vec3(40, 100, -50));
	staticCamera->setViewDirection(glm::vec3(10, -100, 80));
	currentCam = staticCamera;
}

int main() {
	if (!glfwInit()) {
			return 1;
	}

	window = glfwCreateWindow (860, 640, "CPSC 589 - 1", NULL, NULL);
	if (!window) {
		return 1;
	}

	glfwMakeContextCurrent (window);
	glfwSetKeyCallback (window, keyboard);
	glfwSetScrollCallback(window, mouseScroll);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glewInit();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	setup();
	lastFrame = glfwGetTime();

	int w;
	int h;
	while (!glfwWindowShouldClose (window)) {
		glfwGetFramebufferSize (window, &w, &h);
		glViewport (0, 0, w, h);

		render ();

		glfwSwapBuffers (window);
		glfwPollEvents();
	}

	glfwDestroyWindow (window);
	glfwTerminate();
	return 0;
}
