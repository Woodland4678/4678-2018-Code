// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "IntakeWheelsControl.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

IntakeWheelsControl::IntakeWheelsControl(): frc::Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::intake.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void IntakeWheelsControl::Initialize() {
	done = false;
}

// Called repeatedly when this Command is scheduled to run
void IntakeWheelsControl::Execute() {
	double joy = Robot::oi->getdriver()->GetRawAxis(2);
	if(Robot::oi->getdriver()->GetRawButton(12))
		{
		if(joy > 0) //Intake wheels in
			{
			Robot::intake->spinForward(1.0);
			done = true;
			}
		else
			Robot::intake->spinReverse(1.0);
		}
	else
		{
		if(joy < 0)
			Robot::intake->stopWheels();
		done = true;
		}

}

// Make this return true when this Command no longer needs to run execute()
bool IntakeWheelsControl::IsFinished() {
    return done;
}

// Called once after isFinished returns true
void IntakeWheelsControl::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void IntakeWheelsControl::Interrupted() {

}
