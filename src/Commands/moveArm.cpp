// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "moveArm.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

moveArm::moveArm(int sender): frc::Command() {
    m_sender = sender;
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::manipulatorArm.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	prevJoyX = 0;
	prevJoyY = 0;
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void moveArm::Initialize() {
	done = false;
	Robot::manipulatorArm->initMovement();
	if(Robot::oi->getoperate()->GetRawButton(5))
		back = true;
	else
		back = false;

}

// Called repeatedly when this Command is scheduled to run
void moveArm::Execute() {
	switch(m_sender){
		case 0: //No button pressed, POV and Joystick controls here
			{
			//What positions are we allowing?
			if((Robot::manipulatorArm->currPos < 1)||(Robot::manipulatorArm->currPos > 4))
				return;

			double joyX = Robot::oi->getdriver()->GetRawAxis(2);
			double joyY = Robot::oi->getdriver()->GetRawAxis(3);

			//Check if there was enough of a change in the joystick to move the arm
			//	we don't want to be constantly telling the arm to be moving
			Robot::manipulatorArm->fineMoveCase = 0;
			moveInit = Robot::manipulatorArm->fineMovement(joyX,-joyY);
			Robot::manipulatorArm->fineMoveCase = 1;
			moveInit = Robot::manipulatorArm->fineMovement(joyX,-joyY);
			break;
			}
		case 1: //Carry Position
			done = Robot::manipulatorArm->moveTo(11);
			break;
		case 2: //Scale Low
			if(back)
				done = Robot::manipulatorArm->moveTo(4);
			else
				done = Robot::manipulatorArm->moveTo(2);
			break;
		case 3: //Switch High
			if(back)
				done = Robot::manipulatorArm->moveTo(8);
			else
				done = Robot::manipulatorArm->moveTo(7);
			break;
		case 4: //Scale High
			if(back)
				done = Robot::manipulatorArm->moveTo(3);
			else
				done = Robot::manipulatorArm->moveTo(1);
			break;
		case 6: //Pick up Cube
			Robot::manipulatorArm->release();
			done = Robot::manipulatorArm->moveTo(5);
			break;
		case 9: //Home
			done = Robot::manipulatorArm->moveTo(0);
			break;
		case 10: //Portal
			done = Robot::manipulatorArm->moveTo(6);
			break;
		case 11: //Climber
			done = Robot::manipulatorArm->moveTo(9);
			break;
		case 12: //Place Climber
			done = Robot::manipulatorArm->moveTo(10);
			break;
	}
}

// Make this return true when this Command no longer needs to run execute()
bool moveArm::IsFinished() {
    return done;
}

// Called once after isFinished returns true
void moveArm::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void moveArm::Interrupted() {

}
