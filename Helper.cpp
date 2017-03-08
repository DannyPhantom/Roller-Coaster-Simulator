/*
 * Helper.cpp
 *
 *  Created on: Feb 6, 2017
 *      Author: dannyphantom
 */

#include "Helper.h"

int Helper::max = 0;

Helper::Helper() {
	// TODO Auto-generated constructor stub

}

Helper::~Helper() {
	// TODO Auto-generated destructor stub
}

int Helper::wrapIndex(int index) {
	if (index >= max) index -= max;
	if (index < 0) index += max;
	return index;
}
