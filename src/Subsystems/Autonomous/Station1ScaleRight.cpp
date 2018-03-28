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
	m_cnt = 0;
	int  encoder_pulses_cm(getEncoderPulsesPerCm());

	m_calculator1_Ptr.reset(new DriveMotorCalculator(450, 450, encoder_pulses_cm)); //520, 520
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(280, 171, encoder_pulses_cm)); //300, 171
	m_calculator2_init = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(440, 440, encoder_pulses_cm));
	m_calculator3_init = false;

	m_calculator6_Ptr.reset(new DriveMotorCalculator(-160, -210, encoder_pulses_cm));
	m_calculator6_init = false;

//	m_calculator4_Ptr.reset(new DriveMotorCalculator(85, 213, encoder_pulses_cm)); //100, 250
	m_calculator4_Ptr.reset(new DriveMotorCalculator(200, 110, encoder_pulses_cm));
	m_calculator4_init = false;

	// Do this in the state machine
	m_calculator5_Ptr.reset(new DriveMotorCalculator(-200, -148, encoder_pulses_cm));
	m_calculator5_init = false;

	m_stateObserverPtr.reset(new DriveStateObserver);
	m_currentState     = ScenarioState1;

	m_calculator1_Ptr->setRampUpDistance(50);
	m_calculator1_Ptr->setRampDownDistance(100);
	m_calculator1_Ptr->setFinalPower(0.95);
	m_calculator1_Ptr->setTravelPower(0.95);
//	m_calculator1_Ptr->setObserver(m_stateObserverPtr);

    m_calculator2_Ptr->removeStartUpZone();
    m_calculator2_Ptr->removeRampUpZone();
    m_calculator2_Ptr->removeRampDownZone();
	m_calculator2_Ptr->setTravelPower(0.7);
	m_calculator2_Ptr->setFinalPower(0.7);

    m_calculator3_Ptr->removeStartUpZone();
	m_calculator3_Ptr->setRampUpDistance(50);
	m_calculator3_Ptr->setRampDownDistance(50);
	m_calculator3_Ptr->setStartUpPower(0.95);
	m_calculator3_Ptr->setFinalPower(0.95);
	m_calculator3_Ptr->setObserver(m_stateObserverPtr);

	m_calculator4_Ptr->removeStartUpZone();
	m_calculator4_Ptr->setRampUpDistance(50);
	m_calculator4_Ptr->setRampDownDistance(50);
	m_calculator4_Ptr->setStartUpPower(0.9);
//	m_calculator4_Ptr->setObserver(m_stateObserverPtr);

	m_armMovement = false;
	m_armMovement2 = false;
	m_armMovement3 = false;
	m_armMovement4 = false;
	m_armMovement5 = false;

	m_intakeDown = false;
	m_intakeMovement2 = false;
	m_intakeMovement3 = false;
}

int retval;

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
			m_armMovement = false;
		} else {
			if (!m_armMovement) {
				m_armMovement = Robot::manipulatorArm->moveTo(11);
			}
		}

		break;

	case ScenarioState3:
//		if (!m_armMovement) {
//			m_armMovement = Robot::manipulatorArm->moveTo(11);
//		}
		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
			m_currentState = ScenarioState4;
			m_cnt = 0;
			m_armMovement = false;
		}
		if ((m_calculator3_Ptr->getPercentDone() > 0.15) && (m_calculator3_Ptr->getPercentDone() < 0.5)) {
			Robot::driveTrain->shiftUp();
		}
		if (((m_calculator3_Ptr->getPercentDone() > 0.5))) {
			if (!m_armMovement) {
				m_armMovement = Robot::manipulatorArm->moveTo(3);
			}
		}
		break;
	case ScenarioState4:
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
			m_currentState = ScenarioState10;
			m_cnt = 0;
//			m_cnt++;
//			Robot::manipulatorArm->release();
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(3);
		}

		break;
	case ScenarioState10:
		if (m_calculator6_Ptr->getPercentDone() > 0.9) {
			Robot::manipulatorArm->release();
		}
		if (moveRobot(m_calculator6_init, m_calculator6_Ptr) == true) {
			Robot::manipulatorArm->release();
			if (m_cnt > 5) {
				m_currentState = ScenarioState6;
			}
			m_cnt++;
//			Robot::manipulatorArm->release();
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(3);
		}
		if (!m_intakeDown) {
			m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
		}
		break;

	case ScenarioState5: //dont think we need to gyro turn on this one
//		if (Robot::driveTrain->GyroTurn(Robot::ahrs->GetAngle(), 180, 0.15, 0, 0.2)) {
//			m_currentState = ScenarioState6;
//			m_cnt = 0;
//			retval = 0;
//		}
//		//if (abs(Robot::driveTrain->getGyroTurnError() < 80)) {
//			if (!m_armMovement2) {
//				m_armMovement2 = Robot::manipulatorArm->moveTo(11);
//			}
//		//}
//		if (!m_intakeDown) {
//			m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
//		}
		break;

	case ScenarioState6: {
		// Find a cubes
		if (!m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(5);
		}
//		if (m_cnt > 25 && Robot::driveTrain->getLeftSpeed() < 100) {
//			Robot::intake->grab();
//			m_currentState = ScenarioState7;
//			m_cnt = 0;
//		}
		if (!m_intakeDown) {
			m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
		}

		if (retval == 0) {
			retval = Robot::lidar->findCubes(-40.0, 40.0);
		}

		if ((retval != 0)&&(m_armMovement2)&&(m_intakeDown)) {
			if (retval == 1) {
				Robot::intake->grab();
				m_cnt = 0;
				m_currentState = ScenarioState7;
				m_armMovement3 = false;
				}
			else if (retval == 2) {
//				m_currentState = ScenarioState4;
				setFinished();
			}
		}
		m_cnt++;

		break;
	}
	case ScenarioState7:
		m_cnt++;
		if (m_cnt > 15) {
			if (!m_armMovement3) {
				m_armMovement3 = Robot::manipulatorArm->moveTo(5);

			}
			if (!m_intakeMovement2) {
				m_intakeMovement2 = Robot::intake->moveTo(2);
			}
			else if ((m_intakeMovement2) && (Robot::manipulatorArm->checkForCube()) && (m_armMovement3)) {
				m_currentState = ScenarioState8;
				m_cnt = 0;
			}

		}
		break;
	case ScenarioState8:
		m_cnt += 1;
		Robot::manipulatorArm->squeeze();
		if(m_cnt > 5) {
			Robot::intake->release();
			m_currentState = ScenarioState9;
		}
		break;
	case ScenarioState9:
//		if (m_calculator5_init == false) {
//			int  encoder_pulses_cm(getEncoderPulsesPerCm());
//
//			// Get the distance the robot travelled to get the cube
////			int   left_cm(0);
////			int   right_cm(0);
//
////			Robot::lidar->getDistanceToCube(left_cm, right_cm);
//
//			// Negate the distance so the robot can travel backwards
////			left_cm *= -1;
////			right_cm *= -1;
//
//		}
		if ((moveRobot(m_calculator5_init, m_calculator5_Ptr) == true) && (m_armMovement4)) {
			m_currentState = ScenarioState2;
			Robot::manipulatorArm->release();
			setFinished();
		}
		if (!m_armMovement4) {
			m_armMovement4 = Robot::manipulatorArm->moveTo(3);
		}
		if (!m_intakeMovement3) {
			Robot::intake->moveTo(0);
		}
	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
