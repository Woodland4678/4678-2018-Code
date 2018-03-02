// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "RaiseIntake.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

RaiseIntake::RaiseIntake(): frc::Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::intake.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void RaiseIntake::Initialize() {
	Robot::intake->release();

}

// Called repeatedly when this Command is scheduled to run
void RaiseIntake::Execute() {
	Robot::manipulatorArm->updateArm();
	if((Robot::manipulatorArm->endEffectorX > 0) && (Robot::manipulatorArm->endEffectorY < 14))
		return;
	Robot::intake->stopWheels();
	Robot::intake->setLifterPosition(572);
	Robot::intake->upPos = true;
}

// Make this return true when this Command no longer needs to run execute()
bool RaiseIntake::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void RaiseIntake::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void RaiseIntake::Interrupted() {

}
