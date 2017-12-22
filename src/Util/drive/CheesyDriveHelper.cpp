/*
 * CheesyDrive.cpp
 *
 * Ported from:
 * https://github.com/Team254/FRC-2016-Public/blob/master/src/com/team254/frc2016/CheesyDriveHelper.java
 *
 *  Created on: Dec 19, 2017
 *      Author: ryan
 */

#include <Util/drive/CheesyDriveHelper.h>
#include <Util/Util.h>

CheesyDriveHelper::	CheesyDriveHelper(double throttleDeadBand, double wheelDeadBand, double turnSensitivity) {
	mThrottleDeadBand = throttleDeadBand;
	mWheelDeadBand = wheelDeadBand;
	mTurnSensitivity = turnSensitivity;

	mQuickStopAccumulator = 0.0;
}

CheesyDriveHelper::~CheesyDriveHelper() {}

void CheesyDriveHelper::CheesyDrive(double throttle, double wheel, bool isQuickTurn,
		double &leftOutput, double &rightOutput) {

	wheel = Util::HandleDeadband(wheel, mWheelDeadBand);
	throttle = Util::HandleDeadband(throttle, mThrottleDeadBand);

	double overPower;
	double angularPower;

	if(isQuickTurn) {
		if(fabs(throttle) < 0.2) {
			double alpha = 0.1;
			mQuickStopAccumulator = (1 - alpha) * mQuickStopAccumulator
					+ alpha * Util::Limit(wheel, 1.0) * 2;
		}
		overPower = 1.0;
		angularPower = wheel;
	} else {
		overPower = 0.0;
		angularPower = fabs(throttle) * wheel * mTurnSensitivity
				- mQuickStopAccumulator;
		if(mQuickStopAccumulator > 1) {
			mQuickStopAccumulator -= 1;
		} else if(mQuickStopAccumulator < -1) {
			mQuickStopAccumulator += 1;
		} else {
			mQuickStopAccumulator = 0.0;
		}
	}

	double rightPwm = throttle - angularPower;
	double leftPwm = throttle + angularPower;
	if(leftPwm > 1.0) {
		rightPwm -= overPower * (leftPwm - 1.0);
		leftPwm = 1.0;
	} else if(rightPwm > 1.0) {
		leftPwm -= overPower * (rightPwm - 1.0);
		rightPwm = 1.0;
	} else if(leftPwm < -1.0) {
		rightPwm += overPower * (-1.0 - leftPwm);
		leftPwm = -1.0;
	} else if(rightPwm < -1.0) {
		leftPwm += overPower * (-1.0 - rightPwm);
		rightPwm = -1.0;
	}

	leftOutput = leftPwm;
	rightOutput = rightPwm;
}
