/*
 * Station1SwitchLeft.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "RobotHelpers.h"
#include "Station1SwitchLeft.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

Station1SwitchLeft::Station1SwitchLeft() :
    AutoScenario(),
	m_calculatorPtr(),
	m_calculatorInit(false),
	m_currentState(ScenarioStateUnknown)
{ }

Station1SwitchLeft::~Station1SwitchLeft() {
	m_calculatorPtr.reset();
}

// **********************************  METHODS  **********************************************

void  Station1SwitchLeft::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

    m_calculatorPtr.reset(new DriveMotorCalculator(0, 0, 325, 325, encoder_pulses_cm));
	m_calculatorInit = false;

	m_currentState     = ScenarioState1;
}

void  Station1SwitchLeft::execute() {

	switch (m_currentState) {
	case ScenarioState1:
		if (moveRobot(m_calculatorInit, m_calculatorPtr) == true) {
			setFinished();
		}
		break;

	default:
		break;
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
