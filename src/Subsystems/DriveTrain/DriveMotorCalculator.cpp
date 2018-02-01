/*
 * DriveMotorCalculator.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: wchs
 */

// The define below enables asserts within the code
// It should only be enabled when within the test harness - off for production
#define TEST_HARNESS_ENABLE_CHECKS    1

// SYSTEM INCLUDES
#include <cassert>
#include <cmath>

// PROJECT INCLUDES
#include "DriveMotorCalculator.h"

//  MAGIC VALUES
// Default motor powers to use
static const float s_DefaultStartUpPower(0.5);
static const float s_DefaultRampUpPower(0.75);
static const float s_DefaultTravelPower(1.0);
static const float s_DefaultRampDownPower(0.75);
static const float s_DefaultFinishPower(0.0);

// Default upper limits on distances to cap how long a zone can be 
static const int  s_MaxStartUpDistanceCm(5);
static const int  s_MaxRampUpDistanceCm(10);
static const int  s_MaxRampDownDistanceCm(10);
static const int  s_MaxStraightDistanceCm(20);

// Default percents of total distance for a zone expressed as a float
static const float s_DefaultStartUpMultiplier(0.01);
static const float s_DefaultRampUpMultiplier(0.1);
static const float s_DefaultRampDownMultiplier(0.1);
static const float s_DefaultStraightMultiplier(0.02);

// Distances where the power values change - maximum power varies by area
static const int   s_MinPowerDistanceCm(25);
static const int   s_MediumPowerDistanceCm(100);

// Limit distances this code is expected to work with
static const int   s_MinLimitDistanceCm(10);
static const int   s_MaxLimitDistanceCm(1500);  // length of the field

// Turning ratio correction factor - I copied this from the goToDistance code so have no idea why 5??
static const float s_TurningCorrectionMultiplier(5.0);

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

DriveMotorCalculator::DriveMotorCalculator(int leftEncoder, int rightEncoder, int leftDistanceCm,
							int rightDistanceCm, int encoderPulsesPerCm, float initialPower, float finishPower) :
    m_startingLeftEncoder(leftEncoder),
    m_startingRightEncoder(rightEncoder),
	m_encoderPulsesPerCm(encoderPulsesPerCm),

    m_leftTotalDistanceCm(0),
    m_rightTotalDistanceCm(0),

    m_rampUpStartCm(0),
    m_travelStartCm(0),
    m_rampDownStartCm(0),
    m_turnStartCm(0),

    m_startUpPower(0.0),
    m_rampUpPower(0.0),
    m_travelPower(0.0),
    m_rampDownPower(0.0),
    m_finalPower(0.0),

    m_goingBackwards(false)
{
    setTotalDistances(leftDistanceCm, rightDistanceCm);
    setZonePowers(initialPower, finishPower);
    setZoneStartPoints(initialPower);

    if (leftDistanceCm < 0 && rightDistanceCm < 0) {
        m_goingBackwards = true;
    }

    validateIntegerity();
}

DriveMotorCalculator::~DriveMotorCalculator() {
}

// **********************************  METHODS  **********************************************

void  DriveMotorCalculator::setStartingEncoders(int leftEncoder, int rightEncoder) {
	// This should only be called when doing a multi-segment route
	// Won't know the starting values of the encoders until starting the segment
    m_startingLeftEncoder  = leftEncoder;
    m_startingRightEncoder = rightEncoder;
}

void  DriveMotorCalculator::setStartUpPower(float power) {
    m_startUpPower = validatePower(power);
}

void  DriveMotorCalculator::setRampUpPower(float power) {
    m_rampUpPower = validatePower(power);
}

void  DriveMotorCalculator::setTravelPower(float power) {
    m_travelPower = validatePower(power);
}

void  DriveMotorCalculator::setRampDownPower(float power) {
    m_rampDownPower = validatePower(power);
}

void  DriveMotorCalculator::setFinalPower(float power) {
    m_finalPower = validatePower(power);
}

int  DriveMotorCalculator::getLeftDistanceCm() const {
	return m_leftTotalDistanceCm;
}

int   DriveMotorCalculator::getRightDistanceCm() const {
	return m_rightTotalDistanceCm;
}

bool  DriveMotorCalculator::getMotorSpeeds(float &leftMotorPower, float &rightMotorPower, int leftEncoder, int rightEncoder) const {
	validateIntegerity();

	// Convert the encode values to distance travelled
	bool   retval(false);
	float  left_travel_cm(0.0);
	float  right_travel_cm(0.0);

	calculateTravelDistance(left_travel_cm, right_travel_cm, leftEncoder, rightEncoder);

	// Based on the distance travelled figure out what state the robot is in
	MotorStateEnum  motor_state(getMotorState(left_travel_cm, right_travel_cm));

	switch (motor_state) {
	case MOTOR_STATE_START:
		leftMotorPower  = m_startUpPower;
		rightMotorPower = m_startUpPower;
		break;

	case MOTOR_STATE_RAMPUP:
		calculateRampUpSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case MOTOR_STATE_TRAVEL:
		calculateTravelSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case MOTOR_STATE_RAMPDOWN:
		calculateRampDownSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case MOTOR_STATE_FINISH:
		// The finish motor speed can be non-zero if this is a multi-segment route
		leftMotorPower  = m_finalPower;
		rightMotorPower = m_finalPower;
		retval = true;
		break;

	default:
		// Houston we have a problem...
		// We should never end up in the default case

		// TODO: figure out how to handle the error condition - throw an error
		break;
	}

	correctPowers(leftMotorPower, rightMotorPower);

	return retval;
}

// /////////////////////////////////  PRIVATE ////////////////////////////////////////////////

// **********************************  METHODS  **********************************************

void  DriveMotorCalculator::setTotalDistances(int leftDistanceCm, int rightDistanceCm) {
    const int  abs_left_distance(std::abs(leftDistanceCm));

    if (abs_left_distance < s_MinLimitDistanceCm) {
        m_leftTotalDistanceCm = s_MinLimitDistanceCm;
    }
    else if (abs_left_distance > s_MaxLimitDistanceCm) {
        m_leftTotalDistanceCm = s_MaxLimitDistanceCm;
    }
    else {
        m_leftTotalDistanceCm = abs_left_distance;
    }

    const int  abs_right_distance(std::abs(rightDistanceCm));

    if (abs_right_distance < s_MinLimitDistanceCm) {
        m_rightTotalDistanceCm = s_MinLimitDistanceCm;
    }
    else if (abs_right_distance > s_MaxLimitDistanceCm) {
        m_rightTotalDistanceCm = s_MaxLimitDistanceCm;
    }
    else {
        m_rightTotalDistanceCm = abs_right_distance;
    }
}

float  DriveMotorCalculator::validatePower(float power) const {
	// Calculations are done with positive values, and then flipped at end if going backwards
    if (power < 0.0)  {
        return std::abs(power);
    }

    if (power > 1.0) {
        return 1.0;
    }

    return power;
}

void  DriveMotorCalculator::setZonePowers(float initialPower, float finalPower) {
    // Calculate powers and zone starting points based on the total distance
    const int  max_distance((m_leftTotalDistanceCm > m_rightTotalDistanceCm) ? 
                                m_leftTotalDistanceCm : m_rightTotalDistanceCm);

    initialPower = validatePower(initialPower);
    finalPower   = validatePower(finalPower);

    if (initialPower == 0.0) {
        m_startUpPower  = s_DefaultStartUpPower;
    }

    if (max_distance < s_MinPowerDistanceCm) {
        // When going a minimal distance leave the robot at startup power
        m_rampUpPower   = s_DefaultStartUpPower;
        m_travelPower   = s_DefaultStartUpPower;
        m_rampDownPower = s_DefaultStartUpPower;
        m_finalPower    = finalPower;
    }
    else if (max_distance < s_MediumPowerDistanceCm) {
        // When going a short distance (< 1M) get to rampUp power
        m_rampUpPower   = s_DefaultRampUpPower;
        m_travelPower   = s_DefaultRampUpPower;
        m_rampDownPower = s_DefaultRampDownPower;
        m_finalPower    = finalPower;
    }
    else {
        // For longer distances (> 1M && < 15M) get to travelPower
        m_rampUpPower   = s_DefaultRampUpPower;
        m_travelPower   = s_DefaultTravelPower;
        m_rampDownPower = s_DefaultRampDownPower;
        m_finalPower    = finalPower;
    }
}

void  DriveMotorCalculator::setZoneStartPoints(float initialPower) {
    const int  max_distance((m_leftTotalDistanceCm > m_rightTotalDistanceCm) ? 
                                m_leftTotalDistanceCm : m_rightTotalDistanceCm);

    float  tmp_float;
    int    tmp_int;
    int    tmp_distance;

    // Calculate the startUp zone distance
    if (initialPower == 0.0) {
        tmp_float    = max_distance * s_DefaultStartUpMultiplier;
        tmp_int      = static_cast<int>(ceil(tmp_float));
        tmp_distance = std::min(tmp_int, s_MaxStartUpDistanceCm);

        m_rampUpStartCm = tmp_distance;
    }

    // Calculate the rampUp zone distance
    tmp_float    = max_distance * s_DefaultRampUpMultiplier;
    tmp_int      = static_cast<int>(ceil(tmp_float));
    tmp_distance = std::min(tmp_int, s_MaxRampUpDistanceCm);

    m_travelStartCm = m_rampUpStartCm + tmp_distance;

    // Calculate the rampDown zone distance
    tmp_float    = max_distance * s_DefaultRampDownMultiplier;
    tmp_int      = static_cast<int>(ceil(tmp_float));
    tmp_distance = std::min(tmp_int, s_MaxRampDownDistanceCm);

    m_rampDownStartCm = max_distance - tmp_distance;

    // Calculate the turning zone distance
    tmp_float    = max_distance * s_DefaultStraightMultiplier;
    tmp_int      = static_cast<int>(ceil(tmp_float));
    tmp_distance = std::min(tmp_int, s_MaxStraightDistanceCm);

    m_turnStartCm = tmp_distance;
}

void  DriveMotorCalculator::calculateTravelDistance(float &leftTravelCm, float &rightTravelCm,
			int leftEncoder, int rightEncoder) const {
	// Change encoder pulses into distance the robot has travelled in CM
	const float  left_encoder_pulses(static_cast<float>(std::abs(leftEncoder - m_startingLeftEncoder)));
	const float  right_encoder_pulses(static_cast<float>(std::abs(rightEncoder - m_startingRightEncoder)));

	leftTravelCm  = left_encoder_pulses / m_encoderPulsesPerCm;
	rightTravelCm = right_encoder_pulses / m_encoderPulsesPerCm;
}

MotorStateEnum  DriveMotorCalculator::getMotorState(float leftTravelCm, float rightTravelCm) const {
    const float  travel_distance((leftTravelCm > rightTravelCm) ? leftTravelCm : rightTravelCm);

	if (travel_distance < m_rampUpStartCm) {
		return MOTOR_STATE_START;
	}

	if (travel_distance >= m_rampUpStartCm && travel_distance < m_travelStartCm) {
		return MOTOR_STATE_RAMPUP;
	}

	if (travel_distance >= m_travelStartCm && travel_distance < m_rampDownStartCm) {
		return MOTOR_STATE_TRAVEL;
	}

	if (travel_distance >= m_rampDownStartCm && 
        leftTravelCm < m_leftTotalDistanceCm && rightTravelCm < m_rightTotalDistanceCm) {
		return MOTOR_STATE_RAMPDOWN;
	}

	if (leftTravelCm >= m_leftTotalDistanceCm && rightTravelCm >= m_rightTotalDistanceCm) {
		return MOTOR_STATE_FINISH;
	}

    return MOTOR_STATE_UNKNOWN;
}

void   DriveMotorCalculator::calculateRampUpSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
	// The rampUp zone is for gradually increasing the power to the motors
	// In order to get accurate values from the encoders the robot MUST avoid spinning the wheels
	// At the end of the rampUp zone the robot should be traveling at m_rampUpPower

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateTurnMultipliers(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

    if (m_rampUpPower == m_startUpPower) {
        // At small distances run at a constant speed - no rampup
    	leftMotorPower  = left_multiplier * m_rampUpPower;
	    rightMotorPower = right_multiplier * m_rampUpPower;
    }
    else {
	    // Calculate what percentage of the rampUp distance the robot has traveled
        const float  max_travel((leftTravelCm > rightTravelCm) ? leftTravelCm : rightTravelCm);
        const float  ramp_travel(max_travel - m_rampUpStartCm);
        const float  ramp_distance(static_cast<float>(m_travelStartCm - m_rampUpStartCm));
    	const float  ramp_multiplier(ramp_travel / ramp_distance);
    	const float  power_diff(m_rampUpPower - m_startUpPower);

    	leftMotorPower  = left_multiplier * (m_startUpPower + (ramp_multiplier * power_diff));
	    rightMotorPower = right_multiplier * (m_startUpPower + (ramp_multiplier * power_diff));
    }
}

void   DriveMotorCalculator::calculateTravelSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
	// The travel zone is between the rampUp zone and the rampDown zone
	// Other than turning we want the robot to travel at maximum speed

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateTurnMultipliers(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

	leftMotorPower  = left_multiplier * m_travelPower;
	rightMotorPower = right_multiplier * m_travelPower;
}


void   DriveMotorCalculator::calculateRampDownSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
	// The rampDown zone is for gradually decreasing the power to the motors
	// In order to get accurate values from the encoders the robot MUST avoid skidding
	// At the end of the rampDown zone the robot should be traveling at m_rampDownPower

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateTurnMultipliers(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

    if (m_travelPower == m_rampDownPower) {
        // At small distances run at a constant speed - no rampdown
    	leftMotorPower  = left_multiplier * m_rampDownPower;
	    rightMotorPower = right_multiplier * m_rampDownPower;
    }
    else {
    	// Calculate what percentage of the rampDown distance the robot has traveled
        const float  max_distance(static_cast<float>((m_leftTotalDistanceCm > m_rightTotalDistanceCm) ? 
                                    m_leftTotalDistanceCm : m_rightTotalDistanceCm));
        const float  max_travel((leftTravelCm > rightTravelCm) ? leftTravelCm : rightTravelCm);

        const float  ramp_travel(max_travel - m_rampDownStartCm);
        const float  ramp_distance(max_distance - m_rampUpStartCm);
    	const float  ramp_multiplier(ramp_travel / ramp_distance);
	    const float  power_diff(m_travelPower - m_rampDownPower);

    	leftMotorPower  = left_multiplier * (m_travelPower - (ramp_multiplier * power_diff));
	    rightMotorPower = right_multiplier * (m_travelPower - (ramp_multiplier * power_diff));
    }
}

void   DriveMotorCalculator::calculateTurnMultipliers(float &leftMultiplier, float &rightMultiplier,
			float leftDistanceCm, float rightDistanceCm) const {
	// The robot will travel straight until it passes m_straightDistanceCm
	// After passing m_straightDistanceCm the robot can begin to turn
	// The robot turns by adjusting the left and right motor speeds

	// Set multipliers to default values
	leftMultiplier  = 1.0;
	rightMultiplier = 1.0;

	if (m_leftTotalDistanceCm != m_rightTotalDistanceCm && 
        leftDistanceCm >= m_turnStartCm && rightDistanceCm >= m_turnStartCm) {
		// Calculate the percentage the left side has travelled
		const float  left_percent(leftDistanceCm / m_leftTotalDistanceCm);

		// Calculate the percentage the right side has travelled
		const float  right_percent(rightDistanceCm / m_rightTotalDistanceCm);

		// Difference between how far the left and right have gone
		const float  power_correction(s_TurningCorrectionMultiplier * std::abs(left_percent - right_percent));

		// If the right is closer than the left, increase the left power and decrease the right power
		const float  fudge_factor(0.001);
		const float  fudged_left(left_percent + fudge_factor);

		if (right_percent > fudged_left) {
			leftMultiplier  = 1.0 + power_correction;
			rightMultiplier = 1.0 - power_correction;
		}

		// If the left is closer than the right, increase the right power, and decrease the left power
		const float  fudged_right(right_percent + fudge_factor);

		if (left_percent > fudged_right) {
			leftMultiplier  = 1.0 - power_correction;
			rightMultiplier = 1.0 + power_correction;
		}
		printf("Right Mult= %f   Left Mult= %f   Corr=%f\n",right_percent,left_percent,std::abs(left_percent - right_percent));
	}
}

void   DriveMotorCalculator::correctPowers(float &leftMultiplier, float &rightMultiplier) const {
	// The motor speeds need to be between -1.0 and 1.0, if outside this range then clip
	if (leftMultiplier > 1.0) {
		leftMultiplier = 1.0;
	}

	if (rightMultiplier > 1.0) {
		rightMultiplier = 1.0;
	}

	// Check if going backwards - if so reverse the motor settings
	if (m_goingBackwards == true) {
		if (leftMultiplier > 0.0) {
			leftMultiplier *= -1.0;
		}

		if (rightMultiplier > 0.0) {
			rightMultiplier *= -1.0;
		}
	}
}

void   DriveMotorCalculator::validateIntegerity() const {
	// When in the test harness check variables for validity - define needs to be set in test harness
	// When not in the test harness do nothing
#ifdef TEST_HARNESS_ENABLE_CHECKS

//	m_startingLeftEncoder;
//	m_startingRightEncoder;
	assert(m_encoderPulsesPerCm > 0 && m_encoderPulsesPerCm < 100);

	assert(m_leftTotalDistanceCm != 0);
	assert(m_rightTotalDistanceCm != 0);

	assert(m_rampUpStartCm >= 0);
	assert(m_travelStartCm >= 0);
	assert(m_rampDownStartCm >= 0);
	assert(m_turnStartCm >= 0);

	assert(m_startUpPower > 0.0);
	assert(m_rampUpPower > 0.0);
	assert(m_travelPower > 0.0);
	assert(m_rampDownPower > 0.0);
	assert(m_finalPower >= 0.0);

//	m_goingBackwards;

#endif	// TEST_HARNESS_ENABLE_CHECKS
}

// //////////////////////////////////  EOF ///////////////////////////////////////////////////
