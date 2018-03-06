/*
 * LeftSideLeftSwitch.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "Robot.h"
#include "RobotHelpers.h"
#include "LeftSideLeftSwitch.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

LeftSideLeftSwitch::LeftSideLeftSwitch() :
	AutoScenario(),
	m_calculator1_Ptr(),
	m_calculator1_init(false),
	m_calculator2_Ptr(),
	m_calculator2_init(false),
	m_calculator3_Ptr(),
	m_calculator3_init(false),
	m_stateObserverPtr(),
	m_currentState(ScenarioStateUnknown),
	m_armMovement(false)
{ }

LeftSideLeftSwitch::~LeftSideLeftSwitch() {
	m_calculator1_Ptr.reset();
	m_calculator2_Ptr.reset();
	m_calculator2_Ptr.reset();
}

// **********************************  METHODS  **********************************************

void  LeftSideLeftSwitch::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(0, 0, 275, 276, encoder_pulses_cm)); //distance to middle of switch, then we want to s-turn
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(0, 0, 300, 153, encoder_pulses_cm));
	m_calculator2_init = false;

	//m_calculator3_Ptr.reset(new DriveMotorCalculator(0, 0, 175, 227, encoder_pulses_cm));
	//m_calculator3_init = false;

	m_currentState     = ScenarioState1;
	m_stateObserverPtr.reset(new DriveStateObserver);

	m_calculator1_Ptr->setTravelPower(0.7);
	m_calculator1_Ptr->setFinalPower(0.7);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setRampDownPower(0.5);
	m_calculator2_Ptr->setObserver(m_stateObserverPtr);

//    m_calculator3_Ptr->removeStartUpZone();
//	m_calculator3_Ptr->removeRampUpZone();
//	m_calculator3_Ptr->setTravelPower(0.7);
//	m_calculator3_Ptr->setObserver(m_stateObserverPtr);

	//Robot::manipulatorArm->initMovement();
	//m_armMovement = false;
}

void  LeftSideLeftSwitch::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
			//Robot::manipulatorArm->initMovement();
		}
//		if (!m_armMovement)
//			m_armMovement = Robot::manipulatorArm->moveTo(11);
		break;

	case ScenarioState2:
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
//			m_currentState = ScenarioState3;
			setFinished();
		}

//		if (!m_armMovement)
//			m_armMovement = Robot::manipulatorArm->moveTo(1);
		break;

//	case ScenarioState3:
//		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
////			m_currentState = ScenarioState4;
//			Robot::manipulatorArm->release();
//			setFinished();
//		}
//		if (!m_armMovement)
//			m_armMovement = Robot::manipulatorArm->moveTo(1);
//		break;

	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
