/*
 * TurnRobot.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
#include <iostream>

// PROJECT INCLUDES
#include "../../Robot.h"
#include "AutoScenarioHelpers.h"

// MAGIC VALUES
// Change the following values for the PID controller for gryo turning
static const float  s_ProportionalConstant(0.0);
static const float  s_IntegralConstant(0.0);
static const float  s_DerivativeConstant(0.0);


// ////////////////////////////////////  PUBLIC ///////////////////////////////////////////////////

void  gryoTurnLeft() {
	// Turn the robot 90 degrees to the left relative to where it is now
	// Call this function to set up the PID controller
//	const double  starting_angle(Robot::ahrs->GetAngle());
//	const double  ending_angle(starting_angle - 90.0);



}

void  gryoTurnRight() {
	// Turn the robot 90 degrees to the right relative to where it is now
	// Call this function to set up the PID controller
//	const double  starting_angle(Robot::ahrs->GetAngle());
//	const double  ending_angle(starting_angle + 90.0);



}


bool  turnRobot(DriveCalculatorPtrType calculatorPtr) {
	// Use the PID controller to turn the robot in place
	// Need the current angle to feed into the PID controller

//	const double  current_angle(Robot::ahrs->GetAngle());
	float   motor_power(0.0);

    const bool  done(false);

	Robot::driveTrain->SetRightPower(-motor_power);
	Robot::driveTrain->SetLeftPower(motor_power);

	return done;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
