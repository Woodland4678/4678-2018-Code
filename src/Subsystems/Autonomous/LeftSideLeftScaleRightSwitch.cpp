/*
 * LeftSideLeftScaleRightSwitch.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "Robot.h"
#include "RobotHelpers.h"
#include "LeftSideLeftScaleRightSwitch.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

LeftSideLeftScaleRightSwitch::LeftSideLeftScaleRightSwitch() :
	AutoScenario(),
	m_calculator1_Ptr(),
	m_calculator1_init(false),
	m_calculator2_Ptr(),
	m_calculator2_init(false),
	m_calculator3_Ptr(),
	m_calculator3_init(false),
	m_calculator4_Ptr(),
	m_calculator4_init(false),
	m_calculator6_Ptr(),
	m_calculator6_init(false),
	m_calculator7_Ptr(),
	m_calculator7_init(false),
	m_stateObserverPtr(),
	m_currentState(ScenarioStateUnknown),
	m_armMovement(false)
{ }

LeftSideLeftScaleRightSwitch::~LeftSideLeftScaleRightSwitch() {
	m_calculator1_Ptr.reset();
	m_calculator2_Ptr.reset();
	m_calculator2_Ptr.reset();
}

// **********************************  METHODS  **********************************************

void  LeftSideLeftScaleRightSwitch::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(316, 316, encoder_pulses_cm)); //distance to middle of switch, then we want to s-turn
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(355, 305, encoder_pulses_cm));
	m_calculator2_init = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(175, 227, encoder_pulses_cm));
	m_calculator3_init = false;

	m_calculator4_Ptr.reset(new DriveMotorCalculator(-100, -100, encoder_pulses_cm)); //back up
	m_calculator4_init = false;

	m_calculator6_Ptr.reset(new DriveMotorCalculator(300, 300, encoder_pulses_cm)); //traverse the passage
	m_calculator6_init = false;

	m_currentState     = ScenarioState1;
	m_stateObserverPtr.reset(new DriveStateObserver);

	m_calculator1_Ptr->setTravelPower(0.7);
	m_calculator1_Ptr->setFinalPower(0.7);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setRampDownPower(0.5);

    m_calculator3_Ptr->removeStartUpZone();
	m_calculator3_Ptr->removeRampUpZone();
	m_calculator3_Ptr->setTravelPower(0.7);
	m_calculator3_Ptr->setObserver(m_stateObserverPtr);

	m_calculator4_Ptr->setTravelPower(0.7); //back up
	m_calculator4_Ptr->setFinalPower(0.7);

	m_calculator6_Ptr->setTravelPower(0.7);
	m_calculator6_Ptr->setFinalPower(0.7);

	Robot::manipulatorArm->initMovement();
	m_armMovement = false;
}

void  LeftSideLeftScaleRightSwitch::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
			Robot::manipulatorArm->initMovement();
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		break;

	case ScenarioState2:
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			m_currentState = ScenarioState3;
		}

		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(1);
		break;

	case ScenarioState3:
		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
			m_currentState = ScenarioState4;
			Robot::manipulatorArm->release();
//			setFinished();
		}
		if (!m_armMovement)
			m_armMovement = Robot::manipulatorArm->moveTo(1);
		break;

	case ScenarioState4:
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
			m_currentState = ScenarioState5;
			Robot::manipulatorArm->release();
//			setFinished();
		}
		break;
	case ScenarioState5:
		//if (Robot::driveTrain->GyroTurn(current, turnAmount, p, i, d) == true) {
			m_currentState = ScenarioState6;
		//}
		break;
	case ScenarioState6:
		if (moveRobot(m_calculator6_init, m_calculator6_Ptr) == true) {
			m_currentState = ScenarioState7;
		}
		break;
	case ScenarioState7:
		//if (Robot::driveTrain->GyroTurn(current, turnAmount, p, i, d) == true) {
			m_currentState = ScenarioState8;
		//}
		break;

	case ScenarioState8:
		//Will need LIDAR for this step
		break;
	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
