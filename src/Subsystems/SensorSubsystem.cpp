#include "SensorSubsystem.h"
#include "../RobotMap.h"

#include "Commands/SensorStateUpdateCommand.h"

SensorSubsystem::SensorSubsystem() : Subsystem("ExampleSubsystem") {
//	ahrs_ = new AHRS(SerialPort::kUSB);
}

void SensorSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new SensorStateUpdateCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
