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
	m_calculator4_Ptr(),
	m_calculator4_init(false),
	m_calculator5_Ptr(),
	m_calculator5_init(false),
	m_calculator6_Ptr(),
	m_calculator6_init(false),
	m_calculator7_Ptr(),
	m_calculator7_init(false),
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

	m_calculator1_Ptr.reset(new DriveMotorCalculator(275, 276, encoder_pulses_cm)); //distance to middle of switch, then we want to s-turn
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(300, 203, encoder_pulses_cm));
	m_calculator2_init = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(-50, -50, encoder_pulses_cm)); //back up
	m_calculator3_init = false;

	m_calculator4_Ptr.reset(new DriveMotorCalculator(50, 50, encoder_pulses_cm)); //will make a PID turn
	m_calculator4_init = false;

	m_calculator5_Ptr.reset(new DriveMotorCalculator(150, 150, encoder_pulses_cm)); //move forward
	m_calculator5_init = false;

	m_calculator6_Ptr.reset(new DriveMotorCalculator(300, 153, encoder_pulses_cm)); //turn into passage
	m_calculator6_init = false;

	m_calculator7_Ptr.reset(new DriveMotorCalculator(350, 350, encoder_pulses_cm)); //traverse passage
	m_calculator7_init = false;

	m_calculator8_Ptr.reset(new DriveMotorCalculator(100, 250, encoder_pulses_cm)); //curve towards scale
	m_calculator8_init = false;

	m_currentState     = ScenarioState1;
	m_stateObserverPtr.reset(new DriveStateObserver);

	m_calculator1_Ptr->setTravelPower(0.7);
	m_calculator1_Ptr->setFinalPower(0.7);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setRampDownPower(0.5);
	m_calculator2_Ptr->setObserver(m_stateObserverPtr);

	m_calculator3_Ptr->setTravelPower(0.7);
	m_calculator3_Ptr->setObserver(m_stateObserverPtr);

	m_calculator5_Ptr->setTravelPower(0.7);
	m_calculator5_Ptr->setFinalPower(0.7);
	m_calculator5_Ptr->setObserver(m_stateObserverPtr);

    m_calculator6_Ptr->removeStartUpZone();
    m_calculator6_Ptr->removeRampUpZone();
    m_calculator6_Ptr->removeRampDownZone();
	m_calculator6_Ptr->setTravelPower(0.7);
	m_calculator6_Ptr->setFinalPower(0.7);

    m_calculator7_Ptr->removeStartUpZone();
	m_calculator7_Ptr->setRampUpDistance(50);
	m_calculator7_Ptr->setRampDownDistance(100);
	m_calculator7_Ptr->setStartUpPower(0.7);

	Robot::manipulatorArm->initMovement();
	m_armMovement = false;
}

void  LeftSideLeftSwitch::execute() {

	switch (m_currentState) {
	case ScenarioState1: //move forward
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
			Robot::manipulatorArm->initMovement();
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		break;

	case ScenarioState2: //turn towards switch
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			m_currentState = ScenarioState3;
			Robot::manipulatorArm->initMovement();
			Robot::manipulatorArm->release();
		}

		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(7);
		break;

	case ScenarioState3: //back up
		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
			m_currentState = ScenarioState4;
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		break;
	case ScenarioState4: //PID turn towards scale (90)
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
			m_currentState = ScenarioState5;
			Robot::driveTrain->shiftUp();
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		break;

	case ScenarioState5: //Go foward a little
		if (moveRobot(m_calculator5_init, m_calculator5_Ptr) == true) {
			m_currentState = ScenarioState6;
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		break;

	case ScenarioState6: //Curve into passage
		if (moveRobot(m_calculator6_init, m_calculator6_Ptr) == true) {
			m_currentState = ScenarioState7;
		}
		break;

	case ScenarioState7: //Traverse through passage
		if (moveRobot(m_calculator7_init, m_calculator7_Ptr) == true) {
			m_currentState = ScenarioState8;
			Robot::manipulatorArm->initMovement();
		}
		break;

	case ScenarioState8: //Curve towards scale
		if (moveRobot(m_calculator8_init, m_calculator8_Ptr) == true) {
			//m_currentState = ScenarioState9;
			setFinished();
		}
		if (!m_armMovement)
				m_armMovement = Robot::manipulatorArm->moveTo(1);
		break;

	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
