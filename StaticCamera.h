/*
 * StaticCamera.h
 *
 *  Created on: Feb 6, 2017
 *      Author: dannyphantom
 */

#ifndef STATICCAMERA_H_
#define STATICCAMERA_H_

#include "Camera.h"

class StaticCamera: public Camera {
public:
	StaticCamera();
	virtual ~StaticCamera();

	void update(float dt) {}
	void processMouseMovement(int dx, int dy) {}
};

#endif /* STATICCAMERA_H_ */
