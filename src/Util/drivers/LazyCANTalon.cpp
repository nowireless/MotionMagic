/*
 * LazyCANTalon.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: ryan
 */

#include <ctrlib/CANTalon.h>
#include <Util/drivers/LazyCANTalon.h>

#include <limits>

LazyCANTalon::LazyCANTalon(int deviceNumber)
	: CANTalon(deviceNumber),
	  lastSet_(std::numeric_limits<double>::quiet_NaN()),
	  lastContolMode_(CANTalon::TalonControlMode::kDisabled) {}

LazyCANTalon::LazyCANTalon(int deviceNumber, int controlPeriodMS)
	: CANTalon(deviceNumber, controlPeriodMS),
	  lastSet_(std::numeric_limits<double>::quiet_NaN()),
      lastContolMode_(CANTalon::TalonControlMode::kDisabled) {}


LazyCANTalon::~LazyCANTalon() {}

void LazyCANTalon::set(double value) {
	if(value != lastSet_ || GetTalonControlMode() != lastContolMode_) {
		lastSet_ = value;
		lastContolMode_ = GetTalonControlMode();
		CANTalon::Set(value);
	}
}
