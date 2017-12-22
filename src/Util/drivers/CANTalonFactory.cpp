/*
 * CANTalonFactory.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: ryan
 */

#include <Util/drivers/CANTalonFactory.h>
#include <Util/drivers/LazyCANTalon.h>
#include <sstream>

CANTalonFactory::Configuration CANTalonFactory::kDefaultConfiguration = CANTalonFactory::Configuration();
CANTalonFactory::Configuration makeSlaveConfig() {
	CANTalonFactory::Configuration c;
	c.controlFramePeriodMS = 1000;
	c.motionControlFramePeriodMS = 1000;
	c.generalStatusFrameRateMS = 1000;
	c.feedbackStatusFrameRateMS = 1000;
	c.quadEncoderStatusFrameRateMS = 1000;
	c.analogTempVBatStatusFrameRateMS = 1000;
	c.pulseWidthStatusFrameRateMS = 1000;
	return c;
}
CANTalonFactory::Configuration CANTalonFactory::kSlaveConfiguration = makeSlaveConfig();


CANTalonFactory::CANTalonFactory() {}

CANTalonFactory::~CANTalonFactory() {}

CANTalon* CANTalonFactory::CreateDefaultTalon(int id) {
	return CreateTalon(id, kDefaultConfiguration);
}

CANTalon* CANTalonFactory::CreatePermanentSlaveTalon(int id, int masterId) {
	CANTalon* talon = CreateTalon(id, kSlaveConfiguration);
	talon->ChangeMotionControlFramePeriod(CANTalon::TalonControlMode::kFollowerMode);
	talon->Set(masterId);
	return talon;
}

CANTalon* CANTalonFactory::CreateTalon(int id, Configuration config) {
	using TalonControlMode = CANTalon::TalonControlMode;
	using NeutralMode = CANTalon::NeutralMode;
	using StatusFrameRate = CANTalon::StatusFrameRate;

	CANTalon* talon = new LazyCANTalon(id, config.controlFramePeriodMS);

	// Default control mode
	talon->ChangeMotionControlFramePeriod(TalonControlMode::kThrottleMode);

	talon->ChangeMotionControlFramePeriod(config.motionControlFramePeriodMS);
	talon->ClearIaccum();
	talon->ClearMotionProfileHasUnderrun();
	talon->ClearMotionProfileTrajectories();
	talon->ClearStickyFaults();
	talon->ConfigFwdLimitSwitchNormallyOpen(config.limitSwitchNormallyOpen);
	talon->ConfigMaxOutputVoltage(config.maxOutputVoltage);
	talon->ConfigNominalOutputVoltage(config.nominalVoltage, -config.nominalVoltage);
	talon->ConfigPeakOutputVoltage(config.peakVoltage, -config.peakVoltage);
	talon->ConfigRevLimitSwitchNormallyOpen(config.limitSwitchNormallyOpen);
	if(config.enableBreak) {
		talon->ConfigNeutralMode(NeutralMode::kNeutralMode_Brake);
	} else {
		talon->ConfigNeutralMode(NeutralMode::kNeutralMode_Coast);
	}
	talon->EnableCurrentLimit(config.enableCurrentLimit);
	// talon->EnableForwardSoftLimit(config.enableSoftLimit);
	// talon->EnableLimitSwitch(config.enableLimitSwitch, config.enableLimitSwitch);
	// talon->EnableReverseSoftLimit(config.ENABLE_SOFT_LIMIT);
	talon->EnableZeroSensorPositionOnForwardLimit(false);
	talon->EnableZeroSensorPositionOnIndex(false, false);
	talon->EnableZeroSensorPositionOnReverseLimit(false);
	// talon->ReverseOutput(false);
	// talon->reverseSensor(false);
	talon->SetAnalogPosition(0);
	talon->SetCurrentLimit(config.currentLimit);
	talon->SetExpiration(config.expirationTimeoutSeconds);
	// talon->SetForwardSoftLimit(config.forwardSoftLimit);
	talon->SetInverted(config.inverted);
	talon->SetNominalClosedLoopVoltage(config.nominalClosedLoopVoltage);
	talon->SetPosition(0);
	// talon->SetProfile(0);
	talon->SetPulseWidthPosition(0);
	// talon->SetReverseSoftLimit(config.REVERSE_SOFT_LIMIT);
	talon->SetSafetyEnabled(config.safetyEnabled);
	talon->SetVelocityMeasurementPeriod(config.velocityMeasurementPeriod);
	talon->SetVelocityMeasurementWindow(config.velocityMeasurementRollingAverageWindow);
	talon->SetVoltageCompensationRampRate(config.voltageCompensationRampRate);
	talon->SetVoltageRampRate(config.voltageRampRate);

	talon->SetStatusFrameRateMs(StatusFrameRate::StatusFrameRateGeneral, config.generalStatusFrameRateMS);
	talon->SetStatusFrameRateMs(StatusFrameRate::StatusFrameRateFeedback, config.feedbackStatusFrameRateMS);
	talon->SetStatusFrameRateMs(StatusFrameRate::StatusFrameRateQuadEncoder, config.quadEncoderStatusFrameRateMS);
	talon->SetStatusFrameRateMs(StatusFrameRate::StatusFrameRateAnalogTempVbat,
			config.analogTempVBatStatusFrameRateMS);
	talon->SetStatusFrameRateMs(StatusFrameRate::StatusFrameRatePulseWidthMeas, config.pulseWidthStatusFrameRateMS);

	return talon;
}

std::string CANTalonFactory::GetFullTalonInfo(CANTalon* talon) {
	return "";
}
