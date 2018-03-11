/*
 * LeftSideLeftScale.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "Robot.h"
#include "RobotHelpers.h"
#include "LeftSideLeftScale.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

LeftSideLeftScale::LeftSideLeftScale() :
	AutoScenario(),
	m_calculator1_Ptr(),
	m_calculator1_init(false),
	m_calculator2_Ptr(),
	m_calculator2_init(false),
	m_stateObserverPtr(),
	m_currentState(ScenarioStateUnknown),
	m_armMovement1(false),
	m_armMovement2(false),
	m_armInitMovement2(false),
	m_cnt(0)
{ }

LeftSideLeftScale::~LeftSideLeftScale() {
	m_calculator1_Ptr.reset();
	m_calculator2_Ptr.reset();
	m_calculator2_Ptr.reset();
}

// **********************************  METHODS  **********************************************

void  LeftSideLeftScale::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(500, 500, encoder_pulses_cm)); //distance to middle of switch, then we want to s-turn
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(370, 285, encoder_pulses_cm));
	m_calculator2_init = false;

	m_currentState     = ScenarioState1;
	m_stateObserverPtr.reset(new DriveStateObserver);

	m_calculator1_Ptr->setTravelPower(0.9);
	m_calculator1_Ptr->setFinalPower(0.9);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setRampDownPower(0.5);
	m_calculator2_Ptr->setObserver(m_stateObserverPtr);

	m_armMovement1 = false;
	m_armMovement2 = false;
	m_armInitMovement2 = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(-50, -100, encoder_pulses_cm));
	m_calculator3_init = false;

}

void  LeftSideLeftScale::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (m_calculator1_init == false) {
			Robot::driveTrain->shiftUp();
			Robot::manipulatorArm->initMovement();
		}

		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
//			Robot::manipulatorArm->initMovement();
		}

		if (!m_armMovement1) {
			m_armMovement1 = Robot::manipulatorArm->moveTo(11);
		}
		else if (m_armInitMovement2 == false) {
			m_armInitMovement2 = true;
			Robot::manipulatorArm->initMovement();
		}

		if (m_calculator1_Ptr->getPerentDone() >= 0.50 && m_armInitMovement2 == false) {
			m_armMovement1 = true;
			m_armInitMovement2 = true;
			Robot::manipulatorArm->initMovement();
		}

		if (m_armInitMovement2 == true && !m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(1);
		}

		break;

	case ScenarioState2:
		if (m_armInitMovement2 == false) {
			m_armInitMovement2 = true;
			Robot::manipulatorArm->initMovement();
		}

		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			Robot::manipulatorArm->release();

			++m_cnt;
			if (m_cnt == 25) {
				m_currentState = ScenarioState3;
			}
		}

		if (!m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(1);
		}
		break;

	case ScenarioState3:

		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
//			m_currentState = ScenarioState3;
			setFinished();
		}
		break;
	case ScenarioState4: //find cube stuff

//		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
////			m_currentState = ScenarioState3;
//			setFinished();
//		}
		break;
	case ScenarioState5: //backup stuff

//		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
////			m_currentState = ScenarioState3;
//			setFinished();
//		}
		break;

	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
