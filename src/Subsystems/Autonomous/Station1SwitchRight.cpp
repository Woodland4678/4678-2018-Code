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

	m_calculator1_Ptr.reset(new DriveMotorCalculator(165, 100, encoder_pulses_cm));
	m_calculator1_init = false;

	m_calculator2_Ptr.reset(new DriveMotorCalculator(180, 235, encoder_pulses_cm)); //was 225
	m_calculator2_init = false;

	m_calculator3_Ptr.reset(new DriveMotorCalculator(-100, -200, encoder_pulses_cm));
	m_calculator3_init = false;

	m_calculator4_Ptr.reset(new DriveMotorCalculator(-210, -100, encoder_pulses_cm));
	m_calculator4_init = false;

	m_calculator5_Ptr.reset(new DriveMotorCalculator(100, 100, encoder_pulses_cm));
	m_calculator5_init = false;

	m_calculator6_Ptr.reset(new DriveMotorCalculator(290, 171, encoder_pulses_cm));
	m_calculator6_init = false;

	m_calculator7_Ptr.reset(new DriveMotorCalculator(150, 150, encoder_pulses_cm));
	m_calculator7_init = false;

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

	m_calculator3_Ptr->setTravelPower(0.9);
	m_calculator3_Ptr->setFinalPower(0.7);

   	m_calculator4_Ptr->removeStartUpZone();
   	m_calculator4_Ptr->removeRampUpZone();
	m_calculator4_Ptr->setTravelPower(0.7);
//	m_calculator4_Ptr->setRampDownPower(0.5);
	m_calculator4_Ptr->setObserver(m_stateObserverPtr);

	m_calculator5_Ptr->setTravelPower(0.9);
	m_calculator5_Ptr->setFinalPower(0.7);

   	m_calculator6_Ptr->removeStartUpZone();
   	m_calculator6_Ptr->removeRampUpZone();
	m_calculator6_Ptr->setTravelPower(0.7);
//	m_calculator6_Ptr->setRampDownPower(0.5);
//	m_calculator6_Ptr->setObserver(m_stateObserverPtr);

	m_calculator7_Ptr->setObserver(m_stateObserverPtr);

	m_cnt = 0;

	m_armMovement = false;
	m_armMovement2 = false;
	m_intakeMovement = false;
	if (Robot::ms_ourScaleSide == 'R' || Robot::ms_ourScaleSide == 'r') {
		scaleRight = true;
	} else {
		scaleRight = false;
	}
}

void  Station1SwitchRight::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculator1_init, m_calculator1_Ptr) == true) {
			m_currentState = ScenarioState2;

		}
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(7);
		}
		break;

	case ScenarioState2:
		if (!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(7);
		}
		if (moveRobot(m_calculator2_init, m_calculator2_Ptr) == true) {
			m_cnt++;
			if (m_cnt> 15) {
				m_currentState = ScenarioState3;
				m_cnt = 0;
			}
			Robot::manipulatorArm->release();
			m_armMovement = false;
			Robot::driveTrain->shiftUp();
//			setFinished();
		}
		break;
	case ScenarioState3:
		if (moveRobot(m_calculator3_init, m_calculator3_Ptr) == true) {
			m_currentState = ScenarioState4;

		}
		if (m_calculator3_Ptr->getPercentDone() > 0.2) {
			if(!m_armMovement) {
				m_armMovement = Robot::manipulatorArm->moveTo(11);
			}
		}
		if (!m_intakeMovement) {
			m_intakeMovement = Robot::intake->moveTo(1);
		}
		break;
	case ScenarioState4:
		if(!m_armMovement) {
			m_armMovement = Robot::manipulatorArm->moveTo(11);
		}
		if (moveRobot(m_calculator4_init, m_calculator4_Ptr) == true) {
			m_currentState = ScenarioState5;
		}
		break;
	case ScenarioState5: {
		if (!m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(5);
		}
		if (!m_intakeMovement) {
			m_intakeMovement = Robot::intake->moveTo(1);
		}
		const int  retval(Robot::lidar->findCubes(-35.0, 35.0));


		if (retval == 1) {
			Robot::intake->grab();
			m_cnt = 0;
			m_currentState = ScenarioState6;
			m_intakeMovement = false;
			}
		else if (retval == 2) {
//				m_currentState = ScenarioState4;
			setFinished();
		}
		break;
	}
	case ScenarioState6:
		if (!m_armMovement2) {
			m_armMovement2 = Robot::manipulatorArm->moveTo(5);
		} else {
			m_currentState = ScenarioState7;
		}
		break;
	case ScenarioState7:
		if (Robot::manipulatorArm->pickUpCube()) {
			m_currentState = ScenarioState8;
		}
		break;
	case ScenarioState8:
		if (!m_intakeMovement) {
			m_intakeMovement = Robot::intake->moveTo(0);
		} else {
			if (scaleRight) { //if scale is on the right we want to turn positive 90
				if (Robot::driveTrain->GyroTurn(Robot::ahrs->GetAngle(), 90, 0.10, 0, 0.2)) {
					m_currentState = ScenarioState9;
					Robot::driveTrain->shiftUp();
					m_calculator6_Ptr.reset(new DriveMotorCalculator(171, 290, getEncoderPulsesPerCm()));
					setFinished();//Added to stop robot collisions
				}
			} else { //if scale is on the left we want to turn -90
				if (Robot::driveTrain->GyroTurn(Robot::ahrs->GetAngle(), -90, 0.10, 0, 0.2)) {
					m_currentState = ScenarioState9;
					Robot::driveTrain->shiftUp();
					setFinished();//Added to stop robot collisions
				}
			}
		}
		break;
	case ScenarioState9:
		if (moveRobot(m_calculator5_init, m_calculator5_Ptr) == true) {
			m_currentState = ScenarioState10;
		}
		break;
	case ScenarioState10: //I'll have to flip the numbers for movement 6 depending which side of the scale is ours similar to the turn above
		if (moveRobot(m_calculator6_init, m_calculator6_Ptr) == true) {
			m_currentState = ScenarioState11;
//			setFinished();
			Robot::lidar->isSwitchAuto = false;
		}
		break;
	case ScenarioState11:
		if (moveRobot(m_calculator7_init, m_calculator7_Ptr) == true) {
			setFinished();
		}
		break;
	default:
		break;
	}
}
