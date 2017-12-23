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
constexpr double kMaxVelocity = 150; //RPM

constexpr int kEncoderCount = 360;
constexpr double kGearRatio = 15.0/36.0; // Encdoer -> Wheel ratio
constexpr double kEncoderCodesPerRev = kEncoderCount * 1.0 / kGearRatio;

constexpr int kDefaultProfileSlot = 0;
const CLParams kLeftClosedLoopParams = CLParams(kDefaultProfileSlot, 2.84 * kGearRatio, 0.75, 0, 0);
const CLParams  kRightClosedLoopParams = CLParams(kDefaultProfileSlot, 2.84, 0, 0, 0);

// Not observed in MotionMagicMode as it is not needed
constexpr double kInvertLeft = 1.0;
constexpr double kInvertRight = -1.0;

constexpr double kMMCruiseVel = 50; // RPM
constexpr double kMMAccel = 100; // RPM/sec

constexpr double kCloseLoopRampRate = 240;

Chassis::Chassis() : Subsystem("Chassis") {
	using FeedbackDevice = CANTalon::FeedbackDevice;
	printf("[Chassis] Initializing\n");

	// Left Master
	leftMaster_ = new CANTalon(kLeftDriveMasterId);
	leftMaster_->SetFeedbackDevice(FeedbackDevice::QuadEncoder);
	leftMaster_->GetLowLevelObject().SetRevFeedbackSensor(false);
	leftMaster_->GetLowLevelObject().SetRevMotDuringCloseLoopEn(false);
	leftMaster_->ConfigEncoderCodesPerRev(kEncoderCodesPerRev);

	leftMaster_->ConfigNominalOutputVoltage(+0.0f, -0.0f);
	leftMaster_->ConfigPeakOutputVoltage(+12.0f, -12.0f);

	leftMaster_->SelectProfileSlot(0);
	leftMaster_->SetF(2.84 * 15.0/36.0);
	leftMaster_->SetP(0.75);
	leftMaster_->SetI(0);
	leftMaster_->SetD(0);
	leftMaster_->SetCloseLoopRampRate(kCloseLoopRampRate);


	rightMaster_ = new CANTalon(kRightDriveMasterId);
	rightMaster_->SetFeedbackDevice(FeedbackDevice::QuadEncoder);
	rightMaster_->GetLowLevelObject().SetRevFeedbackSensor(true);
	rightMaster_->GetLowLevelObject().SetRevMotDuringCloseLoopEn(true);
	rightMaster_->ConfigEncoderCodesPerRev(kEncoderCodesPerRev);

	rightMaster_->ConfigNominalOutputVoltage(+0.0f, -0.0f);
	rightMaster_->ConfigPeakOutputVoltage(+12.0f, -12.0f);

	rightMaster_->SelectProfileSlot(0);
	rightMaster_->SetF(2.84 * 15.0/36.0);
	rightMaster_->SetP(0.75);
	rightMaster_->SetI(0);
	rightMaster_->SetD(0);
	rightMaster_->SetCloseLoopRampRate(kCloseLoopRampRate);

	SetDefaultMotionMagicParams();

	// Add to live window
	LiveWindow::GetInstance()->AddActuator("Chassis", "Left", leftMaster_);
	LiveWindow::GetInstance()->AddActuator("Chassis", "Right", rightMaster_);

	printf("[Chassis] Ready\n");
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
		rightMaster_->ConfigNeutralMode(Mode::kNeutralMode_Brake);
	} else {
		leftMaster_->ConfigNeutralMode(Mode::kNeutralMode_Coast);
		rightMaster_->ConfigNeutralMode(Mode::kNeutralMode_Coast);
	}
}

void Chassis::SetMotionMagicParams(double cruiseVel, double accel) {
	// cruiseVel RPM
	// accel RPM per sec
	leftMaster_->SetMotionMagicCruiseVelocity(cruiseVel);
	leftMaster_->SetMotionMagicAcceleration(accel);

	rightMaster_->SetMotionMagicCruiseVelocity(cruiseVel);
	rightMaster_->SetMotionMagicAcceleration(accel);
}

void Chassis::SetMotionMagicParamsFeet(double cruiseVel, double accel) {
	cruiseVel *= 60.0 / kWheelCircumference;
	accel *= 60.0 / kWheelCircumference;

	SetMotionMagicParams(cruiseVel, accel);
}

void Chassis::SetDefaultMotionMagicParams() {
	SetMotionMagicParams(kMMCruiseVel, kMMAccel);
}

void Chassis::ZeroEncoders() {
	leftMaster_->SetPosition(0);
	rightMaster_->SetPosition(0);
}

void Chassis::Stop() {
	SetOpenLoop(0.0, 0.0);
}

void Chassis::SetOpenLoop(double left, double right) {
	SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);

	left *= kInvertLeft;
	right *= kInvertRight;

	leftMaster_->Set(left);
	rightMaster_->Set(right);
}

void Chassis::SetVelocity(double leftVel, double rightVel) {
	SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);

	leftVel *= kInvertLeft;
	rightVel *= kInvertRight;

	leftMaster_->Set(leftVel);
	rightMaster_->Set(rightVel);
}

void Chassis::SetPrecentVelocity(double left, double right) {
	left *= kMaxVelocity;
	right *= kMaxVelocity;
	SetVelocity(left, right);
}

/**
 * Configure and enable motion magic mode
 * @param left Setpoint in encoder rotations
 * @param right Setpoint in encoder rotations
 */
void Chassis::SetMotionMagicSetPoints(double left, double right) {
	SetTalonControlMode(CANTalon::TalonControlMode::kMotionMagicMode);

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
	// CANTalon::GetPosition is in rotations
	return leftMaster_->GetPosition() * kWheelCircumference;
}

double Chassis::GetRightDistance() {
	// CANTalon::GetPosition is in rotations
	return rightMaster_->GetPosition() * kWheelCircumference;
}

double Chassis::GetLeftRate() {
	// CANTalon::GetSpeed is RPM
	return leftMaster_->GetSpeed() * kWheelCircumference / 60.0;
}

double Chassis::GetRightRate() {
	// CANTalon::GetSpeed is RPM
	return rightMaster_->GetSpeed() * kWheelCircumference / 60.0;
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
