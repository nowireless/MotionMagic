#ifndef DriveWithControllerCommand_H
#define DriveWithControllerCommand_H

#include "../CommandBase.h"
#include <Util/drive/CheesyDriveHelper.h>

class DriveWithControllerCommand : public CommandBase {
public:
	DriveWithControllerCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // DriveWithControllerCommand_H
