/*
 * PIDController.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: wchs
 */

// PROJECT INCLUDES
#include "PIDController.h"



PIDController::PIDController(float setPoint, float propMultiplier, float integralMultiplier, float derivMultiplier) :
    m_kProportionalConstant(propMultiplier),
    m_kIntegralConstant(integralMultiplier),
    m_kDerivativeConstant(derivMultiplier),
    m_setPoint(setPoint),
    m_tolerance(0.1),
    m_integralSum(0.0),
    m_previousError(0.0),
    m_useLimits(false),
    m_lowerLimit(0.0),
    m_upperLimit(0.0)
{ }


PIDController::~PIDController()
{ }


void  PIDController::setLimits(float lowerLimit, float upperLimit)  {
    m_useLimits  = true;
    m_lowerLimit = lowerLimit;
    m_upperLimit = upperLimit;
}


void  PIDController::setTolerance(float tolerance) {
    m_tolerance = tolerance;
}


bool  PIDController::calculateValue(float &outputValue, float presentValue)  {
    // Calculate the output values for PID for this round
    // Calculate (Kp * error) + (Ki * integral_value) + (Kd * deriv_value)

    const float  error(m_setPoint - presentValue);
    const float  prop_out(error * m_kProportionalConstant);
    const float  integral_sum(m_integralSum + error);
    const float  integral_out(integral_sum * m_kIntegralConstant);
    const float  deriv_out((error - m_previousError) * m_kDerivativeConstant);
    const float  new_out(prop_out + integral_out + deriv_out);

    outputValue = checkLimits(new_out);

    // Set the values needed for the next round
    m_integralSum   = integral_sum;
    m_previousError = error;

    const float  lower_boundary(m_setPoint - m_tolerance);
    const float  upper_boundary(m_setPoint + m_tolerance);

    if (lower_boundary >= outputValue && outputValue <= upper_boundary) {
        return true;
    }

    return false;
}


float  PIDController::checkLimits(float newValue) const {
   // Check if should use limits
    if (m_useLimits == true) {
        if (newValue < m_lowerLimit) {
            return m_lowerLimit;
        }

        if (newValue > m_upperLimit) {
            return m_upperLimit;
        }
    }

    return newValue;
}
