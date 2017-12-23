#include "CommandBase.h"

#include <Commands/Scheduler.h>


// Initialize a single static instance of all of your subsystems. The following
// line should be repeated for each subsystem in the project.
std::unique_ptr<Chassis> CommandBase::chassis;
std::unique_ptr<SensorSubsystem> CommandBase::sensors;
std::unique_ptr<OI> CommandBase::oi;

CommandBase::CommandBase(const std::string &name) :
		frc::Command(name) {

}

void CommandBase::InitSubsystems() {
	chassis = std::make_unique<Chassis>();
	sensors = std::make_unique<SensorSubsystem>();
	oi = std::make_unique<OI>();
}

