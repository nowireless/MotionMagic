#ifndef OI_H
#define OI_H

#include <XboxController.h>
#include <Util/oi/RevDigit.h>

class OI {
public:
	OI();

	XboxController* GetDriver();
	RevDigit* GetRevDigit();
private:
	XboxController* driver_;
	RevDigit* revDigit_;
};

#endif  // OI_H
