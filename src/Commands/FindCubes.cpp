// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "FindCubes.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

FindCubes::FindCubes(): frc::Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::driveTrain.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void FindCubes::Initialize() {
	doneGo = false;
	getCubeCase = 0;
	printf("Here!");
}

// Called repeatedly when this Command is scheduled to run
void FindCubes::Execute() {
	if(!Robot::oi->getdriver()->GetRawButton(8))
		{
		Robot::lidar->cubeFindCase = 0;
		doneGo = true;
		return;
		}
	//if(Robot::lidar->findCubes(-180.0,180.0) == 1)
	//	doneGo = true;
	doneGo = Robot::intake->moveTo(2);
	/*switch(getCubeCase)
		{
		case 0:
			getCubeCase = 3;
			//Is the arm in the way? Is the intake already out?
			if(Robot::intake->checkPosition() != Robot::intake->IntakePositions::GetCube)
				getCubeCase = 2; //Move intake out

			//	If so, initialize the arm movement
			if((Robot::manipulatorArm->wristSeg.posX > 0) && (Robot::manipulatorArm->wristSeg.posY < 14))
				getCubeCase = 1; //Move arm out of the way
			break;
		case 1:
			//Move arm out of the way
			if(Robot::manipulatorArm->moveTo(11))
				getCubeCase = 0; //Go back to case 0 to check intake
			break;
		case 2:
			//Move intake out of the robot
			if(Robot::intake->moveTo(Robot::intake->IntakePositions::GetCube))
				getCubeCase = 0;
			break;
		case 3:
			{
			//At this point we know that the intake is out and the arm is out of the way
			//Find the nearest cube
			int value = Robot::lidar->findCubes();
			if(value == 1)
				{
				Robot::intake->grab();
				getCubeCase = 4;
				}
			if(value == 2)
				{
				getCubeCase = 0;
				doneGo = true;
				}
			}
			break;
		case 4:
			//Move to the cube if one is found
			//Clamp the cube
			break;
		case 5:
			//Use the lidar to ensure the cube is lined up correctly
			//	against the robot
			break;
		case 6:
			//Move arm into pickup location
			//Clamp cube
			//Stop intake wheels
			//Open intake
			break;
		case 7:
			//Move cube to carry position
			//Use lidar to make sure the cube is gone
			break;
		case 8:
			//Bring in the intake
			break;
		}*/
}

// Make this return true when this Command no longer needs to run execute()
bool FindCubes::IsFinished() {
    return doneGo;
}

// Called once after isFinished returns true
void FindCubes::End() {
	Robot::oi->driverRumble(0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FindCubes::Interrupted() {

}
