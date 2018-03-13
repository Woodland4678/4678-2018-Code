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
#include "../Robot.h"
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

	const double INTAKE_POWER = 0.5;

    double startPosition;
    lifter->EnableCurrentLimit(false);
    lifter->ConfigVoltageCompSaturation(10, 0);
    lifter->EnableVoltageCompensation(true);
	lifter->Config_kP(0, 2, 0);
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

	posOffset = 4.25;
	multOffset = (12 + (posOffset - 4)) * 1000;

	//TODO:: Change the x,y to be relative to the shoulder location
	lifterSeg.posX = 12.5;
	lifterSeg.posY = -4.875;
	lifterSeg.length = 18.68; //inches
	lifterSeg.convSlope = -0.033161385;
	lifterSeg.convIntercept = 198.96831245;

	//Status
	Status.position = IntakePositions::InRobot;
	Status.Gribber = IntakeGribber::Open;
	Status.WheelDirection = IntakeWheelDirection::DirectionStopped;
	Status.WheelSpeed = WHEELSPEEDSTOPPED;

	positions[0] = 572;
	positions[1] = 6000;
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

bool Intake::moveTo(int position)
	{
	frc::SmartDashboard::PutNumber("Intake Case", moveCase);
	switch(moveCase)
		{
		case 0:
			initMovement();
			stopWheels();
			Status.position = 2; //means we are moving
			moveCase=1;
			break;
		case 1:
			{
			double currTime = frc::Timer::GetFPGATimestamp() - origTimeStampintake;

			//Check if safe!
			Robot::manipulatorArm->updateArm();
			if((Robot::manipulatorArm->wristSeg.posX > 0) && (Robot::manipulatorArm->wristSeg.posY < 14))
				{
				moveCase = 2; //Lets just not move until the arm is out of the way
				return false;
				if((Robot::manipulatorArm->wristSeg.posX > 0) && (Robot::manipulatorArm->wristSeg.posY < 14))
					{
					//moveCase = 3;
					return true;
					}
				}

			if(lifterGoToPosition(lifterStartPos,positions[position],currTime,1))
				moveCase = 2;

			break;
			}
		case 2:
			Status.position = position;
			if(position == IntakePositions::GetCube)
				spinForward(WHEELSPEEDHIGH);
			return true;
			break;
		}
	return false;
	}

void Intake::initMovement()
	{
	origTimeStampintake = frc::Timer::GetFPGATimestamp();
	lifterStartPos = lifter->GetSensorCollection().GetPulseWidthPosition();
	moveDone = false;
	moveCase = 0;
	moveOverridePos = 0;
	moveOverride = false;
	}

bool Intake::lifterGoToPosition(double start, double position, double current, double time)
	{
	//multOffset = (Sharpness + (offset - 4)) * 1000;
	double multiplier = (multOffset / (time*1000));
	//Calculate the new set point
	double NewSet = sigmod(position, start, multiplier, posOffset, current);

	setLifterPosition(NewSet);

	//Return true if the elapse time is complete
	if (current > time)
		return true;
	return false;
	}

double Intake::sigmod(double end, double start, double mult, double offset, double x)
	{
	return (((end-start)/(1+exp((-(mult*x))+offset)))+start);
	}

double Intake::invSigmod(double end, double start, double mult, double offset, double pnt)
    {
    return ((std::log(((end-start)/(pnt-start)) - 1) - offset)/(-mult));
    }

int Intake::checkPosition()
	{
	Status.position = 2; //Unknown
	if(std::abs(getLifterAngular() - 6000) < 500)
		Status.position = IntakePositions::GetCube;
	if(std::abs(getLifterAngular() - 572) < 500)
		Status.position = IntakePositions::InRobot;
	return Status.position;
	}

void Intake::updateEndEffector()
	{
	lifterSeg.encValue = lifter->GetSensorCollection().GetPulseWidthPosition();
	frc::SmartDashboard::PutNumber("Intake Encoder", lifterSeg.encValue);
	lifterSeg.relAngle = convertEncoderToRelAngle(&lifterSeg, lifterSeg.encValue);
	lifterSeg.absAngle = lifterSeg.relAngle;
	frc::SmartDashboard::PutNumber("Intake Angle", lifterSeg.absAngle);

	endEffectorX = lifterSeg.posX + lifterSeg.length * cos(lifterSeg.absAngle * (M_PI/180));
	endEffectorY = lifterSeg.posY + lifterSeg.length * sin(lifterSeg.absAngle * (M_PI/180));

	frc::SmartDashboard::PutNumber("Intake End X", endEffectorX);
	frc::SmartDashboard::PutNumber("Intake End Y", endEffectorY);
	}

double Intake::convertEncoderToRelAngle(tpArmSegment *Seg, double encoder)
	{
	return (encoder * Seg->convSlope + Seg->convIntercept);
	}
double Intake::convertRelAngleToEncoder(tpArmSegment *Seg, double angle)
	{
	return (((angle) - (Seg->convIntercept))/(Seg->convSlope));
	}


bool Intake::release(){
	Status.Gribber = IntakeGribber::Open;
	releaser->Set(false);
	return true;
}

bool Intake::grab(){
	if(checkPosition() != IntakePositions::GetCube)
		return false;
	Status.Gribber = IntakeGribber::Close;
	releaser->Set(true);
	return true;
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
	Status.WheelSpeed = IntakeWheelSpeed::SpeedStopped;
	Status.WheelDirection = IntakeWheelDirection::DirectionStopped;
	setRightSpeed(0);
	setLeftSpeed(0);
}

bool Intake::spinForward(double power) {
	if(checkPosition() != IntakePositions::GetCube)
		return false;
	Status.WheelSpeed = power;
	Status.WheelDirection = IntakeWheelDirection::DirectionIn;
	setRightSpeed(-power);
	setLeftSpeed(-power);
	return true;
}

bool Intake::spinReverse(double power) {
	if(Status.position != IntakePositions::GetCube)
		return false;
	Status.WheelSpeed = power;
	Status.WheelDirection = IntakeWheelDirection::DirectionOut;
	setRightSpeed(power);
	setLeftSpeed(power);
	return true;
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

