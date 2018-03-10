// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.

// SYSTEM INCLUDES
#include <iostream>

#include "AutonomousCommand.h"
#include "Robot.h"
#include "../Subsystems/Autonomous/AutoScenarioHelpers.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

AutonomousCommand::AutonomousCommand() :
    frc::Command(),
	m_autoScenarioPtr()
{
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::driveTrain.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void AutonomousCommand::Initialize() {

	// Get values from console buttons for station location and game objective
	int  auto_side(0);
	int  auto_mode(0);

//	getConsoleValues(auto_side, auto_mode);

	// Get values from FMS for switch side and scale side
	unsigned char  switch_fms('L');
	unsigned char  scale_fms('L');

//	getFmsValues(switch_fms, scale_fms);

//	getInputsFromDriverStation(auto_side, auto_mode, switch_fms, scale_fms);

	// Transform inputs into enums autoScenarioFinder understands
//	StartStationEnum   station(transformConsoleSide(auto_side));
//	AutoObjectiveEnum  objective(transformConsoleMode(auto_mode));
//	SwitchSideEnum     switch_side(transformSwitchFms(switch_fms));
//	ScaleSideEnum      scale_side(transformScaleFms(scale_fms));
//	AutoScenarioEnum   scenario_select(autoScenarioFinder(station, switch_side, scale_side, objective));

	Robot::driveTrain->shiftUp();

	AutoScenarioEnum scenario_select = AutoScenario_Station1_ScaleRight;

//	printf("AutonomousCommand::Initialize(), station=%d, switch_side=%d, scale_side=%d, objective=%d, scenario_select=%d\n",
//			station, switch_side, scale_side, objective, scenario_select);

	// Create the scenario we want to run using a factory function
	m_autoScenarioPtr = autoScenarioFactory(scenario_select);
	if (m_autoScenarioPtr) {
		m_autoScenarioPtr->initialize();
	}
	else {
		printf("AutonomousCommand::Initialize(), scenarioFactory failed\n");
	}
}

// Called repeatedly when this Command is scheduled to run
void AutonomousCommand::Execute() {

	printf("AutonomousCommand::Execute()\n");

	if (m_autoScenarioPtr) {
		m_autoScenarioPtr->execute();
	}
}

// Make this return true when this Command no longer needs to run execute()
bool AutonomousCommand::IsFinished() {
	if (m_autoScenarioPtr) {
		return m_autoScenarioPtr->isFinished();
	}

	return false;
}

// Called once after isFinished returns true
void AutonomousCommand::End() {
	if (m_autoScenarioPtr) {
		m_autoScenarioPtr->end();
	}
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AutonomousCommand::Interrupted() {
	if (m_autoScenarioPtr) {
		m_autoScenarioPtr->interrupted();
	}
}

// /////////////////////////////////  PRIVATE ///////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************


void  AutonomousCommand::getConsoleValues(int &autoSide, int &autoMode) const  {
	//determines which position the switch is in for auto mode
	if (Robot::oi->getAutoSwitch()->GetRawButton(14)) {
		autoSide += 1;
	}
	if (Robot::oi->getAutoSwitch()->GetRawButton(15)) {
		autoSide += 2;
	}
	if (Robot::oi->getAutoSwitch()->GetRawButton(16)) {
		autoSide += 4;
	}

	//determines which position switch is in for auto positioning
	if (Robot::oi->getAutoSwitch()->GetRawButton(13)) {
		autoMode += 1;
	}
	if (Robot::oi->getAutoSwitch()->GetRawButton(12)) {
		autoMode += 2;
	}
	if (Robot::oi->getAutoSwitch()->GetRawButton(11)) {
		autoMode += 4;
	}
	frc::SmartDashboard::PutNumber("Auto Mode", autoMode);
	frc::SmartDashboard::PutNumber("Auto Side", autoSide);
}


void  AutonomousCommand::getFmsValues(unsigned char &switchFms, unsigned char &scaleFms) const  {

    // TODO: add code to get values from fms
	// TODO: need to loop to get values from fms

	switchFms = 'L';
	scaleFms  = 'L';
}


void  AutonomousCommand::getInputsFromDriverStation(int &auto_side, int &auto_mode,
										unsigned char &switch_fms, unsigned char &scale_fms) {

	std::string   inputs_str(frc::DriverStation::GetInstance().GetGameSpecificMessage());

	if (inputs_str.length() > 0) {
		// First char defines the robot starting location - side button on box
		switch (inputs_str[0]) {
		case 1: auto_side = 0;    break;
		case 2: auto_side = 1;    break;
		case 3: auto_side = 2;    break;
		default:  break;
		}

		// Second char defines the game objective - mode button on box
		switch (inputs_str[1]) {
		case 1: auto_mode = 0;    break;
		case 2: auto_mode = 1;    break;
		case 3: auto_mode = 2;    break;
		case 4: auto_mode = 3;    break;
		case 5: auto_mode = 4;    break;
		default:  break;
		}

		// Third char defines the switch ownership - first value from fms
		switch (inputs_str[2]) {
		case 'l':
		case 'L': switch_fms = 'L';    break;
		case 'r':
		case 'R': switch_fms = 'R';    break;
		default:  break;
		}

		// Fourth char defines the scale ownership - second value from fms
		switch (inputs_str[3]) {
		case 'l':
		case 'L': scale_fms = 'L';    break;
		case 'r':
		case 'R': scale_fms = 'R';    break;
		default:  break;
		}

		// Don't care about anything else
	}
}

