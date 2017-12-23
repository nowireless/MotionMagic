/*
 * ArcadeDrive.h
 *
 *  Created on: Dec 21, 2017
 *      Author: nowir
 */

#ifndef SRC_UTIL_DRIVE_ARCADEDRIVEHELPER_H_
#define SRC_UTIL_DRIVE_ARCADEDRIVEHELPER_H_

class ArcadeDriveHelper {
public:
	static void ArcadeDrive(double moveValue, double rotateValue,
			bool squaredInputs, double &leftOuput, double &rightOutput);

private:
	ArcadeDriveHelper() {}
	virtual ~ArcadeDriveHelper() {}
};

#endif /* SRC_UTIL_DRIVE_ARCADEDRIVEHELPER_H_ */
