/*
 * Station3CrossLine.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"
#include "RobotHelpers.h"
#include "Station3CrossLine.h"		// class declaration

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

Station3CrossLine::Station3CrossLine() :
	AutoScenario(),
	m_calculatorPtr(),
	m_calculatorInit(false)
{ }

Station3CrossLine::~Station3CrossLine() {
	m_calculatorPtr.reset();
}

// **********************************  METHODS  **********************************************

void  Station3CrossLine::initialize() {

	int  encoder_pulses_cm(getEncoderPulsesPerCm());

    m_calculatorPtr.reset(new DriveMotorCalculator(425, 425, encoder_pulses_cm));

	m_calculatorInit = false;
}

void  Station3CrossLine::execute() {

	if (moveRobot(m_calculatorInit, m_calculatorPtr) == true) {
		setFinished();
	}
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
