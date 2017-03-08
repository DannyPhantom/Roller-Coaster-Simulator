/*
 * Cart.cpp
 *
 *  Created on: 2017-02-04
 *      Author: dannyphantom
 */

#include "Cart.h"
#include "Libraries/glm/gtc/constants.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include <iostream>
#include "Helper.h"

Cart::Cart() {
}

Cart::~Cart() {
	// TODO Auto-generated destructor stub
}

void Cart::update() {

}

void Cart::calculateNextPosition(std::vector<glm::vec3> &points, float distance) {
	int ps = points.size();
	int ip1 = (currentIndexOnCurve+1) % ps;
	glm::vec3 vectorToNextPoint = points[ip1] - position;
	if (glm::length(vectorToNextPoint) > distance) {
		position = position + distance * glm::normalize(vectorToNextPoint);
	} else {
		float sp = glm::length(vectorToNextPoint);
		currentIndexOnCurve = ip1;
		ip1 = (currentIndexOnCurve + 1) % ps;
		while (sp + glm::length((points[ip1] - points[currentIndexOnCurve])) < distance) {
			sp = sp + glm::length((points[ip1] - points[currentIndexOnCurve]));
			currentIndexOnCurve = (currentIndexOnCurve + 1) % ps;
			ip1 = (currentIndexOnCurve + 1) % ps;
		}
		position = points[currentIndexOnCurve] + (distance - sp) * glm::normalize(points[ip1] - points[currentIndexOnCurve]);
	}

	traveledDistance = (traveledDistance + distance);
	if (traveledDistance / totalTrackLength > 1.0f) {
		traveledDistance -= totalTrackLength;
	}
}

double Cart::getTraveledDistancePercentage() {
	return traveledDistance / totalTrackLength;
}
