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
#include <sstream>

// PROJECT INCLUDES
#include "DriveMotorCalculator.h"

//  MAGIC VALUES
// Default motor powers to use when traveling > 100 cm
static const float s_DefaultStartUpPower(0.5);
static const float s_DefaultTravelPower(1.0);
static const float s_DefaultRampDownPower(0.20);
static const float s_DefaultFinishPower(0.0);

// Default motor powers to use when traveling < 100 cm
static const float s_MinStartUpPower(0.4);
static const float s_MinTravelPower(0.5);
static const float s_MinRampDownPower(0.20);
static const float s_MinFinishPower(0.0);

// Default limits on distances to cap how long a zone can be 
static const int  s_StartUpDistanceCm(5);
static const int  s_MinRampUpDistanceCm(5);
static const int  s_MaxRampUpDistanceCm(75);
static const int  s_MinRampDownDistanceCm(5);
static const int  s_MaxRampDownDistanceCm(150);
static const int  s_MaxStraightDistanceCm(20);

// Default percents of total distance for a zone expressed as a float
static const float s_DefaultRampUpMultiplier(0.4);
static const float s_DefaultRampDownMultiplier(0.4);
static const float s_DefaultStraightMultiplier(0.02);

// Distances where the power values change - maximum power varies by area
// Between 10-100 cm use min power values
// 100+ use default power values
static const int   s_MinPowerDistanceCm(100);

// Limit distances this code is expected to work with to 10-1500 cm
static const int   s_MinLimitDistanceCm(10);
static const int   s_MaxLimitDistanceCm(1500);  // length of the field

// Min and max power values a motor can handle
static const float s_MinPowerAllowed(-1.0);
static const float s_MaxPowerAllowed(1.0);

// Turning ratio correction factor
static const float s_TurningCorrectionMultiplier(5.0);

// Bit field values - used for tracking what zones are used in the calculations
static const int   s_CalcZoneStartUp(1);
static const int   s_CalcZoneRampUp(2);
static const int   s_CalcZoneTravel(4);
static const int   s_CalcZoneRampDown(8);
static const int   s_CalcZoneAll(s_CalcZoneStartUp | s_CalcZoneRampUp | s_CalcZoneTravel | s_CalcZoneRampDown);

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

DriveMotorCalculator::DriveMotorCalculator(int leftDistanceCm, int rightDistanceCm, int encoderPulsesPerCm) :
    m_startingLeftEncoder(0),
    m_startingRightEncoder(0),
	m_encoderPulsesPerCm(encoderPulsesPerCm),

    m_leftTotalDistanceCm(0),
    m_rightTotalDistanceCm(0),
    m_maxTotalDistanceCm(0),

    m_rampUpStartCm(0),
    m_travelStartCm(0),
    m_rampDownStartCm(0),
    m_turnStartCm(0),

    m_startUpPower(0.0),
    m_travelPower(0.0),
    m_rampDownPower(0.0),
    m_finalPower(0.0),

    m_goingBackwards(false),
    m_previousState(CalculatorStateUnknown),
    m_observerPtr(),
    m_zonesRequired(s_CalcZoneAll),
	m_percentDone(0.0)
{
    setTotalDistances(leftDistanceCm, rightDistanceCm);
    setDefaultZonePowers();
    setDefaultZoneStartPoints();

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

    validateIntegerity();
}


void  DriveMotorCalculator::removeStartUpZone() {
    m_zonesRequired = m_zonesRequired & ~s_CalcZoneStartUp;

    // Need startUpPower for the rampUpZone
//    m_startUpPower = 0.0;

    if ((m_zonesRequired & s_CalcZoneRampUp) == s_CalcZoneRampUp) {
        m_rampUpStartCm = 0;
        m_travelStartCm -= s_StartUpDistanceCm;
    }
    else {
        m_travelStartCm = 0;
    }

    validateIntegerity();
}

void  DriveMotorCalculator::removeRampUpZone() {
    m_zonesRequired = m_zonesRequired & ~s_CalcZoneRampUp;

    m_startUpPower  = 0.0;
    m_rampUpStartCm = 0;

    if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
        m_travelStartCm = s_StartUpDistanceCm;
    }
    else {
        m_travelStartCm = 0;
    }

    validateIntegerity();
}

void  DriveMotorCalculator::removeRampDownZone() {
    m_zonesRequired = m_zonesRequired & ~s_CalcZoneRampDown;

    m_rampDownPower   = 0.0;
    m_rampDownStartCm = 0;

    validateIntegerity();
}


void  DriveMotorCalculator::setStartUpPower(float power) {
    // Power for the motors when the robot is starting from being stopped
    if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
        m_startUpPower = validatePower(power);
    }

    validateIntegerity();
}

void  DriveMotorCalculator::setTravelPower(float power) {
    if ((m_zonesRequired & s_CalcZoneTravel) == s_CalcZoneTravel) {
        m_travelPower = validatePower(power);
    }

    validateIntegerity();
}

void  DriveMotorCalculator::setRampDownPower(float power) {
    // Power for the motors at the end of the distance travelled
    if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
        m_rampDownPower = validatePower(power);
    }

    validateIntegerity();
}

void  DriveMotorCalculator::setFinalPower(float power) {
    // Power for the motors after the distance is travelled
    // Want a smooth transition between segments so the finish power for the last segment
    //   is the initial power of next segment

    // Want m_rampDownPower to match m_finalPower
    if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
        m_rampDownPower = validatePower(power);
    }

    m_finalPower = validatePower(power);
    validateIntegerity();
}

void  DriveMotorCalculator::setRampUpDistance(int distanceCm) {
    if ((m_zonesRequired & s_CalcZoneRampUp) == s_CalcZoneRampUp) {
        // Adjust the travel zone start point based on the new rampUp distance
        distanceCm = std::abs(distanceCm);

        // Same calculations are used for setting default distances
        int   tmp_distance(std::min(distanceCm, s_MaxRampUpDistanceCm));

        tmp_distance = std::max(tmp_distance, s_MinRampUpDistanceCm);
        tmp_distance += m_rampUpStartCm;

        if (tmp_distance < m_rampDownStartCm) {
            m_travelStartCm = tmp_distance;
        }
    }

    validateIntegerity();
}

void  DriveMotorCalculator::setRampDownDistance(int distanceCm) {
    if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
        // Adjust the rampDown zone start point based on the new distance
        distanceCm = std::abs(distanceCm);

        // Same calculations are used for setting default distances
        int   tmp_distance(std::min(distanceCm, s_MaxRampDownDistanceCm));

        tmp_distance = std::max(tmp_distance, s_MinRampDownDistanceCm);
        tmp_distance = m_maxTotalDistanceCm - tmp_distance;

        if (tmp_distance > m_travelStartCm) {
            m_rampDownStartCm = tmp_distance;
        }
    }

    validateIntegerity();
}

void  DriveMotorCalculator::setObserver(StateObserverPtrType observerPtr) {
    m_observerPtr = observerPtr;
}


int  DriveMotorCalculator::getLeftDistanceCm() const {
	return m_leftTotalDistanceCm;
}

int  DriveMotorCalculator::getRightDistanceCm() const {
	return m_rightTotalDistanceCm;
}

bool  DriveMotorCalculator::getGoingBackwards() const {
    return m_goingBackwards;
}


bool  DriveMotorCalculator::getMotorSpeeds(float &leftMotorPower, float &rightMotorPower, int leftEncoder, int rightEncoder) {
	validateIntegerity();

	// Convert the encode values to distance travelled
	bool   retval(false);
	float  left_travel_cm(0.0);
	float  right_travel_cm(0.0);

	calculateTravelDistance(left_travel_cm, right_travel_cm, leftEncoder, rightEncoder);

	// Based on the distance travelled figure out what state the robot is in
	CalculatorStateEnum  motor_state(getMotorState(left_travel_cm, right_travel_cm));

	switch (motor_state) {
	case CalculatorStateStartUp:
		calculateStartUpSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case CalculatorStateRampUp:
		calculateRampUpSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case CalculatorStateTravel:
		calculateTravelSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case CalculatorStateRampDown:
		calculateRampDownSpeeds(leftMotorPower, rightMotorPower, left_travel_cm,  right_travel_cm);
		break;

	case CalculatorStateFinish:
		// The finish motor speed can be non-zero if this is a multi-segment route
		leftMotorPower  = m_finalPower;
		rightMotorPower = m_finalPower;
		retval = true;
		break;

	default:
		// Houston we have a problem...
		// We should never end up in the default case
		break;
	}

    correctPowers(leftMotorPower, rightMotorPower);
    calculatePercentDone(left_travel_cm, right_travel_cm);

    // Check if need to notify the state observer
    notifyObserver(motor_state);

	return retval;
}

std::string  DriveMotorCalculator::dumpObject() const {
    std::stringstream   stream;

    stream << "leftEncoder=" << m_startingLeftEncoder;
    stream << ", rightEncoder=" << m_startingRightEncoder;
	stream << ", pulsesPerCm=" << m_encoderPulsesPerCm;
    stream << std::endl;

    stream << "leftDistCm=" << m_leftTotalDistanceCm;
    stream << ", rightDistCm=" << m_rightTotalDistanceCm;
    stream << ", maxDistCm=" << m_maxTotalDistanceCm;
    stream << std::endl;

    stream << "backwards=" << m_goingBackwards;
    stream << ", previousState=" << m_previousState;
    stream << ", zonesRequired=" << m_zonesRequired;
    stream << std::endl;

    float startUp_power(m_startUpPower);
    float travel_power(m_travelPower);
    float rampDown_power(m_rampDownPower);
    float final_power(m_finalPower);

    if (m_goingBackwards == true) {
        startUp_power  *= -1.0;
        travel_power   *= -1.0;
        rampDown_power *= -1.0;
        final_power    *= -1.0;
    }

    // startUpZone data
    stream << "startUpZone: required=" << (m_zonesRequired & s_CalcZoneStartUp);
    stream << ", startCm=0";

    if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
        stream << ", distanceCm=" << s_StartUpDistanceCm;
    }
    else {
        stream << ", distanceCm=0";
    }

    stream << ", power=" << startUp_power;
    stream << std::endl;

    // rampUpZone data
    stream << "rampUpZone: required=" << (m_zonesRequired & s_CalcZoneRampUp);

    if ((m_zonesRequired & s_CalcZoneRampUp) == s_CalcZoneRampUp) {
        stream << ", startCm=" << m_rampUpStartCm;
        stream << ", distanceCm=" << (m_travelStartCm - m_rampUpStartCm);
        stream << ", startPower=" << startUp_power;
        stream << ", endPower=" << travel_power;
    }
    else {
        stream << ", startCm=0";
        stream << ", distanceCm=0";
        stream << ", startPower=0";
        stream << ", endPower=0";
    }

    stream << std::endl;

    // travelZone data
    stream << "travelZone: required=" << (m_zonesRequired & s_CalcZoneTravel);
    stream << ", startCm=" << m_travelStartCm;

    if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
        stream << ", distanceCm=" << (m_rampDownStartCm - m_travelStartCm);
    }
    else {
        stream << ", distanceCm=" << (m_maxTotalDistanceCm - m_travelStartCm);
    }

    stream << ", power=" << travel_power;
    stream << std::endl;

    // rampDownZone data
    stream << "rampDownZone: required=" << (m_zonesRequired & s_CalcZoneRampDown);

    if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
        stream << ", startCm=" << m_rampDownStartCm;
        stream << ", distanceCm=" << (m_maxTotalDistanceCm - m_rampDownStartCm);
        stream << ", startPower=" << travel_power;
        stream << ", endPower=" << m_rampDownPower;
    }
    else {
        stream << ", startCm=0";
        stream << ", distanceCm=0";
        stream << ", startPower=0";
        stream << ", endPower=0";
    }

    stream << std::endl;

    stream << "finalPower=" << final_power;

    stream << std::endl;
    stream << std::endl;

    return stream.str();
}

float  DriveMotorCalculator::getPerentDone() const {

	return m_percentDone;
}

// /////////////////////////////////  PRIVATE ////////////////////////////////////////////////
// **********************************  METHODS  **********************************************

void  DriveMotorCalculator::setTotalDistances(int leftDistanceCm, int rightDistanceCm) {
    // Found testing with Clyde that both distances could be 0
    // Expect to do nothing in this case
    if (leftDistanceCm != 0 && rightDistanceCm != 0) {
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

        // Determine which is the greater distance
        if (abs_left_distance >= abs_right_distance) {
            m_maxTotalDistanceCm = abs_left_distance;
        }
        else {
            m_maxTotalDistanceCm = abs_right_distance;
        }
    }
}

float  DriveMotorCalculator::validatePower(float power) const {
	// Calculations are done with positive values, and then flipped at end if going backwards
	// Let first if statement fall through to the second if statement below
    if (power < 0.0)  {
        power *= -1.0;
    }

    if (power > s_MaxPowerAllowed) {
        return s_MaxPowerAllowed;
    }

    return power;
}

void  DriveMotorCalculator::setDefaultZonePowers() {
    // Calculate powers based on the total distance
    if (m_maxTotalDistanceCm < s_MinPowerDistanceCm) {
        m_startUpPower  = s_MinStartUpPower;
        m_travelPower   = s_MinTravelPower;
        m_rampDownPower = s_MinRampDownPower;
        m_finalPower    = s_MinFinishPower;
    }
    else {
        // For longer distances (> 1M && < 15M) use default power values
        m_startUpPower  = s_DefaultStartUpPower;
        m_travelPower   = s_DefaultTravelPower;
        m_rampDownPower = s_DefaultRampDownPower;
        m_finalPower    = s_DefaultFinishPower;
    }
}

void  DriveMotorCalculator::setDefaultZoneStartPoints() {
    // Found testing with Clyde that both distances could be 0
    // Expect to do nothing in this case
    if (m_leftTotalDistanceCm != 0 && m_rightTotalDistanceCm != 0) {
        // startUp zone distance is a fixed distance
        m_rampUpStartCm = s_StartUpDistanceCm;

        // Calculate the travel zone start point
        float tmp_float(m_maxTotalDistanceCm * s_DefaultRampUpMultiplier);
        int   tmp_int(static_cast<int>(ceil(tmp_float)));
        int   tmp_distance(std::min(tmp_int, s_MaxRampUpDistanceCm));

        tmp_distance    = std::max(tmp_distance, s_MinRampUpDistanceCm);
        m_travelStartCm = m_rampUpStartCm + tmp_distance;

        // Calculate the rampDown zone distance
        tmp_float    = m_maxTotalDistanceCm * s_DefaultRampDownMultiplier;
        tmp_int      = static_cast<int>(ceil(tmp_float));
        tmp_distance = std::min(tmp_int, s_MaxRampDownDistanceCm);
        tmp_distance = std::max(tmp_distance, s_MinRampDownDistanceCm);

        m_rampDownStartCm = m_maxTotalDistanceCm - tmp_distance;

        // Calculate the turning and correction zone start
        tmp_float     = m_maxTotalDistanceCm * s_DefaultStraightMultiplier;
        tmp_int       = static_cast<int>(ceil(tmp_float));
        m_turnStartCm = std::min(tmp_int, s_MaxStraightDistanceCm);
    }
}

void  DriveMotorCalculator::calculateTravelDistance(float &leftTravelCm, float &rightTravelCm,
			int leftEncoder, int rightEncoder) const {
	// Change encoder pulses into distance the robot has travelled in CM
    const int  left_encoder_pulses(std::abs(leftEncoder - m_startingLeftEncoder));
    const int  right_encoder_pulses(std::abs(rightEncoder - m_startingRightEncoder));

	leftTravelCm  = static_cast<float>(left_encoder_pulses) / m_encoderPulsesPerCm;
	rightTravelCm = static_cast<float>(right_encoder_pulses) / m_encoderPulsesPerCm;
}

CalculatorStateEnum  DriveMotorCalculator::getMotorState(float leftTravelCm, float rightTravelCm) const {

    //  Check if both total distances are 0.  Found this happened sometimes with Clyde
    //  If both are 0 then set to final state, otherwise, the robot jerks and stops
	if (m_leftTotalDistanceCm == 0.0 && m_rightTotalDistanceCm == 0.0) {
		return CalculatorStateFinish;
	}

	// Zones can be removed.  Check if the zone is present
    const float  travel_distance(std::max(leftTravelCm, rightTravelCm));

	if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
	    if (travel_distance < m_rampUpStartCm) {
			return CalculatorStateStartUp;
	    }
	}

    if ((m_zonesRequired & s_CalcZoneRampUp) == s_CalcZoneRampUp) {
        if (travel_distance >= m_rampUpStartCm && travel_distance < m_travelStartCm) {
            return CalculatorStateRampUp;
        }
	}

    if ((m_zonesRequired & s_CalcZoneTravel) == s_CalcZoneTravel) {
        // Check if we have a rampDown or if it is absent
        if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
            if (travel_distance >= m_travelStartCm && travel_distance < m_rampDownStartCm) {
                return CalculatorStateTravel;
            }
        }
        else if (travel_distance >= m_travelStartCm &&
                 (leftTravelCm < m_leftTotalDistanceCm || rightTravelCm < m_rightTotalDistanceCm)) {
             return CalculatorStateTravel;
        }
 	}

    if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
        if (travel_distance >= m_rampDownStartCm &&
            (leftTravelCm < m_leftTotalDistanceCm || rightTravelCm < m_rightTotalDistanceCm)) {
            return CalculatorStateRampDown;
        }
	}

	if (leftTravelCm >= m_leftTotalDistanceCm && rightTravelCm >= m_rightTotalDistanceCm) {
		return CalculatorStateFinish;
	}

    return CalculatorStateUnknown;
}

void   DriveMotorCalculator::calculateStartUpSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
	// The starUp zone is is to get the robot moving without spinning the wheels
	// Assuming the robot is going from 0 to m_startUpPower
    // The startUp zone is impacted by correction or turning

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateCorrection(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

	leftMotorPower  = left_multiplier * m_startUpPower;
	rightMotorPower = right_multiplier * m_startUpPower;
}


void   DriveMotorCalculator::calculateRampUpSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
	// The rampUp zone is for increasing the speed of the robot
    // Assuming the robot is going from m_startUpPower to m_travelPower
    // The rampUp zone is impacted by correction or turning

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateCorrection(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

    if (m_travelPower == m_startUpPower) {
        // At small distances run at a constant speed - no rampup
    	leftMotorPower  = left_multiplier * m_travelPower;
	    rightMotorPower = right_multiplier * m_travelPower;
    }
    else {
	    // Calculate what percentage of the rampUp distance the robot has traveled
        // The speed of the robot is proportional to the distance travelled in the rampUp zone
        const float  max_travel((leftTravelCm > rightTravelCm) ? leftTravelCm : rightTravelCm);
        const float  ramp_travel(max_travel - m_rampUpStartCm);
        const float  ramp_distance(static_cast<float>(m_travelStartCm - m_rampUpStartCm));
    	const float  ramp_multiplier(ramp_travel / ramp_distance);
    	const float  power_diff(m_travelPower - m_startUpPower);

    	leftMotorPower  = left_multiplier * (m_startUpPower + (ramp_multiplier * power_diff));
	    rightMotorPower = right_multiplier * (m_startUpPower + (ramp_multiplier * power_diff));
    }
}

void   DriveMotorCalculator::calculateTravelSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
    // The travel zone is for the robot going at a constant high speed
    // Assuming the robot is staying at m_travelPower
    // The travel zone is impacted by correction or turning

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateCorrection(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

	leftMotorPower  = left_multiplier * m_travelPower;
	rightMotorPower = right_multiplier * m_travelPower;
}

void   DriveMotorCalculator::calculateRampDownSpeeds(float &leftMotorPower, float &rightMotorPower,
			float leftTravelCm, float rightTravelCm) const {
    // The rampDown zone is for decreasing the speed of the robot
    // Assuming the robot is going from m_travelPower to m_rampDownPower
    // The rampDown zone is impacted by correction or turning
    // *** If the m_rampDownPower is set to low then the motors will stop turning before the robot reaches its destination

	// Determine if have any variance between sides from turning
	float  left_multiplier(1.0);
	float  right_multiplier(1.0);

	calculateCorrection(left_multiplier, right_multiplier, leftTravelCm, rightTravelCm);

    if (m_travelPower == m_rampDownPower) {
        // At small distances run at a constant speed - no rampdown
    	leftMotorPower  = left_multiplier * m_rampDownPower;
	    rightMotorPower = right_multiplier * m_rampDownPower;
    }
    else {
    	// Calculate what percentage of the rampDown distance the robot has traveled
        // The speed of the robot is proportional to the distance travelled in the rampDown zone
        const float  max_travel((leftTravelCm > rightTravelCm) ? leftTravelCm : rightTravelCm);
        const float  ramp_travel(max_travel - m_rampDownStartCm);
        const float  ramp_distance(m_maxTotalDistanceCm - m_rampDownStartCm);
    	const float  ramp_multiplier(ramp_travel / ramp_distance);
	    const float  power_diff(m_travelPower - m_rampDownPower);

    	leftMotorPower  = left_multiplier * (m_travelPower - (ramp_multiplier * power_diff));
	    rightMotorPower = right_multiplier * (m_travelPower - (ramp_multiplier * power_diff));
    }
}

void   DriveMotorCalculator::calculateCorrection(float &leftMultiplier, float &rightMultiplier,
			float leftDistanceCm, float rightDistanceCm) const {
	// The robot will travel straight until it passes m_turnStartCm for both wheels
	// After passing m_turnStartCm the robot can begin corrections or to turn
	// The robot turns by adjusting the left and right motor speeds

	// Set multipliers to default values
	leftMultiplier  = 1.0;
	rightMultiplier = 1.0;

	if (leftDistanceCm >= m_turnStartCm && rightDistanceCm >= m_turnStartCm) {
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
	}
}

void   DriveMotorCalculator::correctPowers(float &leftMotorPower, float &rightMotorPower) const {
	// The motor speeds need to be between -1.0 and 1.0, if outside this range then clip
	if (leftMotorPower > s_MaxPowerAllowed) {
		leftMotorPower = s_MaxPowerAllowed;
	}

	if (rightMotorPower > s_MaxPowerAllowed) {
		rightMotorPower = s_MaxPowerAllowed;
	}

	// Check if going backwards - if so reverse the motor settings
	if (m_goingBackwards == true) {
		if (leftMotorPower > 0.0) {
			leftMotorPower *= -1.0;
		}

		if (rightMotorPower > 0.0) {
			rightMotorPower *= -1.0;
		}
	}
}

void   DriveMotorCalculator::calculatePercentDone(float leftTravelCm, float rightTravelCm)  {

	if (leftTravelCm == 0.0 && rightTravelCm == 0.0) {
		m_percentDone = 0.0;
	}
	else if (m_leftTotalDistanceCm == 0.0 && m_rightTotalDistanceCm == 0.0) {
		m_percentDone = 0.0;
	}
	else if (m_leftTotalDistanceCm > m_rightTotalDistanceCm) {
		m_percentDone = leftTravelCm / static_cast<float>(m_leftTotalDistanceCm);
	}
	else {
		m_percentDone = rightTravelCm / static_cast<float>(m_rightTotalDistanceCm);
    }
}


void  DriveMotorCalculator::notifyObserver(CalculatorStateEnum  motorState) {
    if (m_observerPtr && m_previousState != motorState) {
        switch (motorState) {
        case CalculatorStateStartUp:
            m_observerPtr->startStartUpState();
            break;

        case CalculatorStateRampUp:
            m_observerPtr->startRampUpState();
            break;

        case CalculatorStateTravel:
            m_observerPtr->startTravelState();
            break;

        case CalculatorStateRampDown:
            m_observerPtr->startRampDownState();
            break;

        case CalculatorStateFinish:
            m_observerPtr->startFinishState();
            break;

        default:
            // Houston we have a problem...
            // We should never end up in the default case
            break;
        }
    }

    m_previousState = motorState;
}

void   DriveMotorCalculator::validateIntegerity() const {
	// When in the test harness check variables for validity - define needs to be set in test harness
	// When not in the test harness do nothing
#ifdef TEST_HARNESS_ENABLE_CHECKS

//	m_startingLeftEncoder;
//	m_startingRightEncoder;
	assert(m_encoderPulsesPerCm > 0 && m_encoderPulsesPerCm < 100);

    if (m_leftTotalDistanceCm != 0 && m_rightTotalDistanceCm != 0) {
    	assert(m_leftTotalDistanceCm > 0);
    	assert(m_rightTotalDistanceCm > 0);

        if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
            assert(m_startUpPower > 0.0);
        }

        if ((m_zonesRequired & s_CalcZoneRampUp) == s_CalcZoneRampUp) {
            assert(m_startUpPower > 0.0);       // Need startUpPower for rampUpZone calculations
            assert(m_rampUpStartCm <= m_travelStartCm);

            if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
                assert(m_rampUpStartCm == s_StartUpDistanceCm);
            }
            else {
                assert(m_rampUpStartCm == 0);
            }
        }
        else {
            assert(m_rampUpStartCm == 0);
        }

        if ((m_zonesRequired & s_CalcZoneTravel) == s_CalcZoneTravel) {
            assert(m_travelPower > 0.0);

            if ((m_zonesRequired & s_CalcZoneRampUp) == s_CalcZoneRampUp) {
                assert(m_travelStartCm > m_rampUpStartCm);
            }
            else if ((m_zonesRequired & s_CalcZoneStartUp) == s_CalcZoneStartUp) {
                assert(m_travelStartCm == s_StartUpDistanceCm);
            }
            else {
                assert(m_travelStartCm == 0);
            }

            assert(m_travelStartCm < m_leftTotalDistanceCm);
            assert(m_travelStartCm < m_rightTotalDistanceCm);

            if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
                assert(m_rampDownPower > 0.0);
                assert(m_travelStartCm < m_rampDownStartCm);
            }
        }

        if ((m_zonesRequired & s_CalcZoneRampDown) == s_CalcZoneRampDown) {
            assert(m_rampDownPower > 0.0);
            assert(m_rampDownStartCm > m_travelStartCm);
            // assert(m_rampDownStartCm < m_leftTotalDistanceCm);
            // assert(m_rampDownStartCm < m_rightTotalDistanceCm);
        }
        else {
            assert(m_rampDownPower == 0.0);
            assert(m_rampDownStartCm == 0);
        }

    	assert(m_turnStartCm >= 0 && m_turnStartCm <= s_MaxStraightDistanceCm);
    }

//	assert(m_startUpPower >= 0.0);
//	assert(m_travelPower > 0.0);
//	assert(m_rampDownPower >= 0.0);
	assert(m_finalPower >= 0.0);

//	m_goingBackwards;

#endif	// TEST_HARNESS_ENABLE_CHECKS
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
