/*
 * DriveStateObserver.cpp
 *
 *  Created on: Feb 18, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
#include <iostream>

// PROJECT INCLUDES
#include "DriveStateObserver.h"
#include "Robot.h"

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

DriveStateObserver::DriveStateObserver()
{ }

DriveStateObserver::~DriveStateObserver() {
}

// **********************************  METHODS  **********************************************

void  DriveStateObserver::startStartUpState() {
//    Robot::driveTrain->shiftDown();

    std::cout << "DriveStateObserver::startStartUpState() called" << std::endl;
}

void  DriveStateObserver::startRampUpState() {

    std::cout << "DriveStateObserver::startRampUpState() called" << std::endl;
}

void  DriveStateObserver::startTravelState() {
    Robot::driveTrain->shiftUp();

    std::cout << "DriveStateObserver::startTravelState() called" << std::endl;
}

void  DriveStateObserver::startRampDownState() {
    Robot::driveTrain->shiftDown();

    std::cout << "DriveStateObserver::startRampDownState() called" << std::endl;
}

void  DriveStateObserver::startFinishState() {

    std::cout << "DriveStateObserver::startFinishState() called" << std::endl;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
