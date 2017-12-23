#include "OI.h"

#include "Commands/motion/DriveDistanceCommand.h"

#include <SmartDashboard/SmartDashboard.h>
#include <WPILib.h>

OI::OI() {
	// Process operator interface input here.
	revDigit_ = new RevDigit();
	revDigit_->Display("3081");

	driver_ = new XboxController(0);

	SmartDashboard::PutData("MM 3ft", new DriveDistanceCommand(3.0, 10));
}


XboxController* OI::GetDriver() {
	return driver_;
}

RevDigit* OI::GetRevDigit() {
	return revDigit_;
}
