#include <Commands/motion/DriveDistanceCommand.h>

constexpr double kAbsTolerance = 0.5/12.0; //1 inch total tolerance

DriveDistanceCommand::DriveDistanceCommand(double distance, double velocity, double accel, double timeout) {
	// Use Requires() here to declare subsystem dependencies
	Requires(chassis.get());
	distance_ = distance;
	velocity_ = velocity;
	accel_ = accel;
	timeout_ = timeout;

	isLeftDone = false;
	isRightDone = false;

}

// Called just before this Command runs the first time
void DriveDistanceCommand::Initialize() {
	chassis->Stop();
	chassis->ZeroEncoders();

//	chassis->GetLeft()->SetCloseLoopRampRate(240);
//	chassis->GetRight()->SetCloseLoopRampRate(240);

//	chassis->SetMotionMagicParams(50, 100);
//	chassis->SetMotionMagicSetPoints(4,4);

	//chassis->SetMotionMagicParamsFeet(velocity_, accel_);
	//chassis->SetMotionMagicSetPointsFeet(distance_, distance_);

	chassis->GetLeft()->SetTalonControlMode(CANTalon::kMotionMagicMode);
	chassis->GetRight()->SetTalonControlMode(CANTalon::kMotionMagicMode);

	timer_.Reset();
	timer_.Start();
}

// Called repeatedly when this Command is scheduled to run
void DriveDistanceCommand::Execute() {
	chassis->GetLeft()->SetTalonControlMode(CANTalon::kMotionMagicMode);
	chassis->GetRight()->SetTalonControlMode(CANTalon::kMotionMagicMode);
	chassis->GetLeft()->Set(10);
	chassis->GetRight()->Set(10);

	bool isLeftDoneExec = !chassis->IsLeftBusy();
	bool isRightDoneExec = !chassis->IsRightBusy();

	double leftError = distance_ - chassis->GetLeftDistance();
	double rightError = distance_ - chassis->GetRightDistance();

//	bool isLeftInTolerance = fabs(leftError) < kAbsTolerance;
//	bool isRightInTolerance = fabs(leftError) < kAbsTolerance;

//	isLeftDone = isLeftDoneExec && isLeftInTolerance;
//	isRightDone = isRightDoneExec && isRightInTolerance;

	int mmVel;
	chassis->GetLeft()->GetLowLevelObject().GetMotionMagic_ActiveTrajPosition(mmVel);

	printf("%f\t %f\n", chassis->GetLeft()->GetPosition(), chassis->GetRight()->GetPosition());

//	printf("MMVel %i\n", mmVel);

//	printf("Output L:%f, R:%f\n", chassis->GetLeftRealOutput(), chassis->GetRightRealOutput());
//	printf("Closed Loop Error L:%i, R:%i\n", chassis->GetLeftClosedLoopError(), chassis->GetRightClosedLoopError());
//	printf("Wheel Error L:%f, R:%f\n", leftError, rightError);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveDistanceCommand::IsFinished() {
	return (isLeftDone && isRightDone) || timer_.HasPeriodPassed(20);
}

// Called once after isFinished returns true
void DriveDistanceCommand::End() {
	timer_.Stop();
	chassis->GetLeft()->SetTalonControlMode(CANTalon::kThrottleMode);
	chassis->GetRight()->SetTalonControlMode(CANTalon::kThrottleMode);
	chassis->Stop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDistanceCommand::Interrupted() {
	this->End();
}
