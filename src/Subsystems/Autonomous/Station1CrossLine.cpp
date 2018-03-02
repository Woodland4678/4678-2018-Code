/*
 * Station1CrossLine.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "RobotHelpers.h"
#include "Station1CrossLine.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

Station1CrossLine::Station1CrossLine() :
    AutoScenario(),
	m_calculatorPtr(),
	m_calculatorInit(false)
{ }

Station1CrossLine::~Station1CrossLine() {
	m_calculatorPtr.reset();
}

// **********************************  METHODS  **********************************************

void  Station1CrossLine::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

    m_calculatorPtr.reset(new DriveMotorCalculator(0, 0, 325, 325, encoder_pulses_cm));

	m_calculatorInit = false;
}

void  Station1CrossLine::execute() {

	if (moveRobot(m_calculatorInit, m_calculatorPtr) == true) {
		setFinished();
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
