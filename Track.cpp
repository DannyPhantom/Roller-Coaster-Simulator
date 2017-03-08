/*
 * Track.cpp
 *
 *  Created on: 2017-02-04
 *      Author: dannyphantom
 */

#include "Track.h"
#include <math.h>

using namespace std::chrono;

Track::Track() {
	beadMoveMode = BEAD_MOVE_MODE_CONST_INCREASING;
	length = 0;
}

Track::~Track() {
}

void Track::loadPoints(std::string filePath) {
	using std::string;
	using std::stringstream;
	using std::istream_iterator;

	std::ifstream file( filePath );

	if( !file )
	{
		throw std::runtime_error( "Unable to open file." );
	}

	string line;
	size_t index;
	stringstream ss( std::ios_base::in );

	size_t lineNum = 0;
	points.clear();

	while( getline( file, line ) )
	{
		++lineNum;

		// remove comments
		index = line.find_first_of( "#" );
		if( index != string::npos )
		{
			line.erase( index, string::npos );
		}

		// removes leading/tailing junk
		line.erase( 0, line.find_first_not_of( " \t\r\n\v\f" ) );
		index = line.find_last_not_of( " \t\r\n\v\f" )+1;
		if( index != string::npos )
		{
			line.erase( index, string::npos );
		}

		if( line.empty() )
		{
			continue; // empty or commented out line
		}

		ss.str( line );
		ss.clear();

		try{
			Vec3f v;
			ss >> v;
			glm::vec3 vec(v.m_x, v.m_y, v.m_z);

			if( !ss )
			{
				throw std::runtime_error( "Error read file: "
							 + line
							 + " (line: "
							 + std::to_string(lineNum)
							 +  ")" );
			}
			else
			{
				points.push_back( vec );
			}
		} catch (const std::exception & e) { std::cerr << e.what() << std::endl; }
	}
	file.close();

	subdividePoints();
}

void Track::subdividePoints() {
	//run subdivision 20 times
	for (int i=0; i<8; i++) {
		std::vector<glm::vec3> newPoints;
		for (unsigned int pointNum = 0; pointNum < points.size(); pointNum++) {
			newPoints.push_back(points[pointNum]);
			newPoints.push_back(points[pointNum]/2.0f + points[(pointNum+1)%points.size()]/2.0f);
		}

		for (unsigned int pointNum = 0; pointNum < newPoints.size(); pointNum++) {
			newPoints[pointNum] = (newPoints[pointNum] + newPoints[(pointNum+1)%newPoints.size()]) / 2.0f;
		}
		points = newPoints;
	}

	Helper::setMax(points.size());

	//calculate track length, highest y
	indexToSwitchFromConstToGravity = 0;
	highestY = points[0].y;
	for (unsigned int i=0; i<points.size(); i++) {
		length += glm::length(points[Helper::wrapIndex(i+1)] - points[i]);
		if (points[i].y > highestY) { highestY = points[i].y; }
		if (indexToSwitchFromConstToGravity == 0 && glm::normalize((points[Helper::wrapIndex(i+1)] - points[i])).y < -0.2f) indexToSwitchFromConstToGravity = i;
	}
}

void Track::createTrack(Object *cylinderObject) {
	for (unsigned int i=0; i<points.size() / 2; i++) {
		Object *cylinder1 = new Object(*cylinderObject);
		Object *cylinder2 = new Object(*cylinderObject);
		unsigned int index = i*2;
		glm::vec3 b = calculateBitangentAtIndex(Helper::wrapIndex(index+1));
		glm::mat4 modelMatrix1 = calculateModelMatrixAtIndex(Helper::wrapIndex(index+1));
		glm::mat4 modelMatrix2 = calculateModelMatrixAtIndex(Helper::wrapIndex(index+1));
		modelMatrix1[3][0] += 2 * b.x;
		modelMatrix1[3][1] += 2 * b.y;
		modelMatrix1[3][2] += 2 * b.z;
		modelMatrix2[3][0] -= 2 * b.x;
		modelMatrix2[3][1] -= 2 * b.y;
		modelMatrix2[3][2] -= 2 * b.z;
		float length = glm::length(points[Helper::wrapIndex(index+1)] - points[index]) * 2;
		cylinder1 -> setModelMatrix(modelMatrix1 * glm::scale(glm::mat4(), glm::vec3(1, 1, length)));
		cylinder2 -> setModelMatrix(modelMatrix2 * glm::scale(glm::mat4(), glm::vec3(1, 1, length)));
		this -> cylinders.push_back(cylinder1);
		this -> cylinders.push_back(cylinder2);
	}
}

void Track::draw(GLuint program, glm::mat4 viewMatrix) {
	leadingCart->draw(program, viewMatrix);

	for (Cart *additionalCart : additionalCarts) {
		additionalCart -> draw(program, viewMatrix);
	}

	for (Object *obj : cylinders) {
		obj->draw(program, viewMatrix);
	}
}

void Track::update(float dt) {
	leadingCart->calculateNextPosition(points, getCurrentCartSpeed() * dt);
	calculateCartModelMatrix(leadingCart);
	for (Cart *c : additionalCarts) {
		c->calculateNextPosition(points, getCurrentCartSpeed() * dt);
		calculateCartModelMatrix(c);
	}
}

double Track::getCurrentCartSpeed() {
	if (beadMoveMode == BEAD_MOVE_MODE_CONST_INCREASING) {
		//if we are still going up
		if (leadingCart->getCurrentIndexOnCurve() < indexToSwitchFromConstToGravity) {
			return 3.f;
		} else {
			beadMoveMode = BEAD_MOVE_MODE_GRAVITY;
		}
	}

	if (beadMoveMode == BEAD_MOVE_MODE_GRAVITY) {
		if (leadingCart->getTraveledDistancePercentage() < .95) {
			return sqrtf(2 * 9.8f * (highestY - points[leadingCart->getCurrentIndexOnCurve()].y));
		} else {
			beadMoveMode = BEAD_MOVE_MODE_CONST_DECREASING;
			vDec = sqrtf(2 * 9.8f * (highestY - points[leadingCart->getCurrentIndexOnCurve()].y));
			sDec = 1 - leadingCart->getTraveledDistancePercentage();
		}
	}

	if (beadMoveMode == BEAD_MOVE_MODE_CONST_DECREASING) {
		if (leadingCart->getTraveledDistancePercentage() >= 0.999999) {
			beadMoveMode = BEAD_MOVE_MODE_UNLOADING;
			unloadStartTime = duration_cast< milliseconds >(
					    system_clock::now().time_since_epoch()
					);
			leadingCart->setCurrentIndexOnCurve(0);
		} else {
			return (1 - leadingCart->getTraveledDistancePercentage()) * vDec / sDec;
		}
	}

	if (beadMoveMode == BEAD_MOVE_MODE_UNLOADING){
		double secondsSinceStop = (duration_cast< milliseconds >(system_clock::now().time_since_epoch()) - unloadStartTime).count() / 1000.0f;
		if (secondsSinceStop < 2) {
			return 0;
		} else {
			beadMoveMode = BEAD_MOVE_MODE_CONST_INCREASING;
		}
	}

	return 0.f;
}

glm::vec3 Track::getCurrentCartTangentNormalized(Cart *cart) {
	glm::vec3 tangent = glm::normalize(points[Helper::wrapIndex(cart->getCurrentIndexOnCurve() + 1)] - points[cart->getCurrentIndexOnCurve()]);
	if (tangent.x > .99) tangent.x = 1;
	if (tangent.y > .99) tangent.y = 1;
	if (tangent.z > .99) tangent.z = 1;
	return tangent;
}

void Track::addLeadingCart(Cart *cart) {
	this -> leadingCart = cart;
	leadingCart->setTrackLength(length);
	leadingCart->setPosition(points[0]);
}

void Track::addAdditionalCart(Cart *cart) {
	cart->setTrackLength(length);
	additionalCarts.push_back(cart);
	calculateCartModelMatrix(cart);
}


void Track::calculateAdditionalCartIndexes() {
	const float cartLength = 8;
	int index = points.size() - 1;
	for (Cart *c : additionalCarts) {
		float accumulatedLength = glm::length(points[Helper::wrapIndex(index+1)] - points[index]);
		while (accumulatedLength < cartLength) {
			accumulatedLength += glm::length(points[index] - points[index-1]);
			index--;
		}
		c->setCurrentIndexOnCurve(index);
		c->setPosition(points[index]);
	}
}

void Track::calculateCartModelMatrix(Cart *cart) {
	int index = cart->getCurrentIndexOnCurve();
	glm::vec3 T = getCurrentCartTangentNormalized(cart);
	glm::vec3 N = getAverageNormalAtIndex(index);
	glm::vec3 B = glm::normalize(glm::cross(T, N));
	T = glm::normalize(glm::cross(N, B));
	glm::mat4 modelMatrix = glm::mat4(
			glm::vec4(B, 0),
			glm::vec4(N, 0),
			glm::vec4(T, 0),
			glm::vec4(cart->getPosition() + N * 2.0f, 1));
	cart->setModelMatrix(modelMatrix);
}

glm::vec3 Track::calculateBitangentAtIndex(int index) {
	glm::vec3 T = getCartTangentAtIndexNormalized(index);
	glm::vec3 N = getAverageNormalAtIndex(index);
	return glm::normalize(glm::cross(T, N));
}

glm::mat4 Track::calculateModelMatrixAtIndex(int index) {
	glm::vec3 T = getCartTangentAtIndexNormalized(index);
	glm::vec3 N = getAverageNormalAtIndex(index);
	glm::vec3 B = glm::normalize(glm::cross(T, N));
	T = glm::normalize(glm::cross(N, B));
	glm::mat4 modelMatrix = glm::mat4(
			glm::vec4(B, 0),
			glm::vec4(N, 0),
			glm::vec4(T, 0),
			glm::vec4(points[index], 1));
	return modelMatrix;
}

double Track::getApproxCartSpeedAtIndex(int index) {
	if (index < indexToSwitchFromConstToGravity) {
		return 3.f;
	} else if (1.0f * index / points.size() >= .95) {
		vDec = sqrtf(2 * 9.8f * (highestY - points[(int)points.size() * .95].y));
		sDec = 0.05;
		return (1 - index / points.size()) * vDec / sDec;
	} else {
		double height = points[index].y;
		return sqrtf(2 * 9.8f * (highestY - height));
	}
}

float Track::getMaxCartSpeed() {
	return sqrtf(2 * 9.8f * highestY);
}

glm::vec3 Track::getCartTangentAtIndexNormalized(int index) {
	glm::vec3 tangent = glm::normalize(points[Helper::wrapIndex(index + 1)] - points[index]);
	if (tangent.x > .99) tangent.x = 1;
	if (tangent.y > .99) tangent.y = 1;
	if (tangent.z > .99) tangent.z = 1;
	return tangent;
}

glm::vec3 Track::getNormalAtIndex(int index) {
	float v = getApproxCartSpeedAtIndex(index);
	glm::vec3 normal = v*v * curvature(index) + glm::vec3(0, 9.8, 0) + lacc(index) * getCartTangentAtIndexNormalized(index);
	glm::vec3 T = getCartTangentAtIndexNormalized(index);
	glm::vec3 B = glm::normalize(glm::cross(T, normal));
	return glm::normalize(glm::cross(B, T));
}

glm::vec3 Track::getAverageNormalAtIndex(int index) {
	glm::vec3 normal = glm::vec3(0.0f);
	int offset = 50;
	for (int i=-offset; i<=offset; i++) {
		normal += getNormalAtIndex(Helper::wrapIndex(index + i));
	}

	normal = glm::normalize(normal / (offset+1.0f));
	return normal;
}

glm::vec3 Track::curvature(int index) {
	glm::vec3 temp = points[(index-1)] - 2.0f * points[index] + points[Helper::wrapIndex(index+1)];
	float diff = glm::length(points[index] - points[Helper::wrapIndex(index-1)]) * glm::length(points[Helper::wrapIndex(index+1)] - points[index]);
	return temp / diff;
}

float Track::lacc(int index) {
	float temp = getApproxCartSpeedAtIndex(Helper::wrapIndex(index+1)) - getApproxCartSpeedAtIndex(index);
	float diff = glm::length(points[Helper::wrapIndex(index+1)] - points[index]);
	return temp / diff;
}
