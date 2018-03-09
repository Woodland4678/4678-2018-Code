/*
 * Station1SwitchRight.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "RobotHelpers.h"
#include "Robot.h"
#include "Station1SwitchRight.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

Station1SwitchRight::Station1SwitchRight() :
	AutoScenario(),
	m_calculator1_Ptr(),
	m_calculator1_init(false),
	m_calculator2_Ptr(),
	m_calculator2_init(false),
	m_stateObserverPtr(),
	m_currentState(ScenarioStateUnknown),
	m_armMovement(false)
{ }

Station1SwitchRight::~Station1SwitchRight() {
	m_calculator1_Ptr.reset();
	m_calculator2_Ptr.reset();
	m_calculator2_Ptr.reset();
}

// **********************************  METHODS  **********************************************

void  Station1SwitchRight::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(150, 100, encoder_pulses_cm));
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(180, 225, encoder_pulses_cm));
	m_calculator2_init = false;

	m_currentState     = ScenarioState1;
	m_stateObserverPtr.reset(new DriveStateObserver);
//	m_calculator1_Ptr->setStartUpPower(0.25);
	m_calculator1_Ptr->setTravelPower(0.9);
	m_calculator1_Ptr->setFinalPower(0.7);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
	m_calculator2_Ptr->setTravelPower(0.7);
//	m_calculator2_Ptr->setRampDownPower(0.5);
	m_calculator2_Ptr->setObserver(m_stateObserverPtr);

	Robot::manipulatorArm->initMovement();
	m_armMovement = false;
}

void  Station1SwitchRight::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(7);
		break;

	case ScenarioState2:
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
//			m_currentState = ScenarioState3;
			Robot::manipulatorArm->release();
			setFinished();
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(7);
		break;

	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////