/*
 * Station1SwitchLeft.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "Robot.h"
#include "RobotHelpers.h"
#include "DriveForward.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

DriveRobotForward::DriveRobotForward() :
	AutoScenario(),
	m_calculator1_Ptr(),
	m_calculator1_init(false),
	m_currentState(ScenarioStateUnknown)
{ }

DriveRobotForward::~DriveRobotForward() {
	m_calculator1_Ptr.reset();
}

// **********************************  METHODS  **********************************************

void  DriveRobotForward::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(300, 300, encoder_pulses_cm));
	m_calculator1_init = false;

//	m_calculator2_Ptr.reset(new DriveMotorCalculator(275, 195, encoder_pulses_cm));
//	m_calculator2_init = false;

	m_currentState     = ScenarioState1;

//	m_calculator1_Ptr->setStartUpPower(0.25);
	m_calculator1_Ptr->setTravelPower(0.9);

//    m_calculator2_Ptr->removeStartUpZone();
//    m_calculator2_Ptr->removeRampUpZone();
//	m_calculator2_Ptr->setTravelPower(0.7);
////	m_calculator2_Ptr->setRampDownPower(0.5);
//	m_calculator2_Ptr->setObserver(m_stateObserverPtr);

}

void  DriveRobotForward::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
//			m_currentState = ScenarioState2;
			setFinished();

		}
		break;


	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
