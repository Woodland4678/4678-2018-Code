/*
 * PIDController.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: wchs
 */

// PROJECT INCLUDES
#include "PIDController.h"

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

PidController::PidController(float setPoint, float propConstant, float integralConstant, float derivConstant) :
    m_setPoint(setPoint),
    m_kPropConstant(propConstant),
    m_kIntegralConstant(integralConstant),
    m_kDerivConstant(derivConstant),
    m_tolerance(0.1),
    m_integralSum(0.0),
    m_previousError(0.0),
    m_useLimits(false),
    m_lowerLimit(0.0),
    m_upperLimit(0.0)
{ }


PidController::~PidController()
{ }

// **********************************  METHODS  **********************************************

void  PidController::setLimits(float lowerLimit, float upperLimit)  {
    m_useLimits  = true;
    m_lowerLimit = lowerLimit;
    m_upperLimit = upperLimit;
}


void  PidController::setTolerance(float tolerance) {
    m_tolerance = tolerance;
}


bool  PidController::calculateValue(float &outputValue, float presentValue)  {
    // Calculate the output values for PID for this round
    // Calculate (Kp * error) + (Ki * integral_value) + (Kd * deriv_value)

    // Proportional
    const float  error(m_setPoint - presentValue);
    const float  prop_value(error * m_kPropConstant);

    // Derivative
    const float  error_change(error - m_previousError);
    const float  deriv_value(error_change * m_kDerivConstant);

    // Integral
    m_integralSum += error;

    const float  integral_value(m_integralSum * m_kIntegralConstant);
    const float  total_value(prop_value + integral_value + deriv_value);

    // Set the values needed for the next round
    m_previousError = error;

    // Check if should use limits
    if (m_useLimits == true) {
        if (total_value < m_lowerLimit) {
            outputValue = m_lowerLimit;
        }
        else if (total_value > m_upperLimit) {
            outputValue = m_upperLimit;
        }
        else {
            outputValue = total_value;
        }
    }

    const float  lower_boundary(m_setPoint - m_tolerance);
    const float  upper_boundary(m_setPoint + m_tolerance);

    if (outputValue >= lower_boundary && outputValue <= upper_boundary) {
        return true;
    }

    return false;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
