/*
 * LeftSideLeftSwitch.h
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

#ifndef LEFT_SIDE_LEFT_SWITCH_H_
#define LEFT_SIDE_LEFT_SWITCH_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class LeftSideLeftSwitch : public AutoScenario
{
public:
// LIFECYCLE
	LeftSideLeftSwitch();
    virtual ~LeftSideLeftSwitch();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    LeftSideLeftSwitch(const LeftSideLeftSwitch &);		// Hide the copy constructor

// OPERATORS
    LeftSideLeftSwitch & operator=(const LeftSideLeftSwitch &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculator1_Ptr;
	bool  				     m_calculator1_init;

    DriveCalculatorPtrType   m_calculator2_Ptr;
	bool  				     m_calculator2_init;

    DriveCalculatorPtrType   m_calculator3_Ptr;
	bool  				     m_calculator3_init;

	DriveCalculatorPtrType   m_calculator4_Ptr;
	bool  				     m_calculator4_init;

	DriveCalculatorPtrType   m_calculator5_Ptr;
	bool  				     m_calculator5_init;

	DriveCalculatorPtrType   m_calculator6_Ptr;
	bool  				     m_calculator6_init;

	DriveCalculatorPtrType   m_calculator7_Ptr;
	bool  				     m_calculator7_init;

	DriveCalculatorPtrType   m_calculator8_Ptr;
	bool  				     m_calculator8_init;


    StateObserverPtrType     m_stateObserverPtr;
    AutoScenarioStateEnum    m_currentState;
    bool					 m_armMovement;
};

#endif // LEFT_SIDE_LEFT_SCALE_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
