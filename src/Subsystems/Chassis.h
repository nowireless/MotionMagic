#ifndef Chassis_H
#define Chassis_H

#include <Commands/Subsystem.h>
#include <ctrlib/CANTalon.h>



class Chassis : public Subsystem {
public:
	enum Mode {
		kOpenLoop,
		kVelocity,
		kMotionMagic
	};

	struct ClosedLoopParams {
		int profileSolt;
		double f;
		double p;
		double i;
		double d;

		ClosedLoopParams() : profileSolt(0),
				f(0), p(0), i(0), d(0) {}

		ClosedLoopParams(int slot, double f, double p, double i, double d)
		: profileSolt(slot), f(f), p(p), i(i), d(d) {}
	};

	Chassis();
	void InitDefaultCommand();

	void SetBreak(bool state);
	void SetMotionMagicParamsParams(double cruiseVel, double accel);
	void ZeroEncoders();

	void Stop();
	void SetOpenLoop(double left, double right);
	void SetVelocity(double leftVel, double rightVel);
	void SetPrecentVelocity(double left, double right);
	void SetMotionMagicSetPoints(double left, double right);
	void SetMotionMagicSetPointsFeet(double left, double right);

	bool IsLeftBusy();
	bool IsRightBusy();
	double GetLeftDistance();
	double GetRightDistance();
	double GetLeftRate();
	double GetRightRate();
	double GetLeftRealOutput();
	double GetRightRealOutput();

	int GetLeftClosedLoopError();
	int GetRightClosedLoopError();

private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	CANTalon* leftMaster_;
	CANTalon* leftSlave_;
	CANTalon* rightMaster_;
	CANTalon* rightSlave_;

	Mode mode_;

	void SetClosedLoopParams(CANTalon* talon, ClosedLoopParams params);
	void SetTalonControlMode(CANTalon::TalonControlMode mode);
};

#endif  // Chassis_H
