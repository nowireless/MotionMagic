#include "Chassis.h"
#include "../RobotMap.h"
#include <DriverStation.h>
#include <LiveWindow/LiveWindow.h>
#include <Util/drivers/CANTalonFactory.h>
#include "Commands/DriveWithControllerCommand.h"

using namespace frc;
using CLParams = Chassis::ClosedLoopParams;

constexpr double kWheelRadius = 3.0 / 12.0; //Feet
constexpr double kWheelCircumference = kWheelRadius * 2.0 * M_PI;
constexpr double kMaxVelocity = 1; //Feet/Sec

constexpr int kEncoderTicks = 120;
constexpr int KDistancePerPulse = 1;

constexpr int kDefaultProfileSlot = 0;
const CLParams kLeftClosedLoopParams = CLParams(kDefaultProfileSlot, 0, 0, 0, 0);
const CLParams  kRightClosedLoopParams = CLParams(kDefaultProfileSlot, 0, 0, 0, 0);

Chassis::Chassis() : Subsystem("Chassis") {
	using TalonControlMode = CANTalon::TalonControlMode;
	using FeedbackDevice = CANTalon::FeedbackDevice;
	using FeedbackDeviceStatus = CANTalon::FeedbackDeviceStatus;

	const bool invertLeft = true;
	const bool invertRight = false;

	// Start out in open loop
	mode_ = Mode::kOpenLoop;

	// Left Master
	leftMaster_ = CANTalonFactory::CreateDefaultTalon(kLeftDriveMasterId);
	leftMaster_->ChangeMotionControlFramePeriod(TalonControlMode::kThrottleMode);
	leftMaster_->SetFeedbackDevice(FeedbackDevice::QuadEncoder);
	leftMaster_->ConfigEncoderCodesPerRev(kEncoderTicks);
	leftMaster_->SetEncPosition(0);

	// Right Master
	rightMaster_ = CANTalonFactory::CreateDefaultTalon(kRightDriveMasterId);
	rightMaster_->ChangeMotionControlFramePeriod(TalonControlMode::kThrottleMode);
	rightMaster_->SetFeedbackDevice(FeedbackDevice::QuadEncoder);
	rightMaster_->ConfigEncoderCodesPerRev(kEncoderTicks);
	rightMaster_->SetEncPosition(0);

	// Invert the motors
	// Note: SpeedController::SetInverted only works in PercentVbus, speed, and voltage
	// modes, but not motion magic.
	// So we need to reverse both the motor output and sensor direction
	leftMaster_->SetSensorDirection(invertLeft);
	rightMaster_->SetSensorDirection(invertRight);

	// The C++ CRTE API does not expose reverseOutput like the Java API,
	// so we must access it using the low level API.
	leftMaster_->GetLowLevelObject().SetRevMotDuringCloseLoopEn(invertLeft);
	rightMaster_->GetLowLevelObject().SetRevMotDuringCloseLoopEn(invertRight);

	// Closed loop params
	SetClosedLoopParams(leftMaster_, kLeftClosedLoopParams);
	SetClosedLoopParams(rightMaster_, kRightClosedLoopParams );


	// Create Slave talons
	leftSlave_ = CANTalonFactory::CreatePermanentSlaveTalon(kLeftDriveSlaveId, kLeftDriveMasterId);
	rightSlave_ = CANTalonFactory::CreatePermanentSlaveTalon(kRightDriveSlaveId, kRightDriveMasterId);

	// Check sensor status
	FeedbackDeviceStatus leftSensorStatus = leftMaster_->IsSensorPresent(FeedbackDevice::QuadEncoder);
	if(leftSensorStatus != FeedbackDeviceStatus::FeedbackStatusPresent) {
		DriverStation::GetInstance().ReportError("Could not detect left encoder");
	}

	FeedbackDeviceStatus rightSensorStatus = rightMaster_->IsSensorPresent(FeedbackDevice::QuadEncoder);
	if(rightSensorStatus != FeedbackDeviceStatus::FeedbackStatusPresent) {
		DriverStation::GetInstance().ReportError("Could not detect right encoder");
	}

	// Add to live window
	LiveWindow::GetInstance()->AddActuator("Chassis", "Left", leftMaster_);
	LiveWindow::GetInstance()->AddActuator("Chassis", "Right", rightMaster_);
}

void Chassis::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new DriveWithControllerCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void Chassis::SetClosedLoopParams(CANTalon* talon, ClosedLoopParams params) {
	talon->SelectProfileSlot(params.profileSolt);
	talon->SetF(params.f);
	talon->SetP(params.p);
	talon->SetI(params.i);
	talon->SetD(params.d);
}

void Chassis::SetTalonControlMode(CANTalon::TalonControlMode mode) {
	leftMaster_->SetTalonControlMode(mode);
	rightMaster_->SetTalonControlMode(mode);
}

void Chassis::SetBreak(bool state) {
	using Mode = CANTalon::NeutralMode;
	if(state) {
		leftMaster_->ConfigNeutralMode(Mode::kNeutralMode_Brake);
		leftSlave_->ConfigNeutralMode(Mode::kNeutralMode_Brake);
		rightMaster_->ConfigNeutralMode(Mode::kNeutralMode_Brake);
		rightSlave_->ConfigNeutralMode(Mode::kNeutralMode_Brake);
	} else {
		leftMaster_->ConfigNeutralMode(Mode::kNeutralMode_Coast);
		leftSlave_->ConfigNeutralMode(Mode::kNeutralMode_Coast);
		rightMaster_->ConfigNeutralMode(Mode::kNeutralMode_Coast);
		rightSlave_->ConfigNeutralMode(Mode::kNeutralMode_Coast);
	}
}

void Chassis::SetMotionMagicParamsParams(double cruiseVel, double accel) {
	leftMaster_->SetMotionMagicAcceleration(cruiseVel);
	leftMaster_->SetMotionMagicAcceleration(accel);

	rightMaster_->SetMotionMagicAcceleration(cruiseVel);
	rightMaster_->SetMotionMagicAcceleration(accel);
}

void Chassis::ZeroEncoders() {
	leftMaster_->SetEncPosition(0);
	rightMaster_->SetEncPosition(0);
}

void Chassis::Stop() {
	SetOpenLoop(0.0, 0.0);
}

void Chassis::SetOpenLoop(double left, double right) {
	if(Mode::kOpenLoop != mode_) {
		// Not in open loop
		SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);
	}
	leftMaster_->Set(left);
	rightMaster_->Set(right);
}

void Chassis::SetVelocity(double leftVel, double rightVel) {
	if(Mode::kVelocity != mode_) {
		// Not in velocity control
		SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	}
	leftMaster_->Set(leftVel);
	rightMaster_->Set(rightVel);
}

void Chassis::SetPrecentVelocity(double left, double right) {
	left *= kMaxVelocity;
	right *= kMaxVelocity;
	SetPrecentVelocity(left, right);
}

/**
 * Configure and enable motion magic mode
 * @param left Setpoint in encoder rotations
 * @param right Setpoint in encoder rotations
 */
void Chassis::SetMotionMagicSetPoints(double left, double right) {
	if(Mode::kMotionMagic != mode_) {
		// Not in motion magic mode
		SetTalonControlMode(CANTalon::TalonControlMode::kMotionMagicMode);
	}
	leftMaster_->Set(left);
	rightMaster_->Set(right);
}

void Chassis::SetMotionMagicSetPointsFeet(double left, double right) {
	left /= kWheelCircumference;
	right /= kWheelCircumference;
	SetMotionMagicSetPoints(left, right);
}


bool Chassis::IsLeftBusy() {
	// When the Active Traj Velocity is zero, then the generated motion profile is "done"
	// See: https://www.chiefdelphi.com/forums/showpost.php?p=1661590&postcount=12
	int vel;
	leftMaster_->GetLowLevelObject().GetMotionMagic_ActiveTrajPosition(vel);
	return 0 == vel;
}

bool Chassis::IsRightBusy() {
	// When the Active Traj Velocity is zero, then the generated motion profile is "done"
	// See: https://www.chiefdelphi.com/forums/showpost.php?p=1661590&postcount=12
	int vel;
	rightMaster_->GetLowLevelObject().GetMotionMagic_ActiveTrajPosition(vel);
	return 0 == vel;
}

double Chassis::GetLeftDistance() {
	return leftMaster_->GetEncPosition() * KDistancePerPulse;
}

double Chassis::GetRightDistance() {
	return rightMaster_->GetEncPosition() * KDistancePerPulse;
}

double Chassis::GetLeftRate() {
	return leftMaster_->GetEncVel() * KDistancePerPulse;
}

double Chassis::GetRightRate() {
	return rightMaster_->GetEncVel() * KDistancePerPulse;
}

double Chassis::GetLeftRealOutput() {
	return leftMaster_->GetOutputVoltage() / leftMaster_->GetBusVoltage();
}

double Chassis::GetRightRealOutput() {
	return rightMaster_->GetOutputVoltage() / rightMaster_->GetBusVoltage();
}

int Chassis::GetLeftClosedLoopError() {
	return leftMaster_->GetClosedLoopError();
}

int Chassis::GetRightClosedLoopError() {
	return rightMaster_->GetClosedLoopError();
}
