/*
 * CheesyDrive.h
 *
 *  Created on: Dec 19, 2017
 *      Author: ryan
 */

#ifndef SRC_UTIL_DRIVE_CHEESYDRIVEHELPER_H_
#define SRC_UTIL_DRIVE_CHEESYDRIVEHELPER_H_

class CheesyDriveHelper {
public:
	CheesyDriveHelper(double throttleDeadBand, double wheelDeadBand, double turnSensitivity);
	virtual ~CheesyDriveHelper();

	void CheesyDrive(double throttle, double wheel, bool isQuickTurn, double &leftOutput, double &rightOutput);
private:
	double mQuickStopAccumulator;

	double mThrottleDeadBand;
	double mWheelDeadBand;
	double mTurnSensitivity;
};

#endif /* SRC_UTIL_DRIVE_CHEESYDRIVEHELPER_H_ */
