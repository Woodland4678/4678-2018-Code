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
#include "PIDController.h"

// MAGIC VALUES
// Change the following values for the PID controller for gryo turning
static const float  s_PropConstant(0.0);
static const float  s_IntegralConstant(0.0);
static const float  s_DerivConstant(0.0);

// ////////////////////////////////////  STATIC ///////////////////////////////////////////////////

static PidControllerPtrType  createController(double setPoint) {

    PidControllerPtrType  controller_ptr(new PidController(setPoint,
                                s_PropConstant, s_IntegralConstant, s_DerivConstant));

    controller_ptr->setLimits(-1.0, 1.0);
    controller_ptr->setTolerance(1.0);

    return controller_ptr;
}

// ////////////////////////////////////  PUBLIC ///////////////////////////////////////////////////

PidControllerPtrType  gryoTurnLeft90() {
	// Turn the robot 90 degrees to the left relative to where it is now
	// Call this function to set up the PID controller
	//const double  starting_angle(Robot::ahrs->GetAngle());
	//const double  ending_angle(starting_angle - 90.0);

    //return createController(ending_angle);
}

PidControllerPtrType  gryoTurnRight90() {
	// Turn the robot 90 degrees to the right relative to where it is now
	// Call this function to set up the PID controller
	//const double  starting_angle(Robot::ahrs->GetAngle());
	//const double  ending_angle(starting_angle + 90.0);

    //return createController(ending_angle);
}


bool  turnRobot(PidControllerPtrType controllerPtr) {
	// Use the PID controller to turn the robot in place
	// Need the current angle to feed into the PID controller
	//const double  current_angle(Robot::ahrs->GetAngle());
	float   motor_power(0.0);

    //const bool  done(controllerPtr->calculateValue(motor_power, current_angle));

	//Robot::driveTrain->SetRightPower(-motor_power);
	//Robot::driveTrain->SetLeftPower(motor_power);

	//return done;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
