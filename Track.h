/*
 * Track.h
 *
 *  Created on: 2017-02-04
 *      Author: dannyphantom
 */

#ifndef TRACK_H_
#define TRACK_H_

#include <string>
#include <vector>
#include <cstddef>
#include <sstream>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "Libraries/glm/glm.hpp"
#include <GL/glew.h>
#include "Cart.h"
#include <chrono>
#include "Helper.h"

using namespace std::chrono;

struct Vec3f
{
	Vec3f( float x = 0.f, float y = 0.f, float z = 0.f ) :
		m_x( x ),
		m_y( y ),
		m_z( z )
	{}

	float m_x, m_y, m_z;

	friend std::istream & operator >> ( std::istream & in, Vec3f & vec )
	{
		float x, y, z;

		if( in >> x >> y >> z )
		{
			vec.m_x = x;
			vec.m_y = y;
			vec.m_z = z;
		}

		return in;
	}

	friend std::ostream & operator << ( std::ostream & out, Vec3f const & vec )
	{
		return out << vec.m_x << " " << vec.m_y << " " << vec.m_z;
	}
};

class Track {
private:
	std::vector<Object*> cylinders;
	enum BeadMoveMode {
		BEAD_MOVE_MODE_CONST_INCREASING,
		BEAD_MOVE_MODE_GRAVITY,
		BEAD_MOVE_MODE_CONST_DECREASING,
		BEAD_MOVE_MODE_UNLOADING,
	} beadMoveMode;

	int indexToSwitchFromConstToGravity;
	milliseconds unloadStartTime;

	std::vector<glm::vec3> points;
	Cart *leadingCart;
	std::vector<Cart*> additionalCarts;

	double length;
	float highestY;

	//speed at deceleration
	double vDec;
	//distance at deceleration
	double sDec;

	void subdividePoints();
	double getApproxCartSpeedAtIndex(int index);
	void calculateCartModelMatrix(Cart *cart);
	glm::vec3 calculateBitangentAtIndex(int index);
	glm::mat4 calculateModelMatrixAtIndex(int index);
	glm::vec3 getCartTangentAtIndexNormalized(int index);
	glm::vec3 getNormalAtIndex(int index);
	glm::vec3 curvature(int index);
	float lacc(int index);

public:
	Track();
	virtual ~Track();

	void loadPoints(std::string filePath);
	void createTrack(Object *cylinderObject);

	float getMaxCartSpeed();
	glm::vec3 getAverageNormalAtIndex(int index);
	double getCurrentCartSpeed();
	glm::vec3 getCurrentCartTangentNormalized(Cart *cart);
	void draw(GLuint program, glm::mat4 viewMatrix);
	void update(float dt);
	void addLeadingCart(Cart *cart);
	void addAdditionalCart(Cart *cart);
	void calculateAdditionalCartIndexes();
};

#endif /* TRACK_H_ */
