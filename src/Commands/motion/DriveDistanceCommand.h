#ifndef DriveForwardCommand_H
#define DriveForwardCommand_H

#include "CommandBase.h"
#include <Timer.h>

class DriveDistanceCommand : public CommandBase {
public:
	DriveDistanceCommand(double distance, double timeout);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
private:
	double distance_;
	double timeout_;

	bool isLeftDone;
	bool isRightDone;

	Timer timer_;
};

#endif  // DriveForwardCommand_H
