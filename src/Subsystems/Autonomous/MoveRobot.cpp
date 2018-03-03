/*
 * MoveRobot.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
//#include <iostream>

// PROJECT INCLUDES
#include "../../Robot.h"
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenarioHelpers.h"

// ////////////////////////////////////  PUBLIC ///////////////////////////////////////////////////

bool  moveRobot(bool &calculatorInit, DriveCalculatorPtrType calculatorPtr) {

	const int   left_encoder(Robot::driveTrain->getLeftEncoder()->Get());
	const int   right_encoder(Robot::driveTrain->getRightEncoder()->Get());

	if (calculatorInit == false) {
		calculatorInit = true;
		calculatorPtr->setStartingEncoders(left_encoder, right_encoder);

//		std::string   dump_str(calculatorPtr->dumpObject());
//		printf("\calculatorPtr->dumpObject()\n%s", dump_str.c_str());
	}

	float left_motor_power;
	float right_motor_power;

	bool  done(calculatorPtr->getMotorSpeeds(left_motor_power, right_motor_power, left_encoder, right_encoder));

//	printf("done=%d, leftPW: %f, leftEncGet: %i, rightPW: %f, rightEnc: %i\n",
//			done, left_motor_power, left_encoder, right_motor_power, right_encoder);

	Robot::driveTrain->SetRightPower(right_motor_power);
	Robot::driveTrain->SetLeftPower(left_motor_power);

	return done;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
