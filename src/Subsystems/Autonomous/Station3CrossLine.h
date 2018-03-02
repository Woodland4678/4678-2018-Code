/*
 * Station3CrossLine.h
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

#ifndef STATION_3_CROSS_LINE_H_
#define STATION_3_CROSS_LINE_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "../DriveTrain/DriveMotorCalculator.h"
#include "AutoScenario.h"

// CLASS DECLARATION
class Station3CrossLine : public AutoScenario
{
public:
// LIFECYCLE
	Station3CrossLine();
    virtual ~Station3CrossLine();

// METHODS
    void  initialize() override;
    void  execute() override;

private:
// LIFECYCLE
    Station3CrossLine(const Station3CrossLine &);		// Hide the copy constructor

// OPERATORS
    Station3CrossLine & operator=(const Station3CrossLine &);  // Hide the assignment operator

// MEMBERS
    DriveCalculatorPtrType   m_calculatorPtr;
	bool  				     m_calculatorInit;
};

#endif // STATION_3_CROSS_LINE_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
