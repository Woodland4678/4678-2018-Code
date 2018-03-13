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
	m_calculator3_Ptr(),
	m_calculator3_init(false),
	m_calculator4_Ptr(),
	m_calculator4_init(false),
	m_calculator5_Ptr(),
	m_calculator5_init(false),
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

	m_currentState     = ScenarioState1;
	m_stateObserverPtr.reset(new DriveStateObserver);

	m_calculator1_Ptr.reset(new DriveMotorCalculator(500, 500, encoder_pulses_cm)); //distance to middle of switch, then we want to s-turn
	m_calculator1_init = false;

	m_calculator1_Ptr->setTravelPower(0.9);
	m_calculator1_Ptr->setFinalPower(0.9);

	m_calculator2_Ptr.reset(new DriveMotorCalculator(380, 285, encoder_pulses_cm));
	m_calculator2_init = false;

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setRampDownPower(0.5);
	m_calculator2_Ptr->setObserver(m_stateObserverPtr);

	m_calculator3_Ptr.reset(new DriveMotorCalculator(-75, -125, encoder_pulses_cm));
	m_calculator3_init = false;

	m_calculator4_Ptr.reset(new DriveMotorCalculator(225, 100, encoder_pulses_cm));
	m_calculator4_init = false;

	m_calculator5_Ptr.reset(new DriveMotorCalculator(-50, -50, encoder_pulses_cm));
	m_calculator5_init = false;

	m_armMovement1 = false;
	m_armMovement2 = false;
	m_armInitMovement2 = false;
	m_armMovement3 = false;

	m_armMovement4 = false;

	m_intakeDown = false;
}

void  LeftSideLeftScale::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (m_calculator1_init == false) {
			Robot::driveTrain->shiftUp();
		}

		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
		}

		if (!m_armMovement1) {
			m_armMovement1 = Robot::manipulatorArm->moveTo(11);
		}
		else if (m_armInitMovement2 == false) {
			m_armInitMovement2 = true;
		}

		if (m_calculator1_Ptr->getPerentDone() >= 0.50 && m_armInitMovement2 == false) {
			m_armMovement1 = true;
			m_armInitMovement2 = true;
		}

		if (m_armInitMovement2 == true && !m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(1);
		}

		break;

	case ScenarioState2:
		if (m_armInitMovement2 == false) {
			m_armInitMovement2 = true;
		}

		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			Robot::manipulatorArm->release();

			++m_cnt;
			if (m_cnt == 25 && m_armMovement2) {
				m_currentState = ScenarioState3;
			}
		}

		if (!m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(1);
		}
		break;

	case ScenarioState3:
		// Back robot up and turn slightly
		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
			m_currentState = ScenarioState4;
//			setFinished();
		}
//		if (!m_intakeDown) {
//			m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
//		}
		break;

	case ScenarioState4:
		// Come ahead so can find a cube
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
			if(m_armMovement3){
				m_currentState = ScenarioState5;
				m_cnt = 0;
			}
//			setFinished();
		}
		if (!m_armMovement3) {
			m_armMovement3 = Robot::manipulatorArm->moveTo(11);
		}
//		if (!m_intakeDown) {
//			m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
//		}
		break;

	case ScenarioState5:
	{
		// Find a cubes
		if (!m_armMovement3) {
			m_armMovement3 = Robot::manipulatorArm->moveTo(11);
		}
		if (m_cnt > 25 && Robot::driveTrain->getLeftSpeed() < 100) {
			Robot::intake->grab();
			m_currentState = ScenarioState6;
		}
		const int  retval(Robot::lidar->findCubes());

		if (retval == 1) {
			Robot::intake->grab();
			m_currentState = ScenarioState6;
		}
		else if (retval == 2) {
//			m_currentState = ScenarioState4;
			setFinished();
		}
		m_cnt++;
		break;
	}

	case ScenarioState6:
		// Come ahead so can find a cube
		if (!m_armMovement3) { //should be movement4 and moveTo5
			m_armMovement3 = Robot::manipulatorArm->moveTo(11);
		}
		if (moveRobot(m_calculator5_init, m_calculator5_Ptr) == true) {
			m_currentState = ScenarioState7;
//			setFinished();
		}
		break;
	case ScenarioState7:
		if (!m_armMovement3) { //should be movement4 and moveTo5
			m_armMovement3 = Robot::manipulatorArm->moveTo(11);
		} else {
			m_currentState = ScenarioState8;
			m_cnt = 0;
		}
		break;
	case ScenarioState8:
		m_cnt += 1;
		Robot::manipulatorArm->squeeze();
		if(m_cnt > 5) {
			Robot::intake->release();
		}
		break;
	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
