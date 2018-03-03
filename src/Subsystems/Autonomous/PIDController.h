/*
 * PIDController.h
 *
 *  Created on: Feb 28, 2018
 *      Author: wchs
 */

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

// SYSTEM INCLUDES
// #include <boost/shared_ptr.hpp>         // test harness version
#include <memory>                     // robot code version

// FORWARD DECLARATION
//class PIDController;
class PidController;

// TYPEDEFS
// typedef boost::shared_ptr<PidController>     PidControllerPtrType;     // test harness version
typedef std::shared_ptr<PidController>       PidControllerPtrType;     // robot code version


// CLASS DECLARATION
class PidController
{
public:
// LIFECYCLE
	PidController(float setPoint, float propConstant, float integralConstant, float derivConstant);
    ~PidController();

// METHODS
    void  setLimits(float lowerLimit, float upperLimit);
    void  setTolerance(float tolerance);

    bool  calculateValue(float &outputValue, float presentValue);

private:
// LIFECYCLE
    PidController();    // Hide the default constructor
    PidController(const PidController &);    // Hide the copy constructor

// OPERATORS
    PidController & operator =(const PidController &);    // Hide the assignment operator

// MEMBERS
    float  m_setPoint;                // Target value to stop at
    float  m_kPropConstant;           // Proportional constant
    float  m_kIntegralConstant;       // Integral constant
    float  m_kDerivConstant;          // Derivative constant

    float  m_tolerance;               // How close to be to the target to stop
    float  m_integralSum;             // Total sum of errors over calculations
    float  m_previousError;           // Error from the previous calculation

    bool   m_useLimits;
    float  m_lowerLimit;
    float  m_upperLimit;
};

#endif // PID_CONTROLLER_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////

