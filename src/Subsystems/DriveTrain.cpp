// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "DriveTrain.h"
#include "../RobotMap.h"
#include "../Commands/DriveRobot.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

DriveTrain::DriveTrain() : frc::Subsystem("DriveTrain") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    rightMotor = RobotMap::driveTrainrightMotor;
    leftMotor = RobotMap::driveTrainleftMotor;
    rightEncoderDrive = RobotMap::driveTrainrightEncoderDrive;
    leftEncoderDrive = RobotMap::driveTrainleftEncoderDrive;
    compressor = RobotMap::driveTrainCompressor;
    shoulderClimber = RobotMap::driveTrainshoulderClimber;
    shifter = RobotMap::driveTrainshifter;
    pdp = RobotMap::driveTrainpdp;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    encoderClicksPerCentimeter = 30; // 7.2;
    goToDistanceState = 0;
    highGear = true;
}

void DriveTrain::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

        SetDefaultCommand(new DriveRobot());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}

void DriveTrain::Periodic() {
    // Put code here to be run every loop

}

void DriveTrain::readLeftMotor()
	{

	}

void DriveTrain::SetRightPower(double power) {
	rightMotor->Set(power);
}

void DriveTrain::SetLeftPower(double power) {
	leftMotor->Set(-power);
}

std::shared_ptr<frc::Encoder> DriveTrain::getLeftEncoder() {
	return leftEncoderDrive;
}

std::shared_ptr<frc::Encoder> DriveTrain::getRightEncoder() {
	return rightEncoderDrive;
}

bool DriveTrain::goToDistance(double rightCentimeters, double leftCentimeters, double power) {
	static int  s_init_left_encoder(0);
	static int  s_init_right_encoder(0);

	if (goToDistanceState == 0) {
		calculator.reset(new DriveMotorCalculator(getLeftEncoder()->Get(), getRightEncoder()->Get(), leftCentimeters, rightCentimeters, encoderClicksPerCentimeter));

		calculator->setStartUpPower(0.25);
		calculator->setTravelPower(0.9);
		calculator->setRampDownPower(0.15);

		std::string   dump_str(calculator->dumpObject());
		printf("%s\n", dump_str.c_str());

		 s_init_left_encoder  = getLeftEncoder()->Get();
		 s_init_right_encoder = getRightEncoder()->Get();

		goToDistanceState = 1;
		return false;
	} else if (goToDistanceState == 1) {
		float left_motor_power;
		float right_motor_power;

		bool done = calculator->getMotorSpeeds(left_motor_power, right_motor_power, getLeftEncoder()->Get(), getRightEncoder()->Get());
        int  left_encoder_clicks(getLeftEncoder()->Get() - s_init_left_encoder);
        int  right_encoder_clicks(getRightEncoder()->Get() - s_init_right_encoder);

		printf("leftPW: %f, leftEncGet: %i, leftPulses=%d, rightPW: %f, rightEnc: %i, rightPulses=%d\n",
				left_motor_power, getLeftEncoder()->Get(), left_encoder_clicks,
				right_motor_power, getRightEncoder()->Get(), right_encoder_clicks);
		SetRightPower(right_motor_power);
		SetLeftPower(left_motor_power);
		if (done) {
			goToDistanceState = 0;
		}
		return done;
	}
	return false;
}

bool DriveTrain::GyroTurn(double current, double turnAmount, double p, double i, double d){

	static double past = 0;
	static double iValue = 0;
	double error = turnAmount - current;
	double pValue = p*error;
	iValue += i*(error);
	double dValue = d*(past - current);
	double totalValue = pValue + iValue + dValue;

	printf("Gyro: %f  tv = %f\n",current, totalValue);

	if(totalValue > 1)
		totalValue = 0.8;
	if(totalValue < -1)
		totalValue = -0.8;

	SetRightPower(-totalValue);
	SetLeftPower(totalValue);


	past = current;
	if (std::abs(error) < 1) {
		past = 0;
		iValue = 0;
		SetRightPower(0);
		SetLeftPower(0);
		return true;
	}
	return false;
}
void DriveTrain::enableCompressor() {
	compressor->Enabled();
}
void DriveTrain::shiftUp() {
	highGear = true;
	shifter->Set(true);
}
void DriveTrain::shiftDown() {
	highGear = false;
	shifter->Set(false);
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

