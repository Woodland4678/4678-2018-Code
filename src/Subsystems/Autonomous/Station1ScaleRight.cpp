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

	m_calculator1_Ptr.reset(new DriveMotorCalculator(450, 450, encoder_pulses_cm)); //520, 520 Drive straight
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(285, 171, encoder_pulses_cm)); //300, 171 //curve into the switch area
	m_calculator2_init = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(455, 455, encoder_pulses_cm)); //drive straight through the switch area
	m_calculator3_init = false;

	m_calculator4_Ptr.reset(new DriveMotorCalculator(200, 110, encoder_pulses_cm));//first part of the Nat maneuver. The drive forward part
	m_calculator4_init = false;

	m_calculator6_Ptr.reset(new DriveMotorCalculator(-150, -210, encoder_pulses_cm)); //left right the backup part of the nat maneuver to score the cube
	m_calculator6_init = false;

	m_calculator5_Ptr.reset(new DriveMotorCalculator(-200, -148, encoder_pulses_cm)); // The backup movtion after grabbing the second cube
	m_calculator5_init = false;

//	m_calculator4_Ptr.reset(new DriveMotorCalculator(85, 213, encoder_pulses_cm)); //100, 250


	// Do this in the state machine


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

	switch (m_currentState) { //This state will drive forward
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		}
		break;

	case ScenarioState2: //this state will curve into the switch/scale passage
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			m_currentState = ScenarioState3;
			m_armMovement = false;
		} else {
			if (!m_armMovement) {
				m_armMovement = Robot::manipulatorArm->moveTo(11);
			}
		}

		break;

	case ScenarioState3: //this will drive straight through the switch/scale passage
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
				m_armMovement = Robot::manipulatorArm->moveTo(3,0.1,0.3);
			}
		}
		break;
	case ScenarioState4: //This is the first part of the nat maneuver (the forward part)
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
			m_currentState = ScenarioState10; //we go to 10 here because the nat maneuver was sorta pushed in.
			m_cnt = 0;
//			m_cnt++;
//			Robot::manipulatorArm->release();
		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(3,0.1,0.3);
		}
		if (!m_intakeDown) {
				m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
		}
		break;
	case ScenarioState10: //the backup part of the nat manuever
		if (m_calculator6_Ptr->getPercentDone() > 0.9) { //release the cube when the backup is 90% done (this kinda throws it, not sure we need to adjust it at all)
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
			m_armMovement = Robot::manipulatorArm->moveTo(3,0.1,0.3);
		}
		if (!m_intakeDown) {
			m_intakeDown = Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube);
		}
		break;

	case ScenarioState5: //dont think we need to gyro turn on this one, this state is skipped
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

	case ScenarioState6: { //This state looks for the second cube using the lidar
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

		if ((retval != 0)&&(m_armMovement2)) { //used to wait for the arm and intake movement to be done but I took it out, doesn't go into the if until the arm is in position
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
	case ScenarioState7: //This picks up the cube, we know the arm is in position here based on the previous state.
		Robot::manipulatorArm->pickUpCube();
//		m_cnt++;
//		if (m_cnt > 15) {
//			if (!m_armMovement3) {
//				m_armMovement3 = Robot::manipulatorArm->moveTo(5);
//
//			}
		if (!m_intakeMovement2) {
			m_intakeMovement2 = Robot::intake->moveTo(1);
		}
		if ((Robot::manipulatorArm->targetPos == 11)) {
			m_currentState = ScenarioState9;
		}
//			else if ((m_intakeMovement2) && (Robot::manipulatorArm->checkForCube()) && (m_armMovement3)) {
//				m_currentState = ScenarioState8;
//				m_cnt = 0;
//			}
//
//		}
		break;
	case ScenarioState8: //this state is skipped ignore it
//		m_cnt += 1;
//		Robot::manipulatorArm->squeeze();
//		if(m_cnt > 25) {
//			Robot::intake->release();
			m_currentState = ScenarioState9;
//		}
		break;
	case ScenarioState9: //THis state backs up towards the scale to drop the second cube
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
			Robot::manipulatorArm->release(); //only does this once the driving and the arm motion is done
			setFinished();
		}
		if (!m_armMovement4) {
			m_armMovement4 = Robot::manipulatorArm->moveTo(3);
		}
		if (!m_intakeMovement3) {
			Robot::intake->moveTo(1);
		}
	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
