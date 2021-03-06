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
#include "RaiseIntake.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

moveArm::moveArm(int sender): frc::Command() {
    m_sender = sender;
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::manipulatorArm.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void moveArm::Initialize() {
	if (Robot::isAuto) {
		done = true;
		return;
	}
	done = false;
	done2 = false;
	done3 = false;
	//Robot::manipulatorArm->initMovement();
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
			//Pov stuffs
			int povValue = Robot::oi->getoperate()->GetPOV();
			switch(povValue)
				{
				case -1:
					break;
				case 0:
					break;
				case 90:
					m_sender = 11;
					break;
				case 180:
					m_sender = 15;
					break;
				case 270:
					m_sender = 12;
					break;
				}
			/*frc::SmartDashboard::PutBoolean("Climb", Robot::manipulatorArm->isClimber);
			if(Robot::manipulatorArm->isClimber)
				{
				double climberSpeed = Robot::oi->getdriver()->GetRawAxis(3);
				printf("Here!\n");
				Robot::manipulatorArm->setShoulderPosition(Robot::manipulatorArm->getShoulderAngular() - (climberSpeed*50));
				}*/
			//What positions are we allowing?
			double joyX = Robot::oi->getoperate()->GetRawAxis(2);
			double joyY = Robot::oi->getoperate()->GetRawAxis(3);
			double wristMove = Robot::oi->getoperate()->GetRawAxis(1);
			if((std::abs(joyX-0) < 0.07)&&(std::abs(joyY-0) < 0.07))
				{
				frc::SmartDashboard::PutBoolean("Fine Motion Override", false);
				Robot::manipulatorArm->fineMovingGoing = 0;
				}
			else
				{
				frc::SmartDashboard::PutBoolean("Fine Motion Override", true);
				Robot::manipulatorArm->fineMovingGoing = 1;
				}

			//What positions are we allowing?
			if((Robot::manipulatorArm->targetPos < 1)||(Robot::manipulatorArm->targetPos > 4))
				{
				if((Robot::manipulatorArm->targetPos <= 8) || (Robot::manipulatorArm->targetPos == 11))
					{
					frc::SmartDashboard::PutBoolean("Fine Motion Control", false);
					return;
					}
				}
			frc::SmartDashboard::PutBoolean("Fine Motion Control", true);
			/*joyValuesX[joyCount] = joyX;
			joyValuesY[joyCount] = joyY;
			joyCount++;
			if(joyCount == 4)
				{
				//Filters, remove largest and lowest and average the remaining two
				double large = -3, small = 3;
				int larId = 0, smaId = 0;
				for(int i=0;i<4;i++)
					{
					double val = joyValuesX[i] + joyValuesY[i];
					if(val > large)
						{
						large = val;
						larId = i;
						continue;
						}
					if(val < small)
						{
						small = val;
						smaId = i;
						continue;
						}
					}
				//Average
				double sumX = 0;
				double sumY = 0;
				for(int i=0;i<4;i++)
					{
					if((i == larId)||(i == smaId))
						continue;
					sumX += joyValuesX[i];
					sumY += joyValuesY[i];
					}
				moveInit = Robot::manipulatorArm->fineMovement(sumX / 2,-(sumY/2));
				joyCount = 0;
				}*/

			break;
			}
		case 1: //Carry Position
			done = Robot::manipulatorArm->moveTo(11);
			/*if((Robot::manipulatorArm->wristSeg.posX < 0) && (Robot::manipulatorArm->wristSeg.posY > 18))
				{
				//Once this movement is done check if there is a cube in the claw
				if(Robot::manipulatorArm->checkForCube())
					{
					//Bring in the intake
					Robot::intake->checkPosition();
					done = Robot::intake->moveTo(Robot::intake->IntakePositions::InRobot);
					}
				else
					{
					if(done2) //Wait for the arm movement is complete
						done = true; //No cube then end the command
					}
				}*/

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
			/*if(done2)
				{
				Robot::intake->release();
				//Check for a cube
				if(Robot::manipulatorArm->checkForCube())
					{
					//We have a cube take it back to carry
					if(!done3)
						done3 = Robot::manipulatorArm->moveTo(11);
					else
						{
						Robot::intake->checkPosition();
						done = Robot::intake->moveTo(Robot::intake->IntakePositions::InRobot);
						}
					}
				else
					done = Robot::manipulatorArm->moveTo(11); //failed to get cube, return
				}*/
			break;
		case 9: //Home
			done = Robot::manipulatorArm->moveTo(0);
			break;
		case 10: //Portal
			done = Robot::manipulatorArm->moveToRelative(6);
			break;
		case 11: //Climber
			if(!done3)
				{
				//if(Robot::manipulatorArm->currPos != 8)
				//	done3 = Robot::manipulatorArm->moveTo(8);
				//else
					done3= true;
				}

			if(!done2 && done3)
				done2 = Robot::manipulatorArm->moveTo(9);
			if(done2 && done3)
				{
				if (Robot::manipulatorArm->moveTo(10))
					{
					done = true;
					m_sender = 0;
					}
				}
			break;
		case 12: //Place Climber
			if(!done2)
				done2 = Robot::manipulatorArm->moveTo(14);
			if(done2)
				{
				done = Robot::manipulatorArm->moveTo(15);
				if(done)
					m_sender = 0;
				}
			break;
		case 13:
			//Move elbow down
			//Robot::manipulatorArm->setElbowSpeed(-0.2);
			done = true;
			break;
		case 14:
			//Move shoulder back
			//Robot::manipulatorArm->setShoulderSpeed(-0.1);
			done = true;
			break;
		case 15:
			//Calibrate
			if(!done2)
				done2 = Robot::manipulatorArm->moveTo(11);
			else
				{
				if(Robot::manipulatorArm->calibrate())
					{
					done = true;
					m_sender = 0;
					}
				}
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
	if((m_sender == 15)||(m_sender == 11)||(m_sender == 12))
		m_sender = 0;
}
