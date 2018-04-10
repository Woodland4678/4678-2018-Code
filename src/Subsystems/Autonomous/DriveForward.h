/*
 * Station1SwitchLeft.h
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

#ifndef DRIVE_FORWARD_H_
#define DRIVE_FORWARD_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class DriveRobotForward : public AutoScenario
{
public:
// LIFECYCLE
	DriveRobotForward();
    virtual ~DriveRobotForward();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    DriveRobotForward(const DriveRobotForward &);		// Hide the copy constructor

// OPERATORS
    DriveRobotForward & operator=(const DriveRobotForward &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculator1_Ptr;
	bool  				     m_calculator1_init;

    AutoScenarioStateEnum    m_currentState;

};

#endif //DRIVE_FORWARD_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
