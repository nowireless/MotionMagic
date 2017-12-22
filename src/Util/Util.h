/*
 * Utils.h
 *
 * https://github.com/Team254/FRC-2012/tree/master/src/config
 * https://github.com/Team254/FRC-2016-Public/blob/master/src/com/team254/lib/util/Util.java
 *
 *  Created on: Dec 19, 2017
 *      Author: ryan
 */

#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

#include <cmath>



namespace Util {

/**
 * @author Eric Bakan
 * Useful functions which are repeated across classes
 */

/**
 * Implements a deadband on a joystick
 * @param val the joystick value
 * @param deadband the maximum value the deadband will return 0 for
 * @return 0.0 if the absolute value of the joystick value is less than the deadband, else the joystick value
 */
inline double HandleDeadband(double val, double deadband) {
	return (fabs(val) > fabs(deadband)) ? val : 0.0;
}

/**
 * Limits the given input to the given magnitude.
 */
inline double Limit(double v, double limit) {
	return (fabs(v) < limit) ? v : limit * (v < 0 ? -1 : 1);
}

}

#endif /* SRC_UTIL_UTIL_H_ */
