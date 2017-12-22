/*
 * AutoSelection.h
 *
 *  Created on: Feb 21, 2017
 *      Author: Ryan
 */

#ifndef AUTOSELECTION_H_
#define AUTOSELECTION_H_

#include <Commands/Command.h>
#include <SmartDashboard/SendableChooser.h>
#include <Util/oi/RevDigit.h>

#include <mutex>

class AutoSelection {
public:
	enum Auto {
		DefaultAuto,
		/* Put additional auto modes here*/
		LastAuto // Not an auto, but a marker
	};
private:
	AutoSelection(RevDigit* revDigit);
	void Init();
	void UpdateSmartDashboard(Auto selected);

	Auto selectedAuto;
	std::mutex selectedAutoMutex;
	RevDigit* revDigit;

	std::thread t;
	static void WorkerThread(AutoSelection* i);

	static AutoSelection* instance;
public:
	virtual ~AutoSelection();
	frc::Command* GetSelected();
	void NextAuto();
	void PreviousAuto();
	Auto CurrentAuto();

	static void Initialize(RevDigit* revDigit);
	static AutoSelection* GetInstance();
};

#endif /* AUTOSELECTION_H_ */
