/*
 * DriveMotorCalculator.h
 *
 *  Created on: Jan 24, 2018
 *      Author: wchs
 */

#ifndef DRIVE_MOTOR_CALCULATOR_H_
#define DRIVE_MOTOR_CALCULATOR_H_

// SYSTEM INCLUDES
#include <string>

// TODO: change this to a class enum for c11
// ENUMS
enum MotorStateEnum
{
   MOTOR_STATE_UNKNOWN = 0,
   MOTOR_STATE_START,
   MOTOR_STATE_RAMPUP,
   MOTOR_STATE_TRAVEL,
   MOTOR_STATE_RAMPDOWN,
   MOTOR_STATE_FINISH,
   MOTOR_STATE_MAX
};


class DriveMotorCalculator
{
public:
// LIFECYCLE
    DriveMotorCalculator(int leftEncoder, int rightEncoder, int leftDistanceCm, int rightDistanceCm, int encoderPulsesPerCm,
                         float initialPower = 0.0, float finalPower = 0.0);
    ~DriveMotorCalculator();

// METHODS
    void  setStartingEncoders(int leftEncoder, int rightEncoder);
    void  setStartUpPower(float power);
    void  setTravelPower(float power);
    void  setRampDownPower(float power);
    void  setFinalPower(float power);

    int   getLeftDistanceCm() const;
    int   getRightDistanceCm() const;

    bool  getMotorSpeeds(float &leftMotorPower, float &rightMotorPower, int leftEncoder, int rightEncoder) const;

    std::string  dumpObject() const;

private:
    // TODO: change the constructors and operators to c11
// LIFECYCLE
    DriveMotorCalculator();	  // Hide the default constructor
    DriveMotorCalculator(const DriveMotorCalculator &);		// Hide the copy constructor

// OPERATORS
    DriveMotorCalculator & operator=(const DriveMotorCalculator &);  // Hide the assignment operator

// METHODS
    void   setTotalDistances(int leftDistanceCm, int rightDistanceCm);
    float  validatePower(float powerValue) const;

    void  setZonePowers(float initialPower, float finalPower);
    void  setZoneStartPoints(float initialPower);
    void  calculateTravelDistance(float &leftTravelCm, float &rightTravelCm, int leftEncoder, int rightEncoder) const;

    MotorStateEnum  getMotorState(float leftTravelCm, float rightTravelCm) const;

    void   calculateStartUpSpeeds(float &leftMotorPower, float &rightMotorPower, float leftDistanceCm, float rightDistanceCm) const;
    void   calculateRampUpSpeeds(float &leftMotorPower, float &rightMotorPower, float leftDistanceCm, float rightDistanceCm) const;
    void   calculateTravelSpeeds(float &leftMotorPower, float &rightMotorPower, float leftTravelCm, float rightTravelCm) const;
    void   calculateRampDownSpeeds(float &leftMotorPower, float &rightMotorPower, float leftTravelCm, float rightTravelCm) const;
    void   calculateTurnMultipliers(float &leftMultiplier, float &rightMultiplier, float leftTravelCm, float rightTravelCm) const;
    void   correctPowers(float &leftMotorPower, float &rightMotorPower) const;
    void   validateIntegerity() const;

// VARIABLES
    int    m_startingLeftEncoder;		// starting value of the left encoder
    int    m_startingRightEncoder;		// starting value of the right encoder
    int    m_encoderPulsesPerCm;		// number of encoder pulese per CM, can change from year to year

    int    m_leftTotalDistanceCm;       // number of CMs for the left wheels to travel
    int    m_rightTotalDistanceCm;      // number of CMs for the right wheels to travel
    int    m_maxTotalDistanceCm;        // number of CMs max(leftDistance, rightDistance)

    int    m_rampUpStartCm;             // number of CMs to the start of the rampUp zone
    int    m_travelStartCm;             // number of CMs to the start of the travel zone
    int    m_rampDownStartCm;           // number of CMs to the start of the rampDown zone
    int    m_turnStartCm;               // number of CMs from the starting point before the robot can start turning

    float  m_startUpPower;				// power the motors should be set to on startup
    float  m_travelPower;				// power the motors should be set to between rampUp and rampDown
    float  m_rampDownPower;				// power the motors should be set to at the end of the rampDown zone
    float  m_finalPower;				// power the motors should be after the robot has traveled the distance

    bool   m_goingBackwards;			// whether the robot is going forward or backwards
};

#endif /* DRIVE_MOTOR_CALCULATOR_H_ */
