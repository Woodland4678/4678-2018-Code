/*
 * LeftSideLeftScale.h
 *
 *  Created on: Mar 2, 2018
 *      Author: wchs
 */

#ifndef LEFT_SIDE_LEFT_SCALE_H_
#define LEFT_SIDE_LEFT_SCALE_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class LeftSideLeftScale : public AutoScenario
{
public:
// LIFECYCLE
	LeftSideLeftScale();
    virtual ~LeftSideLeftScale();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    LeftSideLeftScale(const LeftSideLeftScale &);		// Hide the copy constructor

// OPERATORS
    LeftSideLeftScale & operator=(const LeftSideLeftScale &);  // Hide the assignment operator

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

    bool					 m_armMovement1;
    bool					 m_armMovement2;
    bool 					 m_armMovement3;
    bool					 m_armMovement4;
    bool					 m_armMovement5;
    bool					 m_armMovement6;
    bool					 m_armMovement7;
    bool                     m_armInitMovement2;

    bool					 m_intakeDown;
    bool					 m_intakeMovement2;
    bool					 m_intakeMovement3;


    int                      m_cnt;

	int 					 leftDistance = 0;
	int 					 rightDistance = 0;
};

#endif // LEFT_SIDE_LEFT_SCALE_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
