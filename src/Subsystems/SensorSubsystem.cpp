#include "SensorSubsystem.h"
#include "../RobotMap.h"

#include "Commands/SensorStateUpdateCommand.h"

SensorSubsystem::SensorSubsystem() : Subsystem("SensorSubsystem") {
	printf("[SensorSubsystem] Initializing\n");
	try {
		ahrs_ = nullptr;//new AHRS(frc::I2C::Port::kMXP);
	} catch (std::exception& ex) {
		std::string what_string = ex.what();
		std::string err_msg("Error instantiating navX MXP: " + what_string);
		const char *p_err_msg = err_msg.c_str();
		DriverStation::ReportError(p_err_msg);

		ahrs_ = nullptr;
	}
	printf("[SensorSubsystem] Ready\n");
}

void SensorSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new SensorStateUpdateCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
