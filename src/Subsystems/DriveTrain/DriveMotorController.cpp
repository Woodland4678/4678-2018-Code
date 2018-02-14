/*
 * DriveMotorController.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: CyberCavs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "DriveMotorController.h"

//  MAGIC VALUES
static const int  s_EncoderPulesPerCm(30);		// For Clyde use 30 pulses per cm


// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

DriveMotorController::DriveMotorController(
		std::shared_ptr<frc::SpeedController> leftMotorPtr, std::shared_ptr<frc::Encoder> leftEncoder,
		std::shared_ptr<frc::SpeedController> rightMotorPtr, std::shared_ptr<frc::Encoder> rightEncoder,
		int leftCentimeters, int rightCentimeters) :
	m_driveLeftMotor(leftMotorPtr),
	m_driveLeftEncoder(leftEncoder),
    m_driveRightMotor(rightMotorPtr),
	m_driveRightEncoder(rightEncoder),
    m_calculator(new DriveMotorCalculator(leftEncoder->Get(), rightEncoder->Get(),
    		leftCentimeters, rightCentimeters, s_EncoderPulesPerCm))
{ }


DriveMotorController::~DriveMotorController() {
	m_driveLeftMotor.reset();
	m_driveLeftEncoder.reset();
    m_driveRightMotor.reset();
	m_driveRightEncoder.reset();
	m_calculator.reset();
}

// **********************************  METHODS  **********************************************

bool   DriveMotorController::goToDistance() const {
	float  left_motor_power(0.0);
	float  right_motor_power(0.0);
	const int  left_encoder_value(m_driveLeftEncoder->Get());
	const int  right_encoder_value(m_driveRightEncoder->Get());

	bool  done(m_calculator->getMotorSpeeds(left_motor_power, right_motor_power, left_encoder_value, right_encoder_value));

	// For Clyde reverse the left motor setting
	m_driveLeftMotor->Set(-left_motor_power);
	m_driveRightMotor->Set(right_motor_power);

	return done;
}

