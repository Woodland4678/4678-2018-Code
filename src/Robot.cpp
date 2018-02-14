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
std::shared_ptr<SmallLidars> Robot::smallLidars;
std::shared_ptr<ManipulatorArm> Robot::manipulatorArm;
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
    smallLidars.reset(new SmallLidars());
    manipulatorArm.reset(new ManipulatorArm());

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
	ahrs = new AHRS(SPI::Port::kMXP);

	frc::SmartDashboard::PutNumber("p factor", 0.02);
	frc::SmartDashboard::PutNumber("i factor", 0);
	frc::SmartDashboard::PutNumber("d factor", 0);


	done = false;
	if (autonomousCommand != nullptr)
		autonomousCommand->Cancel();

	Robot::lidar->Start();
}

void Robot::TeleopPeriodic() {
	frc::Scheduler::GetInstance()->Run();
	//frc::SmartDashboard::PutNumber("LidarFrequency", Robot::lidar->getFrequency());
	//int pwm = frc::SmartDashboard::GetNumber("LidarPWM", 660);
	//frc::SmartDashboard::PutNumber("LeftEncoder", Robot::driveTrain->getLeftEncoder()->Get());
	//frc::SmartDashboard::PutNumber("RightEncoder", Robot::driveTrain->getRightEncoder()->Get());
	//frc::SmartDashboard::PutNumber("LeftPower", -RobotMap::driveTrainLeftMotor->Get());
	//frc::SmartDashboard::PutNumber("RightPower", RobotMap::driveTrainRightMotor->Get());
	//Robot::lidar->setMotorSpeed(pwm);
	//printf("Gyro: %f\n", ahrs->GetAngle());
	static double target = ahrs->GetAngle();

	if (oi->driver->GetRawButton(1) && !done) {

		double p = frc::SmartDashboard::GetNumber("p factor",0.02);
		double i = frc::SmartDashboard::GetNumber("i factor",0);
		double d = frc::SmartDashboard::GetNumber("d factor",0);

		done = driveTrain->GyroTurn(ahrs->GetAngle(), target + 90, p, i, d);

		frc::SmartDashboard::PutNumber("Angle", ahrs->GetAngle());
	}
	if (oi->driver->GetRawButton(2)) {
		done = false;
		target = ahrs->GetAngle();
	}

}

START_ROBOT_CLASS(Robot);

