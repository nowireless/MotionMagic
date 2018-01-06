/*
 * ArcadeDrive.cpp
 *
 *  Created on: Dec 21, 2017
 *      Author: ryan
 */

#include <Util/drive/ArcadeDriveHelper.h>
#include <Util/Util.h>

#include <algorithm>


/**
 * Arcade drive implements single stick driving.
 *
 * This function lets you directly provide joystick values from any source.
 *
 * @param moveValue     Positive for forwards/Negative for backwards
 * @param rotateValue   Positive for left/Negative for right
 * @param squaredInputs If set, increases the sensitivity at low speeds
 */
void ArcadeDriveHelper::ArcadeDrive(double moveValue, double rotateValue,
		bool squaredInputs, double &leftOutput, double &rightOutput) {

	// local variables to hold the computed PWM values for the motors
	moveValue = Util::Limit(moveValue, 1.0);
	rotateValue = Util::Limit(rotateValue, 1.0);

	// square the inputs (while preserving the sign) to increase fine control
	// while permitting full power
	if(squaredInputs) {
		moveValue = std::copysign(moveValue * moveValue, moveValue);
		rotateValue = std::copysign(rotateValue * rotateValue, rotateValue);
	}

	if(moveValue > 0.0) {
		if(rotateValue > 0.0) {
			leftOutput = moveValue - rotateValue;
			rightOutput = std::max(moveValue, rotateValue);
		} else {
			leftOutput = std::max(moveValue, -rotateValue);
			rightOutput = moveValue + rotateValue;
		}
	} else {
		if(rotateValue > 0.0) {
			leftOutput = -std::max(-moveValue, rotateValue);
			rightOutput = moveValue + rotateValue;
		} else {
			leftOutput = moveValue - rotateValue;
			rightOutput = -std::max(-moveValue, -rotateValue);
		}
	}


}
