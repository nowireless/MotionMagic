#include "OI.h"

#include <WPILib.h>

OI::OI() {
	// Process operator interface input here.
	revDigit_ = new RevDigit();
	revDigit_->Display("3081");

	driver_ = new XboxController(0);
}


XboxController* OI::GetDriver() {
	return driver_;
}

RevDigit* OI::GetRevDigit() {
	return revDigit_;
}
