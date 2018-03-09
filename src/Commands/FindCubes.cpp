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
	cubeCount = 0;
	leftcm = 0;
	rightcm = 0;
	theta = 0;
	r = 0;
	rLeft = 0;
	rRight = 0;
	rumbleCount = 0;
	doneGo = false;
	Search();
	FindPath();
}

void FindCubes::Search() {

	cubeCount = 0;
}

void FindCubes::FindPath() {
	if (cubeCount == 0) {
		rightcm = 0;
		leftcm = 0;
		doneGo = true;
		return;
	}
	/*unsigned int idx = 0;
	int shortestdist = 6000;
	for (int i = 0; i < cubeCount; i++){
		if(cubes[i].distance < shortestdist){
			shortestdist = cubes[i].distance;
			idx = i;
		}
	}
	theta = (M_PI * cubes[idx].angle)/180;
	theta = std::abs(theta);
	r = (cubes[idx].distance * sin((M_PI/2) - theta))/(sin(2 * theta));
	rRight = ((cubes[idx].angle > 0) ? (r - 368) : (r + 368));
	rLeft = ((cubes[idx].angle > 0) ? (r + 368) : (r - 368));
	rightcm = (theta * rRight)/5;
	leftcm = (theta * rLeft)/5;

	printf("rightcm = %f   leftcm = %f\n", rightcm, leftcm);*/
}

// Called repeatedly when this Command is scheduled to run
void FindCubes::Execute() {
	//Check if there are any cubes
	if (cubeCount == 0)
		doneGo = true;
	//Check if button released
	if(!Robot::oi->getdriver()->GetRawButton(6))
		doneGo = true;
	if (rumbleCount < 25)
		{
		Robot::oi->driverRumble(0.2);
		rumbleCount++;
		}
	else
		Robot::oi->driverRumble(0);
	if(!doneGo)
		doneGo = Robot::driveTrain->goToDistance(rightcm, leftcm, 0.75);
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
