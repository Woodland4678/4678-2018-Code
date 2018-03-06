// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "Robot.h"
#include <sstream>

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION
std::shared_ptr<DriveTrain> Robot::driveTrain;
std::shared_ptr<Lidar> Robot::lidar;
std::shared_ptr<ManipulatorArm> Robot::manipulatorArm;
std::shared_ptr<ScaleDetection> Robot::scaleDetection;
std::shared_ptr<Intake> Robot::intake;
std::unique_ptr<OI> Robot::oi;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INITIALIZATION

void Robot::RobotInit() {
	RobotMap::init();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    driveTrain.reset(new DriveTrain());
    lidar.reset(new Lidar());
    manipulatorArm.reset(new ManipulatorArm());
    scaleDetection.reset(new ScaleDetection());
    intake.reset(new Intake());

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
	//fclose(data);
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
	//boundaryCheck = new frc::Relay(0);
	//boundaryCheck->Set(frc::Relay::Value::kOff);
	cnt = 0;

	/*data = fopen("/tmp/dataCollect.txt","a");
	if(!data)
		frc::SmartDashboard::PutString("File Status", "File Not Found!");
	frc::SmartDashboard::PutString("File Status", "File Found!");*/

	done = false;
	caseThing = 0;

	if (autonomousCommand != nullptr)
		autonomousCommand->Cancel();

	Robot::lidar->Start();
}

void Robot::TeleopPeriodic() {
	frc::Scheduler::GetInstance()->Run();
	cnt++;
	//Robot Checks
	manipulatorArm->updateArm();
	/*if ((manipulatorArm->endEffectorX > 32))
		{
		//std::stringstream  text_stream;
		//text_stream << "Position: " << manipulatorArm->targetPos << "\n";
		//text_stream << "Previous Position: " << manipulatorArm->prevPos << "\n\n";
		//fwrite(text_stream.str().c_str(),1, text_stream.str().length(), data);

		//Turn on relay
		boundaryCheck->Set(frc::Relay::Value::kForward);
		cnt = 0;
		}
	if (manipulatorArm->endEffectorX < -32)
	{
		boundaryCheck->Set(frc::Relay::Value::kReverse);
		cnt = 0;
	}
	if(cnt > 50)
		boundaryCheck->Set(frc::Relay::Value::kOff);
	*/
//	if (oi->getdriver()->GetRawButton(1)) {
//
//		//driveTrain->goToDistance(548, 548, 1);
//		//intake->grab();
//
//		double p = frc::SmartDashboard::GetNumber("p factor",1);
//		double i = frc::SmartDashboard::GetNumber("i factor",0);
//		double d = frc::SmartDashboard::GetNumber("d factor",0);
//		double zone = frc::SmartDashboard::GetNumber("zone factor",10);
//
//		manipulatorArm->setShoulderPID(p, i, d);
//		frc::SmartDashboard::PutString("PID", "Changed");
//	}
//	else
//		frc::SmartDashboard::PutString("PID", "Same");
//	manipulatorArm->updateArm();
//	if (oi->getoperate()->GetRawButton(5) && !done) {
//		double joyY = oi->getoperate()->GetRawAxis(1);
//		if (cnt < 5)
//			manipulatorArm->fineMovement(joyY, 0);
//		if(cnt > 20)
//			cnt = 0;
//	}
//	if (oi->getdriver()->GetRawButton(9)) {
//		intake->setRightSpeed(0);
//		intake->setLeftSpeed(0);
//	}
//	if (oi->getdriver()->GetRawButton(4)) {
//		intake->setRightSpeed(0.5);
//		intake->setLeftSpeed(0.5);
//	}
//	if (oi->getdriver()->GetRawButton(7)) {
//		manipulatorArm->squeeze();
//	}
//	if (oi->getdriver()->GetRawButton(8)) {
//		manipulatorArm->release();
//	}
//	if (oi->getdriver()->GetRawButton(1)) {
//		intake->grab();
//	}
//	if (oi->getdriver()->GetRawButton(4)) {
//		intake->release();
//	}
	/*if (oi->getdriver()->GetRawButton(4)) {
		Robot::manipulatorArm->release();
	}
	if (oi->getdriver()->GetRawButton(3)) {
		Robot::manipulatorArm->squeeze();
	}*/


	/*if ((oi->getdriver()->GetRawButton(1)) && (!done)) { //hold button down
		switch(caseThing){
			case 0:
				Robot::manipulatorArm->initMovement();
				caseThing++;
				break;
			case 1:
				done = Robot::manipulatorArm->pickUpCube();
				break;
		}

	}*/
}



























START_ROBOT_CLASS(Robot);

