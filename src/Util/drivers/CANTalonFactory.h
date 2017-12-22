/*
 * CANTalonFactory.h
 *
 *  Created on: Dec 18, 2017
 *      Author: ryan
 */

#ifndef SRC_UTIL_CANTALONFACTORY_H_
#define SRC_UTIL_CANTALONFACTORY_H_

#include <MotorSafety.h>
#include <ctrlib/CANTalon.h>

#include <string>

/**
 * Creates CANTalon objects and configures all the parameters we care about to factory defaults. Closed-loop and sensor
 * parameters are not set, as these are expected to be set by the application.
 */
class CANTalonFactory {
public:
	struct Configuration {
	public:
		bool limitSwitchNormallyOpen;
		double maxOutputVoltage;
		double nominalVoltage;
		double peakVoltage;
		bool enableBreak;
		bool enableCurrentLimit;
		bool enableSoftLimit;
		bool enableLimitSwitch;
		int currentLimit;
		double expirationTimeoutSeconds;
		double forwardSoftLimit;
		bool inverted;
		double nominalClosedLoopVoltage;
		bool referseSoftLimit;
		bool safetyEnabled;

		int controlFramePeriodMS;
		int motionControlFramePeriodMS;
		int generalStatusFrameRateMS;
		int feedbackStatusFrameRateMS;
		int quadEncoderStatusFrameRateMS;
		int analogTempVBatStatusFrameRateMS;
		int pulseWidthStatusFrameRateMS;

		CANTalon::VelocityMeasurementPeriod velocityMeasurementPeriod;
		int velocityMeasurementRollingAverageWindow;

		double voltageCompensationRampRate;
		double voltageRampRate;

		Configuration() :
			limitSwitchNormallyOpen(true),
			maxOutputVoltage(12.0),
			nominalVoltage(0.0),
			peakVoltage(12.0),
			enableBreak(false),
			enableCurrentLimit(false),
			enableSoftLimit(false),
			enableLimitSwitch(false),
			currentLimit(0),
			expirationTimeoutSeconds(DEFAULT_SAFETY_EXPIRATION),
			forwardSoftLimit(0),
			inverted(false),
			nominalClosedLoopVoltage(12.0),
			referseSoftLimit(0),
			safetyEnabled(false),

			controlFramePeriodMS(5),
			motionControlFramePeriodMS(100),
			generalStatusFrameRateMS(5),
			feedbackStatusFrameRateMS(100),
			quadEncoderStatusFrameRateMS(100),
			analogTempVBatStatusFrameRateMS(100),
			pulseWidthStatusFrameRateMS(100),

			velocityMeasurementPeriod(CANTalon::VelocityMeasurementPeriod::Period_100Ms),
			velocityMeasurementRollingAverageWindow(64),

			voltageCompensationRampRate(0),
			voltageRampRate(0) {}
	};


    /**
     * Create a CANTalon with the default (out of the box) configuration.
     */
	static CANTalon* CreateDefaultTalon(int id);

    /**
     * Create a slave CANTalon with the default (out of the box) configuration.
     */
	static CANTalon* CreatePermanentSlaveTalon(int id, int masterId);


	static CANTalon* CreateTalon(int id, Configuration config);

	static std::string GetFullTalonInfo(CANTalon* talon);

private:
	static Configuration kDefaultConfiguration;
	static Configuration kSlaveConfiguration;

	CANTalonFactory();
	virtual ~CANTalonFactory();
};

#endif /* SRC_UTIL_CANTALONFACTORY_H_ */
