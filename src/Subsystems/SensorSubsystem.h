#ifndef SensorSubsystem_H
#define SensorSubsystem_H

#include <Commands/Subsystem.h>
#include <AHRS.h>

class SensorSubsystem : public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	AHRS* ahrs_;
public:
	SensorSubsystem();
	void InitDefaultCommand();
};

#endif  // SensorSubsystem_H
