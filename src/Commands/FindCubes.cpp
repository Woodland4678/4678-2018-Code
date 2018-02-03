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
	doneGo = false;
	Search();
	FindPath();
}

void FindCubes::Search() {
	size_t cnt = Robot::lidar->GetRawData(Nodes);

	size_t filteredcnt = Robot::lidar->FilterRaw(Nodes, Filtered, cnt, 60,60,50,3000);
	Robot::lidar->ConvertToXY(Filtered, Data, filteredcnt);
	int lineCount = Robot::lidar->FindLines(Data, lines, filteredcnt);
	cubeCount = Robot::lidar->FindCubes(lines, cubes,lineCount);

	for(unsigned int i = 0; i<filteredcnt; i++)
		printf("%i, %i \n",Data[i].x,Data[i].y);

	for(int i = 0; i<=lineCount; i++)
		printf("Line %i: start(%i, %i) end(%i, %i) angle=%f length=%i \n",i, lines[i].start.x,lines[i].start.y, lines[i].end.x,lines[i].end.y,lines[i].angle,lines[i].length);

	for(int i = 0; i<cubeCount; i++)
		printf("Cube %i: Loca(%i, %i) dist=%i angle=%f\n",i, cubes[i].location.x,cubes[i].location.y, cubes[i].distance,cubes[i].angle);
}

void FindCubes::FindPath() {
	unsigned int idx = 0;
	int shortestdist = 6000;
	for (unsigned int i = 0; i < cubeCount; i++){
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
	printf("rightcm = %f   leftcm = %f\n", rightcm, leftcm);
}

// Called repeatedly when this Command is scheduled to run
void FindCubes::Execute() {
	doneGo = Robot::driveTrain->goToDistance(100, 75,0.75,10,10,0.5,0.5);
}

// Make this return true when this Command no longer needs to run execute()
bool FindCubes::IsFinished() {
    return doneGo;
}

// Called once after isFinished returns true
void FindCubes::End() {
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FindCubes::Interrupted() {

}
