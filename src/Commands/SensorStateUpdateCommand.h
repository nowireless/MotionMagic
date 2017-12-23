#ifndef SensorStateUpdateCommand_H
#define SensorStateUpdateCommand_H

#include "../CommandBase.h"

class SensorStateUpdateCommand : public CommandBase {
public:
	SensorStateUpdateCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // SensorStateUpdateCommand_H
