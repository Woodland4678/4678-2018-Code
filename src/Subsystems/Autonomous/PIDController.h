/*
 * PIDController.h
 *
 *  Created on: Feb 28, 2018
 *      Author: wchs
 */

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES

// CLASS DECLARATION
class PIDController
{
public:
// LIFECYCLE
    PIDController(float setPoint, float propMultiplier, float integralMultiplier, float derivMultiplier);
    ~PIDController();

// METHODS
    void  setLimits(float lowerLimit, float upperLimit);
    void  setTolerance(float tolerance);

    bool  calculateValue(float &outputValue, float presentValue);

private:
// LIFECYCLE
    PIDController();    // Hide the default constructor
    PIDController(const PIDController &);    // Hide the copy constructor

// OPERATORS
    PIDController & operator =(const PIDController &);    // Hide the assignment operator

// METHODS
    float  checkLimits(float newValue) const;

// MEMBERS
    float  m_kProportionalConstant;   // Proportional constant
    float  m_kIntegralConstant;       // Integral constant
    float  m_kDerivativeConstant;     // Derivative constant

    float  m_setPoint;                // Target value to stop at
    float  m_tolerance;               // How close to be to the target to stop
    float  m_integralSum;             // Total sum of errors over calculations
    float  m_previousError;           // Error from the previous calculation

    bool   m_useLimits;
    float  m_lowerLimit;
    float  m_upperLimit;
};

#endif // PID_CONTROLLER_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////

