/*
 * Station1ScaleRight.h
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

#ifndef STATION_1_SCALE_RIGHT_H_
#define STATION_1_SCALE_RIGHT_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "../DriveTrain/DriveStateObserver.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class Station1ScaleRight : public AutoScenario
{
public:
// LIFECYCLE
	Station1ScaleRight();
    virtual ~Station1ScaleRight();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    Station1ScaleRight(const Station1ScaleRight &);		// Hide the copy constructor

// OPERATORS
    Station1ScaleRight & operator=(const Station1ScaleRight &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculator1_Ptr;
	bool  				     m_calculator1_init;

    DriveCalculatorPtrType   m_calculator2_Ptr;
	bool  				     m_calculator2_init;

    DriveCalculatorPtrType   m_calculator3_Ptr;
	bool  				     m_calculator3_init;

    StateObserverPtrType     m_stateObserverPtr;
    AutoScenarioStateEnum    m_currentState;
};

#endif // STATION_1_SCALE_RIGHT_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
