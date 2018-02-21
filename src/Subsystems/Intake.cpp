// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "ctre/Phoenix.h"


#include "Intake.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

Intake::Intake() : frc::Subsystem("Intake") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    lifter = RobotMap::intakelifter;
    rightWheels = RobotMap::intakerightWheels;
    leftWheels = RobotMap::intakeleftWheels;
    releaser = RobotMap::intakereleaser;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    double startPosition;
    lifter->EnableCurrentLimit(true);
	lifter->ConfigContinuousCurrentLimit(1, 0);
	lifter->ConfigPeakCurrentLimit(0, 0);
	lifter->Config_kP(0, 1, 0);
	lifter->Config_kI(0, 0, 0);
	lifter->Config_kD(0, 0, 0);
	lifter->ConfigAllowableClosedloopError(0, 20, 0);
	lifter->ConfigSelectedFeedbackSensor(FeedbackDevice::PulseWidthEncodedPosition, 0, 0);
	lifter->SetStatusFramePeriod(StatusFrame::Status_1_General_, 10, 0);
	startPosition = (getLifterAngular() % 4096);
	if (startPosition < 0)
		startPosition += 4096;
	lifter->GetSensorCollection().SetPulseWidthPosition(startPosition, 0);
	lifter->GetSensorCollection().SetQuadraturePosition(startPosition, 0);
}

void Intake::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}

void Intake::Periodic() {
    // Put code here to be run every loop

}

void Intake::getCude(){

}

void Intake::setLifterPID(double p, double i, double d)
	{
	lifter->Config_kP(0, p, 0);
	lifter->Config_kI(0, i, 0);
	lifter->Config_kD(0, d, 0);
	}

int Intake::getLifterAngular()
	{
	frc::SmartDashboard::PutNumber("lifter Current", lifter->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("lifter Voltage", lifter->GetBusVoltage());
	frc::SmartDashboard::PutNumber("lifter Quad", lifter->GetSensorCollection().GetQuadraturePosition());
	return lifter->GetSensorCollection().GetPulseWidthPosition();
	}

void Intake::setLifterPosition(double position)
	{
	lifter->Set(ControlMode::Position, position);
	}

void Intake::release(){
	releaser->Set(false);
}

void Intake::grab(){
	releaser->Set(true);
}

void Intake::setRightSpeed(double power){
	rightWheels->Set(power);
}

void Intake::setLeftSpeed(double power){
	leftWheels->Set(power);
}
int Intake::getLifterError(){
	return lifter->GetClosedLoopError(0);
}
void Intake::stopWheels(){
	setRightSpeed(0);
	setLeftSpeed(0);
}
void Intake::lowerIntake(){
	setLifterPosition(6410);
	if (lifter->GetClosedLoopError(0) < 100) {
		setRightSpeed(-0.5);
		setLeftSpeed(-0.5);
	} else {
		setRightSpeed(0);
		setLeftSpeed(0);
	}
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

