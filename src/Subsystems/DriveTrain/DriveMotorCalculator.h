/*
 * DriveMotorCalculator.h
 *
 *  Created on: Jan 24, 2018
 *      Author: wchs
 */

#ifndef DRIVE_MOTOR_CALCULATOR_H_
#define DRIVE_MOTOR_CALCULATOR_H_

// SYSTEM INCLUDES
#include <memory>                     // robot code version
#include <string>

// PROJECT INCLUDES
#include "DriveStateObserver.h"

// TODO: change this to a class enum for c11
// ENUMS
enum CalculatorStateEnum
{
   CalculatorStateUnknown= 0,
   CalculatorStateStartUp,
   CalculatorStateRampUp,
   CalculatorStateTravel,
   CalculatorStateRampDown,
   CalculatorStateFinish,
   CalculatorStateMax
};

// FORWARD DECLARATION
class DriveMotorCalculator;

// TYPEDEFS
typedef std::shared_ptr<DriveMotorCalculator>    DriveCalculatorPtrType;     // robot code version

// CLASS DECLARATION
class DriveMotorCalculator
{
public:
// LIFECYCLE
    DriveMotorCalculator(int leftDistanceCm, int rightDistanceCm, int encoderPulsesPerCm);
    ~DriveMotorCalculator();

// METHODS
    void  setStartingEncoders(int leftEncoder, int rightEncoder);

    void  removeStartUpZone();
    void  removeRampUpZone();
    void  removeRampDownZone();

    void  setStartUpPower(float power);
    void  setTravelPower(float power);
    void  setRampDownPower(float power);
    void  setFinalPower(float power);

    void  setRampUpDistance(int distanceCm);
    void  setRampDownDistance(int distanceCm);

    void  setObserver(StateObserverPtrType observerPtr);

    int   getLeftDistanceCm() const;        // needed in test harness
    int   getRightDistanceCm() const;       // needed in test harness
    bool  getGoingBackwards() const;        // needed in test harness

    bool  getMotorSpeeds(float &leftMotorPower, float &rightMotorPower, int leftEncoder, int rightEncoder);

    std::string  dumpObject() const;
    float  getPercentDone() const;

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

    void  setDefaultZonePowers();
    void  setDefaultZoneStartPoints();

    bool  checkIfStopped(int leftEncoder, int rightEncoder);
    void  calculateTravelDistance(float &leftTravelCm, float &rightTravelCm, int leftEncoder, int rightEncoder) const;

    CalculatorStateEnum  getMotorState(float leftTravelCm, float rightTravelCm) const;

    void   calculateStartUpSpeeds(float &leftMotorPower, float &rightMotorPower, float leftDistanceCm, float rightDistanceCm) const;
    void   calculateRampUpSpeeds(float &leftMotorPower, float &rightMotorPower, float leftDistanceCm, float rightDistanceCm) const;
    void   calculateTravelSpeeds(float &leftMotorPower, float &rightMotorPower, float leftTravelCm, float rightTravelCm) const;
    void   calculateRampDownSpeeds(float &leftMotorPower, float &rightMotorPower, float leftTravelCm, float rightTravelCm) const;
    void   calculateCorrection(float &leftMultiplier, float &rightMultiplier, float leftTravelCm, float rightTravelCm) const;
    void   correctPowers(float &leftMotorPower, float &rightMotorPower) const;
    void   calculatePercentDone(float leftTravelCm, float rightTravelCm);
    void   notifyObserver(CalculatorStateEnum  motorState);
    void   validateIntegerity() const;

// VARIABLES
    int    m_startingLeftEncoder;		// starting value of the left encoder
    int    m_startingRightEncoder;		// starting value of the right encoder
    int    m_encoderPulsesPerCm;		// number of encoder pulese per CM, can change from year to year

    int    m_leftTotalDistanceCm;       // number of CMs for the left wheels to travel (abs - used for calculations)
    int    m_rightTotalDistanceCm;      // number of CMs for the right wheels to travel (abs - used for calculations)
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
    CalculatorStateEnum   m_previousState;    // Tracks the previous motor state value so can identify when a state change happens
    StateObserverPtrType  m_observerPtr;      // Who to notify for state changes
    unsigned int          m_zonesRequired;    // What zones are required for the calculations
    float  m_percentDone;				// what percent of the total distance the robot has covered

    int    m_prevLeftEncoder;           // value of the left encoder last time getMotorSpeeds was called
    int    m_prevRightEncoder;          // value of the right encoder last time getMotorSpeeds was called
    int    m_stoppedCnt;                // counter to track when encoder values don't change
    CalculatorStateEnum   m_prevState;   // previous state of the calculator code
};

#endif /* DRIVE_MOTOR_CALCULATOR_H_ */

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
