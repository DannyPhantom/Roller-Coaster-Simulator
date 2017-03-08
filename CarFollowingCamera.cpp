/*
 * CarFollowingCamera.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: dannyphantom
 */

#include "CarFollowingCamera.h"

CarFollowingCamera::CarFollowingCamera(Cart *cart, Track *track) {
	toFollow = cart;
	this -> track = track;
}

CarFollowingCamera::~CarFollowingCamera() {
	// TODO Auto-generated destructor stub
}

void CarFollowingCamera::update(float dt) {
	setPosition(toFollow->getPosition() + track->getAverageNormalAtIndex(toFollow->getCurrentIndexOnCurve()) * 4.5f);
	setViewDirection(track->getCurrentCartTangentNormalized(toFollow));
	setUpVector(track->getAverageNormalAtIndex(toFollow->getCurrentIndexOnCurve()));
}
