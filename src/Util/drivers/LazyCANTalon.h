/*
 * LazyCANTalon.h
 *
 *  Created on: Dec 18, 2017
 *      Author: ryan
 */

#ifndef SRC_UTIL_LAZYCANTALON_H_
#define SRC_UTIL_LAZYCANTALON_H_

#include <ctrlib/CANTalon.h>

/**
 * This class is a thin wrapper around the CANTalon that reduces CAN bus / CPU overhead by skipping duplicate set
 * commands. (By default the Talon flushes the Tx buffer on every set call).
 */
class LazyCANTalon: public CANTalon {
public:
	LazyCANTalon(int deviceNumber);
	LazyCANTalon(int deviceNumber, int controlPeriodMS);
	virtual ~LazyCANTalon();

	virtual void set(double value);
protected:
	double lastSet_;
	CANTalon::TalonControlMode lastContolMode_;
};

#endif /* SRC_UTIL_LAZYCANTALON_H_ */
