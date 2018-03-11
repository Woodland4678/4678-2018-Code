/*
 * Station1ScaleRight.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "RobotHelpers.h"
#include "Robot.h"
#include "Station1ScaleRight.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

Station1ScaleRight::Station1ScaleRight() :
    AutoScenario(),
	m_calculator1_Ptr(),
	m_calculator1_init(false),
	m_calculator2_Ptr(),
	m_calculator2_init(false),
	m_calculator3_Ptr(),
	m_calculator3_init(false),
	m_calculator4_Ptr(),
	m_calculator4_init(false),
	m_stateObserverPtr(),
	m_currentState(ScenarioStateUnknown)
{ }

Station1ScaleRight::~Station1ScaleRight() {

	m_calculator1_Ptr.reset();
	m_calculator2_Ptr.reset();
}

// **********************************  METHODS  **********************************************

void  Station1ScaleRight::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(500, 500, encoder_pulses_cm));
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(300, 166, encoder_pulses_cm));
	m_calculator2_init = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(400, 400, encoder_pulses_cm));
	m_calculator3_init = false;

	m_calculator4_Ptr.reset(new DriveMotorCalculator(160, 335, encoder_pulses_cm));
	m_calculator4_init = false;

	m_stateObserverPtr.reset(new DriveStateObserver);
	m_currentState     = ScenarioState1;

	m_calculator1_Ptr->setRampUpDistance(50);
	m_calculator1_Ptr->setRampDownDistance(100);
	m_calculator1_Ptr->setFinalPower(0.7);
	m_calculator1_Ptr->setObserver(m_stateObserverPtr);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
    m_calculator2_Ptr->removeRampDownZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setFinalPower(0.7);

    m_calculator3_Ptr->removeStartUpZone();
	m_calculator3_Ptr->setRampUpDistance(50);
	m_calculator3_Ptr->setRampDownDistance(50);
	m_calculator3_Ptr->setStartUpPower(0.7);
	m_calculator3_Ptr->setFinalPower(0.7);
	m_calculator3_Ptr->setObserver(m_stateObserverPtr);

	m_calculator4_Ptr->removeStartUpZone();
	m_calculator4_Ptr->setRampUpDistance(50);
	m_calculator4_Ptr->setRampDownDistance(50);
	m_calculator4_Ptr->setStartUpPower(0.7);
	m_calculator4_Ptr->setObserver(m_stateObserverPtr);

	Robot::manipulatorArm->initMovement();
	m_armMovement = false;
}

void  Station1ScaleRight::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		}
		break;

	case ScenarioState2:
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			m_currentState = ScenarioState3;
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		}
		break;

	case ScenarioState3:
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		}
		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
			m_currentState = ScenarioState4;
			Robot::manipulatorArm->initMovement();
			m_armMovement = false;
//			setFinished();
		}
		if ((m_calculator3_Ptr->getPerentDone() > 0.2) && (m_calculator3_Ptr->getPerentDone() < 0.5)) {
			Robot::driveTrain->shiftUp();
		}
		break;
	case ScenarioState4:
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
//			m_currentState = ScenarioStateMax;
			Robot::manipulatorArm->release();
			setFinished();
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(1);
		}
		break;

	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
