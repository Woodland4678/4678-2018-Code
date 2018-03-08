/*
 * LeftSideLeftScaleRightSwitch.h
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

#ifndef LEFT_SIDE_LEFT_SCALE_RIGHT_SWITCH_H_
#define LEFT_SIDE_LEFT_SCALE_RIGHT_SWITCH_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class LeftSideLeftScaleRightSwitch : public AutoScenario
{
public:
// LIFECYCLE
	LeftSideLeftScaleRightSwitch();
    virtual ~LeftSideLeftScaleRightSwitch();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    LeftSideLeftScaleRightSwitch(const LeftSideLeftScaleRightSwitch &);		// Hide the copy constructor

// OPERATORS
    LeftSideLeftScaleRightSwitch & operator=(const LeftSideLeftScaleRightSwitch &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculator1_Ptr;
	bool  				     m_calculator1_init;

    DriveCalculatorPtrType   m_calculator2_Ptr;
	bool  				     m_calculator2_init;

    DriveCalculatorPtrType   m_calculator3_Ptr;
	bool  				     m_calculator3_init;

	DriveCalculatorPtrType   m_calculator4_Ptr;
	bool  				     m_calculator4_init;

//	DriveCalculatorPtrType   m_calculator5_Ptr; //will be PID turn
//	bool  				     m_calculator5_init;

	DriveCalculatorPtrType   m_calculator6_Ptr;
	bool  				     m_calculator6_init;

	DriveCalculatorPtrType   m_calculator7_Ptr; //will be PID turn
	bool  				     m_calculator7_init;

    StateObserverPtrType     m_stateObserverPtr;
    AutoScenarioStateEnum    m_currentState;
    bool					 m_armMovement;
};

#endif // LEFT_SIDE_LEFT_SCALE_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
