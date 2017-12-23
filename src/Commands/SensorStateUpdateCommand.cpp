#include "SensorStateUpdateCommand.h"
#include <SmartDashboard/SmartDashboard.h>

SensorStateUpdateCommand::SensorStateUpdateCommand() {
	// Use Requires() here to declare subsystem dependencies
	Requires(sensors.get());
}

// Called just before this Command runs the first time
void SensorStateUpdateCommand::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void SensorStateUpdateCommand::Execute() {
	SmartDashboard::PutNumber("Chassis Left Encoder Distance", chassis->GetLeftDistance());
	SmartDashboard::PutNumber("Chassis Right Encoder Distance", chassis->GetRightDistance());

	SmartDashboard::PutNumber("Chassis Left Encoder Rate", chassis->GetLeftRate());
	SmartDashboard::PutNumber("Chassis Right Encoder Rate", chassis->GetRightRate());

}

// Make this return true when this Command no longer needs to run execute()
bool SensorStateUpdateCommand::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void SensorStateUpdateCommand::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SensorStateUpdateCommand::Interrupted() {

}
