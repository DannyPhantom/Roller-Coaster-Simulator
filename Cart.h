/*
 * Cart.h
 *
 *  Created on: 2017-02-04
 *      Author: dannyphantom
 */

#ifndef CART_H_
#define CART_H_

#include <GL/glew.h>
#include <vector>
#include "Libraries/glm/glm.hpp"
#include "Object.h"

class Cart : public Object{
private:
	int currentIndexOnCurve = 0;

	double traveledDistance = 0;
	double totalTrackLength;
public:
	Cart();
	virtual ~Cart();

	void update();
	void calculateNextPosition(std::vector<glm::vec3> &points, float distance);
	int getCurrentIndexOnCurve() {return currentIndexOnCurve;}
	void setCurrentIndexOnCurve(int index) {currentIndexOnCurve = index;}
	double getTraveledDistancePercentage();
	void setTrackLength(double length) {this -> totalTrackLength = length;}
};

#endif /* CART_H_ */
