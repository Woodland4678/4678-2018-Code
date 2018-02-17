// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "ScaleDetection.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

ScaleDetection::ScaleDetection() : frc::Subsystem("ScaleDetection") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    cntFront = new frc::Counter(9);
    cntBack = new frc::Counter(8);

    cntFront->SetSemiPeriodMode(true);
    cntBack->SetSemiPeriodMode(true);

}

void ScaleDetection::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}

void ScaleDetection::Periodic() {
    // Put code here to be run every loop

}

//Returns an int depending on whether or not the scale is found
//	0 = No scale found
//	1 = Scale found in front of robot
//	2 = Scale found behind the robot
int ScaleDetection::FoundScale() {
	double distFront = ReadFront();
	double distBack = ReadBack();
	//Check front
	if ((distFront <= SCALEMAXMAX)&&(distFront >= SCALEMINMIN))
		return 1;
	//Check back
	if ((distBack <= SCALEMAXMAX)&&(distBack >= SCALEMINMIN))
		return 2;

	//Else return No scale
	return 0;
}

//Returns an int depending on what the sensors are picking up
//	0 = No Scale
//	1 = Scale Front High Position
//	2 = Scale Front Start Position
//	3 = Scale Front Low Position
//	4 = Scale Back High Position
//	5 = Scale Back Start Position
//	6 = Scale Back Low Position
//	7 = Unknown
int ScaleDetection::GetScalePosition() {
	double distFront = ReadFront();
	double distBack = ReadBack();

	frc::SmartDashboard::PutNumber("Front Dist", distFront);
	//frc::SmartDashboard::PutNumber("Intake Thing", clydeIntake->Pos());

	double dist = 0;
	int out = 1;
	//Are we picking up the scale?
	if ((distFront <= SCALEMAXMAX)&&(distFront >= SCALEMINMIN))
		dist = distFront;
	if ((distBack <= SCALEMAXMAX)&&(distBack >= SCALEMINMIN)){
		out = 4; //Shift to the back outputs
		dist = distFront;
	}
	if(dist == 0)
		return 0;//No scale

	//Check dist height
	if ((dist <= (SCALESTART+0.05))&&(dist >= (SCALESTART - 0.05))) //0.05 = 2"
		return out+1;
	if ((dist <= SCALEMAXMAX)&&(dist >= SCALESTART))
		return out;
	if ((dist <= SCALESTART)&&(dist >= SCALEMINMIN))
		return out+2;

	//Still here? Thats not good!
	return 7;
}


double ScaleDetection::ReadFront() {
	double period = cntFront->GetPeriod();
	//convert to us
	period *= 1000000;
	//Calculate the distance
	frc::SmartDashboard::PutNumber("Period", period);
	return ((period * 0.00278) + 0.26388);
}

double ScaleDetection::ReadBack() {
	double period = cntBack->GetPeriod();
	//convert to us
	period *= 1000000;
	//Calculate the distance
	return ((period * 0.00278) + 0.26388);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.

