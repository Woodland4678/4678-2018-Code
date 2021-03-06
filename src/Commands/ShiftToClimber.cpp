// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "ShiftToClimber.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

ShiftToClimber::ShiftToClimber(): frc::Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::manipulatorArm.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void ShiftToClimber::Initialize() {
	moveCase = 0;
}

// Called repeatedly when this Command is scheduled to run
void ShiftToClimber::Execute() {
	switch(moveCase)
		{
		case 0:
			//Move arm to position
			if(Robot::manipulatorArm->moveTo(16,-0.8,-0.8))
				moveCase = 1;
			break;
		case 1:
			if(Robot::manipulatorArm->moveToRelative(13))
				moveCase = 2;
			break;
		case 2:
			//Switch to climber
			Robot::manipulatorArm->climber();
			moveCase = 3;
			break;
		case 3:
			double climberSpeed = Robot::oi->getdriver()->GetRawAxis(3);
			Robot::manipulatorArm->setShoulderPosition(Robot::manipulatorArm->getShoulderAngular() - (climberSpeed*300));
			break;
		}
}

// Make this return true when this Command no longer needs to run execute()
bool ShiftToClimber::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void ShiftToClimber::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ShiftToClimber::Interrupted() {

}
