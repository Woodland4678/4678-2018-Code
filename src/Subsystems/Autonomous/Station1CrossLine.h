/*
 * Station1CrossLine.h
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

#ifndef STATION_1_CROSS_LINE_H_
#define STATION_1_CROSS_LINE_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class Station1CrossLine : public AutoScenario
{
public:
// LIFECYCLE
	Station1CrossLine();
    virtual ~Station1CrossLine();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    Station1CrossLine(const Station1CrossLine &);		// Hide the copy constructor

// OPERATORS
    Station1CrossLine & operator=(const Station1CrossLine &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculatorPtr;
	bool  				     m_calculatorInit;
};

#endif // STATION_1_CROSS_LINE_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
