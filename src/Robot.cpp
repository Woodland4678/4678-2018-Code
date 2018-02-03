// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.

#include <math.h>

#include "Robot.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
std::shared_ptr<DriveTrain> Robot::driveTrain;
std::shared_ptr<Lidar> Robot::lidar;
std::unique_ptr<OI> Robot::oi;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION

rplidar_response_measurement_node_t Nodes[720];
rplidar_response_measurement_node_t Filtered[720];
tpPoint Data[720];
tpLine lines[100];
tpCube cubes[5];

int cubeCount;
double leftcm, rightcm, theta, r, rLeft, rRight;
bool doneGo = false;

void Robot::RobotInit() {
	RobotMap::init();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    driveTrain.reset(new DriveTrain());
    lidar.reset(new Lidar());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	// This MUST be here. If the OI creates Commands (which it very likely
	// will), constructing it during the construction of CommandBase (from
	// which commands extend), subsystems are not guaranteed to be
	// yet. Thus, their requires() statements may grab null pointers. Bad
	// news. Don't move it.
	oi.reset(new OI());

	// Add commands to Autonomous Sendable Chooser
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS


	chooser.AddDefault("Autonomous Command", new AutonomousCommand());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
	frc::SmartDashboard::PutData("Auto Modes", &chooser);
	frc::SmartDashboard::PutNumber("LidarFrequency", 0.0);
	frc::SmartDashboard::PutNumber("LidarPWM", 660);
	frc::SmartDashboard::PutNumber("LeftEncoder", 0.0);
	frc::SmartDashboard::PutNumber("RightEncoder", 0.0);
	frc::SmartDashboard::PutNumber("LeftPower", 0.0);
	frc::SmartDashboard::PutNumber("RightPower", 0.0);
}

/**
 * This function is called when the disabled button is hit.
 * You can use it to reset subsystems before shutting down.
 */
void Robot::DisabledInit(){
	lidar->Stop();
}

void Robot::DisabledPeriodic() {
	frc::Scheduler::GetInstance()->Run();
}

void Robot::AutonomousInit() {
	autonomousCommand = chooser.GetSelected();
	if (autonomousCommand != nullptr)
		autonomousCommand->Start();
}

void Robot::AutonomousPeriodic() {
	frc::Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// these lines or comment it out.
	done = false;
	if (autonomousCommand != nullptr)
		autonomousCommand->Cancel();

	Robot::lidar->Start();
}

void Robot::TeleopPeriodic() {
	frc::Scheduler::GetInstance()->Run();
	//frc::SmartDashboard::PutNumber("LidarFrequency", Robot::lidar->getFrequency());
	int pwm = frc::SmartDashboard::GetNumber("LidarPWM", 660);
	frc::SmartDashboard::PutNumber("LeftEncoder", Robot::driveTrain->getLeftEncoder()->Get());
	frc::SmartDashboard::PutNumber("RightEncoder", Robot::driveTrain->getRightEncoder()->Get());
	frc::SmartDashboard::PutNumber("LeftPower", -RobotMap::driveTrainLeftMotor->Get());
	frc::SmartDashboard::PutNumber("RightPower", RobotMap::driveTrainRightMotor->Get());
	Robot::lidar->setMotorSpeed(pwm);
	/*if((oi->getDriver()->GetRawButton(1))&&(!done))
		{
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
		done = true;
		}
	if(oi->getDriver()->GetRawButton(2))
		done = false;
	if(oi->getDriver()->GetRawButton(3))
		{
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
	if((oi->getDriver()->GetRawButton(4))&&(!doneGo)){
		doneGo = driveTrain->goToDistance(-rightcm, -leftcm,0.75,10,10,0.5,0.5);
	}
	frc::SmartDashboard::PutNumber("Done",done);*/
}

START_ROBOT_CLASS(Robot);

