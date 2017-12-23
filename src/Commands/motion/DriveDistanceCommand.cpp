#include <Commands/motion/DriveDistanceCommand.h>

constexpr double kAbsTolerance = 0.5/12.0; //1 inch total tolerance

DriveDistanceCommand::DriveDistanceCommand(double distance, double timeout) {
	// Use Requires() here to declare subsystem dependencies
	Requires(chassis.get());
	distance_ = distance;
	timeout_ = timeout;

	isLeftDone = false;
	isRightDone = false;
}

// Called just before this Command runs the first time
void DriveDistanceCommand::Initialize() {
	chassis->Stop();
	chassis->ZeroEncoders();

	chassis->SetDefaultMotionMagicParams();
	chassis->SetMotionMagicSetPointsFeet(distance_, distance_);

	timer_.Reset();
	timer_.Start();
}

// Called repeatedly when this Command is scheduled to run
void DriveDistanceCommand::Execute() {
	chassis->SetMotionMagicSetPointsFeet(distance_, distance_);

	bool isLeftDoneExec = !chassis->IsLeftBusy();
	bool isRightDoneExec = !chassis->IsRightBusy();

	double leftError = distance_ - chassis->GetLeftDistance();
	double rightError = distance_ - chassis->GetRightDistance();

	isLeftDone = isLeftDoneExec;
	isRightDone = isRightDoneExec;

	printf("%f\t %f\n", chassis->GetLeft()->GetPosition(), chassis->GetRight()->GetPosition());

	printf("Closed Loop Error L:%i, R:%i\n", chassis->GetLeftClosedLoopError(), chassis->GetRightClosedLoopError());
	printf("Wheel Error L:%f, R:%f\n", leftError, rightError);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveDistanceCommand::IsFinished() {
	return (isLeftDone && isRightDone) || timer_.HasPeriodPassed(timeout_);
}

// Called once after isFinished returns true
void DriveDistanceCommand::End() {
	timer_.Stop();
	chassis->Stop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDistanceCommand::Interrupted() {
	this->End();
}
