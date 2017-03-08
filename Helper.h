/*
 * Helper.h
 *
 *  Created on: Feb 6, 2017
 *      Author: dannyphantom
 */

#ifndef HELPER_H_
#define HELPER_H_

class Helper {
public:
	Helper();
	virtual ~Helper();

	static int wrapIndex(int index);
	static void setMax(int max) {Helper::max = max;}

private:
	static int max;
};

#endif /* HELPER_H_ */
