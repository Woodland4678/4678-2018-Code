/*
 * DriveMotorController.h
 *
 *  Created on: Feb 6, 2018
 *      Author: CyberCavs
 */

#ifndef SRC_SUBSYSTEMS_DRIVETRAIN_DRIVEMOTORCONTROLLER_H_
#define SRC_SUBSYSTEMS_DRIVETRAIN_DRIVEMOTORCONTROLLER_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "DriveMotorCalculator.h"
#include "WPILib.h"



class DriveMotorController {
public:
// LIFECYCLE
	DriveMotorController(std::shared_ptr<frc::SpeedController> leftMotorPtr, std::shared_ptr<frc::Encoder> leftEncoder,
			std::shared_ptr<frc::SpeedController> rightMotorPtr, std::shared_ptr<frc::Encoder> rightEncoder,
			int leftCentimeters, int rightCentimeters);

	virtual ~DriveMotorController();

// METHODS
	bool   goToDistance() const;

private:
// LIFECYCLE
	DriveMotorController();	  // Hide the default constructor
	DriveMotorController(const DriveMotorController &);		// Hide the copy constructor

// OPERATORS
	DriveMotorController & operator=(const DriveMotorController &);  // Hide the assignment operator

// VARIABLES
	std::shared_ptr<frc::SpeedController>     m_driveLeftMotor;
	std::shared_ptr<frc::Encoder>             m_driveLeftEncoder;

	std::shared_ptr<frc::SpeedController>     m_driveRightMotor;
	std::shared_ptr<frc::Encoder>             m_driveRightEncoder;

	std::shared_ptr<DriveMotorCalculator>     m_calculator;

};

#endif /* SRC_SUBSYSTEMS_DRIVETRAIN_DRIVEMOTORCONTROLLER_H_ */
