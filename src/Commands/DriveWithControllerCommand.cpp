#include "DriveWithControllerCommand.h"
#include <XboxController.h>

DriveWithControllerCommand::DriveWithControllerCommand() {
	// Use Requires() here to declare subsystem dependencies
	Requires(chassis.get());

	helper_ = new CheesyDriveHelper(0.01, 0.01, 1.0);
}

// Called just before this Command runs the first time
void DriveWithControllerCommand::Initialize() {
	chassis->Stop();
}

// Called repeatedly when this Command is scheduled to run
void DriveWithControllerCommand::Execute() {
	double throtle = oi->GetDriver()->GetX(XboxController::kLeftHand);
	double wheel = oi->GetDriver()->GetY(XboxController::kLeftHand);

	bool quickTurn = oi->GetDriver()->GetAButton();

	double left, right;
	helper_->CheesyDrive(throtle, wheel, quickTurn, left, right);

	chassis->SetOpenLoop(left, right);
	//chassis->SetPrecentVelocity(left, right);

}

// Make this return true when this Command no longer needs to run execute()
bool DriveWithControllerCommand::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveWithControllerCommand::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveWithControllerCommand::Interrupted() {

}
