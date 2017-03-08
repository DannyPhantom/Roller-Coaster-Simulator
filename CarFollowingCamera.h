/*
 * CarFollowingCamera.h
 *
 *  Created on: Feb 6, 2017
 *      Author: dannyphantom
 */

#ifndef CARFOLLOWINGCAMERA_H_
#define CARFOLLOWINGCAMERA_H_

#include "Camera.h"
#include "Cart.h"
#include "Track.h"

class CarFollowingCamera: public Camera {
private:
	Cart *toFollow;
	Track *track;
public:
	CarFollowingCamera(Cart *cart, Track* track);
	virtual ~CarFollowingCamera();

	void update(float dt);
};

#endif /* CARFOLLOWINGCAMERA_H_ */
