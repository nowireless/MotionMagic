#include "DriveWithControllerCommand.h"
#include <Util/drive/ArcadeDriveHelper.h>
#include <Util/Util.h>
#include <XboxController.h>

DriveWithControllerCommand::DriveWithControllerCommand() {
	// Use Requires() here to declare subsystem dependencies
	Requires(chassis.get());
}

// Called just before this Command runs the first time
void DriveWithControllerCommand::Initialize() {
	chassis->Stop();
}

// Called repeatedly when this Command is scheduled to run
void DriveWithControllerCommand::Execute() {
	double move = oi->GetDriver()->GetY(XboxController::kLeftHand);
	move *= -1.0;
	double rotate = oi->GetDriver()->GetX(XboxController::kLeftHand);
	rotate *= -1.0;

	SmartDashboard::PutNumber("Move", move);
	SmartDashboard::PutNumber("Rotate", rotate);

	move = Util::HandleDeadband(move, 0.12);
	rotate = Util::HandleDeadband(rotate, 0.12);

//	printf("Left  Out:%f Pos:%i Vel%i\n", chassis->GetLeft()->GetOutputVoltage(), chassis->GetLeft()->GetEncPosition(), chassis->GetLeft()->GetEncVel());
//	printf("Right Out:%f Pos:%i Vel%i\n", chassis->GetRight()->GetOutputVoltage(), chassis->GetRight()->GetEncPosition(), chassis->GetRight()->GetEncVel());

	double left, right;
	ArcadeDriveHelper::ArcadeDrive(move, rotate, true, left, right);

	SmartDashboard::PutNumber("Left", left);
	SmartDashboard::PutNumber("Right", right);

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
