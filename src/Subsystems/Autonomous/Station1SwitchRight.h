/*
 * Station1SwitchRight.h
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

#ifndef STATION_1_SWITCH_RIGHT_H_
#define STATION_1_SWITCH_RIGHT_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class Station1SwitchRight : public AutoScenario
{
public:
// LIFECYCLE
	Station1SwitchRight();
    virtual ~Station1SwitchRight();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    Station1SwitchRight(const Station1SwitchRight &);		// Hide the copy constructor

// OPERATORS
    Station1SwitchRight & operator=(const Station1SwitchRight &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculator1_Ptr;
	bool  				     m_calculator1_init;

    DriveCalculatorPtrType   m_calculator2_Ptr;
	bool  				     m_calculator2_init;

    DriveCalculatorPtrType   m_calculator3_Ptr;
	bool  				     m_calculator3_init;

    StateObserverPtrType     m_stateObserverPtr;
    AutoScenarioStateEnum    m_currentState;
    bool					 m_armMovement;
};

#endif // STATION_1_SWITCH_RIGHT_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
