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


#include "ManipulatorArm.h"
#include "../Commands/moveArm.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

ManipulatorArm::ManipulatorArm() : frc::Subsystem("ManipulatorArm") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    shoulder = RobotMap::manipulatorArmshoulder;
    elbow = RobotMap::manipulatorArmelbow;
    wrist = RobotMap::manipulatorArmwrist;
    shoulderSlave = RobotMap::manipulatorArmshoulderSlave;
    elbowSlave = RobotMap::manipulatorArmelbowSlave;
    claw = RobotMap::manipulatorArmclaw;
    indicator = RobotMap::manipulatorArmindicator;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    shoulderNewSet = 0;

    //Initialize the talons here:
    double startPosition;
    //Shoulder
    shoulderSlave->Follow(*shoulder);
    shoulder->EnableCurrentLimit(false);
    shoulder->ConfigVoltageCompSaturation(12, 0);
    shoulder->EnableVoltageCompensation(true);
    setShoulderPID(4,0,0,0,0);
	shoulder->ConfigAllowableClosedloopError(0, 5, 0);
	shoulder->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
	shoulder->SetStatusFramePeriod(StatusFrame::Status_1_General_, 10, 0);
    startPosition = (shoulder->GetSensorCollection().GetPulseWidthPosition() % 4096);
	if (startPosition < 0)
		startPosition += 4096;
	shoulder->GetSensorCollection().SetPulseWidthPosition(startPosition, 0);
	shoulder->GetSensorCollection().SetQuadraturePosition(startPosition, 0);
	
	int pulse = shoulder->GetSensorCollection().GetPulseWidthPosition();
	double enc = getShoulderAngular();
	if (std::abs(pulse - enc) < 100)
		frc::SmartDashboard::PutBoolean("Shoulder Encoders", true);
	else
		frc::SmartDashboard::PutBoolean("Shoulder Encoders", false);

	shoulderSeg.posX = 0;
	shoulderSeg.posY = 0;
	shoulderSeg.length = 37; //inches
	shoulderSeg.convSlope = 0.008594429939077;
	shoulderSeg.convIntercept = 102.68635770235;
	shDegreePerSecond = 50;
	shMinTime = 1;
	updateShoulder();
	if(std::abs(shoulderSeg.absAngle - 109) < 15)
		frc::SmartDashboard::PutBoolean("Shoulder Angle", true);
	else
		{
		//Lets try again!

		frc::SmartDashboard::PutBoolean("Shoulder Angle", false);
		}
    //Elbow
	elbowSlave->Follow(*elbow);
	elbow->SelectProfileSlot(0,0);
	elbow->ConfigVoltageCompSaturation(12, 0);
	elbow->EnableVoltageCompensation(false);
	elbow->EnableCurrentLimit(false);
	setElbowPID(3,0,0,0,0);
	elbow->ConfigAllowableClosedloopError(0, 5, 0);
	elbow->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
	elbow->SetStatusFramePeriod(StatusFrame::Status_1_General_, 10, 0);
    startPosition = ((getElbowAngular()) % 4096);
	if (startPosition < 0)
		startPosition += 4096;
	elbow->SetSelectedSensorPosition(startPosition, 0, 0);
	
	elbowSeg.posX = 0;
	elbowSeg.posY = 37;
	elbowSeg.length = 40; //inches
	elbowSeg.convSlope = -0.023020774;
	elbowSeg.convIntercept = -159.2 - getElbowAngular()*elbowSeg.convSlope ;
	elDegreePerSecond = 90;
	elMinTime = 2;
	updateElbow();
	
	if(std::abs(elbowSeg.absAngle - (-50.2)) < 15)
		frc::SmartDashboard::PutBoolean("Elbow Angle", true);
	else
		frc::SmartDashboard::PutBoolean("Elbow Angle", false);

    //Wrist
	wrist->EnableCurrentLimit(false);
	wrist->ConfigVoltageCompSaturation(12, 0);
	wrist->EnableVoltageCompensation(true);
	setWristPID(2,0,0,0,0);
	wrist->ConfigAllowableClosedloopError(0, 5, 0);
	wrist->ConfigSelectedFeedbackSensor(FeedbackDevice::PulseWidthEncodedPosition, 0, 0);
	wrist->SetStatusFramePeriod(StatusFrame::Status_1_General_, 10, 0);
    startPosition = (getWristAngular() % 4096);
	if (startPosition < 0)
		startPosition += 4096;
	wrist->GetSensorCollection().SetPulseWidthPosition(startPosition, 0);
	
	wristSeg.posX = 0;
	wristSeg.posY = 77;
	wristSeg.length = 6; //inches
	wristSeg.convSlope = 0.037453761;
	wristSeg.convIntercept = -56.25369914;
	wrDegreePerSecond = 90;
	wrMinTime = 1.5;
	updateWrist();
	
	if(std::abs(wristSeg.absAngle - 0) < 15)
			frc::SmartDashboard::PutBoolean("Wrist Angle", true);
		else
			frc::SmartDashboard::PutBoolean("Wrist Angle", false);

	posOffset = 4.25;
	multOffset = (12 + (posOffset - 4)) * 1000;
	
	indicator->Set(frc::Relay::Value::kOff);
	lightCounter = 0;
	lightCounter2 = 0;
	lightShowType = 0;

	//Positions:
	//Home Position
	positions[0][0] = 109;   //e:624     r:109
    positions[0][1] = -54; //e:2914    r:-162.5
    positions[0][2] = 0;     //e:2970    r:55
    
    //Scale front Max
    positions[1][0] = 87;    //e:-2634   r:87
    positions[1][1] = 75;    //e:-3673   r:-12
    positions[1][2] = 15;    //e:-99     r:-60
    
    //Scale front Min
    positions[2][0] = 116;   //e:1660    r:116
    positions[2][1] = 44.5;  //e:-1096   r:-71
    positions[2][2] = 14;    //e:687     r:-30
    
    //Scale back Max
    positions[3][0] = 93;    //e:-1745   r:93
    positions[3][1] = 105;   //e:-4714   r:12 
    positions[3][2] = 165;   //e:3103    r:60
    
    //scale back min
    positions[4][0] = 64;    //e:-6042   r:64
    positions[4][1] = 135;   //e:-7272   r:71 
    positions[4][2] = 165;   //e:2303    r:30
    
    //Pick up cube
    positions[5][0] = 94;    //e:-1745   r:93
    positions[5][1] = -54;   //e:2478    r:-154
    positions[5][2] = -90;   //e:754     r:-28
    
    //Pick up cube middle
    positions[6][0] = 98;    //e:-2784   r:86
    positions[6][1] = -50;   //e:2134    r:-146
    positions[6][2] = 21;    //e:728     r:-29
    
    //Switch Front
    positions[7][0] = 123;   //e:2697    r:123
    positions[7][1] = -11;   //e:1614    r:-134
    positions[7][2] = 19;    //e:2302    r:30
    
    //Switch Back
    positions[8][0] = 57;    //e:-7080   r:123
    positions[8][1] = 191;   //e:-10001  r:134
    positions[8][2] = 162;   //e:728     r:-30
    
    //Grab climber
    positions[9][0] = 71;    //e:-5007   r:71
    positions[9][1] = 227;   //e:-10957  r:155
    positions[9][2] = 270;   //e:2649    r:43
    
    //Place climber
    positions[10][0] = 99;   //e:-859    r:99
    positions[10][1] = 98;   //e:-4154   r:0
    positions[10][2] = 180;  //e:3691    r:82
    
    //Carry
    positions[11][0] = 127;  //e:3291    r:127
    positions[11][1] = -14;  //e:2479    r:-154
    positions[11][2] = 71;   //e:3851    r:88

    currPos = 0;
    targetPos = 0;
    prevPos = 0;
}

void ManipulatorArm::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

        SetDefaultCommand(new moveArm(0));

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}

void ManipulatorArm::Periodic() {
    // Put code here to be run every loop
	lightCounter++;
	frc::SmartDashboard::PutNumber("Light Type", lightShowType);
	frc::SmartDashboard::PutNumber("Light cnt 1", lightCounter);
	frc::SmartDashboard::PutNumber("Light cnt 2", lightCounter2);
	switch(lightShowType)
		{
		case 0:
			indicator->Set(frc::Relay::Value::kOff);
			lightCounter2 = 0;
			lightCounter = 0;
			break;
		case 1: //Outside boundary
			if(lightCounter < 10)
				indicator->Set(frc::Relay::Value::kForward);
			if(lightCounter > 10)
				indicator->Set(frc::Relay::Value::kReverse);
			if(lightCounter > 20)
				{
				lightCounter2++;
				lightCounter = 0; //Start over again
				}
			if(lightCounter2 > 10)
				{
				lightShowType = 0;
				lightCounter2 = 0;
				}
			break;
		case 2: //Near Boundary
			indicator->Set(frc::Relay::Value::kForward);
			if(lightCounter > 50)
				lightShowType = 0;
			break;
		}
}

void ManipulatorArm::initMovement(){
	origTimeStamp = frc::Timer::GetFPGATimestamp();
	updateShoulder();
	updateElbow();
	updateWrist();
	
	shoulderStartPos = shoulderSeg.absAngle;
	elbowStartPos = elbowSeg.absAngle;
	wristStartPos = wristSeg.absAngle;
	shoulderMovement = false;
	shoulderMovement2 = false;
	shoulderMovement3 = false;
	elbowMovement = false;
	wristMovement = false;
	moveCase = 0;
	shTarget1 = 0;
    shTarget2 = 0;
	shTime = 0;
	elTime = 0;
	wrTime = 0;
    shTime1 = 0;
    shTime2 = 0;
}

bool ManipulatorArm::fineMovement(double joyX, double joyY)
	{
	switch(fineMoveCase)
		{
		case 0:
			{
			//Get current end point
			double elbPosX = shoulderSeg.length * std::cos(positions[currPos][0] * (M_PI/180));
			double elbPosY = shoulderSeg.length * std::sin(positions[currPos][0] * (M_PI/180));

			double currX = elbPosX + elbowSeg.length * std::cos(positions[currPos][1] * (M_PI/180));
			double currY = elbPosY + elbowSeg.length * std::sin(positions[currPos][1] * (M_PI/180));
			//Calculate Target Position
			double tarX = std::abs(currX);
			double tarY = std::abs(currY);

			tarX += FINECONTROLBOXSIZE * joyX;
			tarY += FINECONTROLBOXSIZE * joyY;

			frc::SmartDashboard::PutNumber("Target X", tarX);
			frc::SmartDashboard::PutNumber("Target Y", tarY);

			//Check limits
			if(tarX > 27)
				tarX = 27;
			if(tarX < -27)
				tarX = -27;
			if(tarY > 77)
				tarY = 77;
			if(tarY < 0)
				tarY = 0;
			if((tarX < 1)&&(tarY > 0))
				tarX = 1;

			//Inverse Kitematics
			//Elbow
			double casBrakets = ((std::pow(tarX,2) + std::pow(tarY,2)) - std::pow(37,2) - std::pow(40,2))/(-2*(37)*(40));
			//Check value incase it is too far
			if((casBrakets < -1)||(casBrakets > 1))
				casBrakets = std::round(casBrakets);
			frc::SmartDashboard::PutNumber("El cos Braket", casBrakets);
			double angElbow = M_PI - std::acos(casBrakets);
			frc::SmartDashboard::PutNumber("El radians", angElbow);
			//Shoulder
			double angShoulder = std::atan(tarY / tarX) - std::atan((40 * std::sin(-angElbow))/(37 + 40 * std::cos(-angElbow)));
			frc::SmartDashboard::PutNumber("Sh radians", angShoulder);
			//Convert radians to degrees
			elbowDeg = (angElbow * 180) / M_PI;
			ShoulderDeg = (angShoulder * 180) / M_PI;

			//Check for negative
			if((tarX) < 0)
				{
				ShoulderDeg = (90 - std::abs(ShoulderDeg)) + 90;
				elbowDeg *= -1;
				}

			if(ShoulderDeg > 127)
				ShoulderDeg = 127;
			if(ShoulderDeg < 48)
				ShoulderDeg = 48;
			if(elbowDeg > 164)
				elbowDeg = 164;
			if(elbowDeg < -164)
				elbowDeg = -164;

			elbowDeg -= ShoulderDeg;
			elbowDeg *= -1;
			frc::SmartDashboard::PutNumber("Shoulder Degree", ShoulderDeg);
			frc::SmartDashboard::PutNumber("Elbow Degree", elbowDeg);

			//Lets just do some quick tests to make sure we aren't attempting to move
			//	too far too quickly
			if (std::abs(ShoulderDeg - shoulderSeg.absAngle) > 20)
				moveCase = 2;
			if (std::abs(elbowDeg - elbowSeg.absAngle) > 20)
				moveCase = 2;

			//How much time?
			shTime = 1;
			elTime = 1;

			fineMoveCase++;
			break;
			}
		case 1:
			{
			//updateArm();
			/*double currTime = frc::Timer::GetFPGATimestamp() - origTimeStamp;

			if (!shoulderMovement)
				shoulderMovement = shoulderGoToPosition(shoulderStartPos,ShoulderDeg,currTime,shTime);
			if (!elbowMovement)
				elbowMovement = elbowGoToPosition(elbowStartPos,elbowDeg,currTime,elTime);
			//if (!wristMovement)
			//	wristMovement = wristGoToPosition(wristStartPos,positions[pos][2],currTime,wrTime);

			//Movement Check
			if(shoulderMovement && elbowMovement)
				{
				fineMoveCase++;
				return true;
				}*/
			}
			break;
		}
	return false;
	}

bool ManipulatorArm::moveTo(int pos, double addShTime, double addElTime)
	{
	switch(moveCase)
		{
		case 0:
			{
			targetPos = pos;
			//Pre calculations here, timing and shoulder movements
			//Timing
			wrTime = (std::abs(wristStartPos - positions[pos][2])) / wrDegreePerSecond;
			elTime = (std::abs(elbowStartPos - positions[pos][1])) / elDegreePerSecond;
			shTime = (std::abs(shoulderStartPos - positions[pos][0])) / shDegreePerSecond;

			if(shTime < shMinTime)
				shTime = shMinTime;
			if(elTime < elMinTime)
				elTime = elMinTime;
			if(wrTime < wrMinTime)
				wrTime = wrMinTime;
			
			shTime += addShTime;
			elTime += addElTime;

			//Extra shoulder positions
			//We have to determine how the arm needs to moves to stay within the 16"
			//Does the elbow move from one zone to another?
			//Current zone
			int currentZone = 1;
			int targetZone = 1;
			if(elbowStartPos <= 0)
				currentZone = 0;
			if(elbowStartPos >= 180)
				currentZone = 2;

			if(positions[pos][1] <= 0)
				targetZone = 0;
			if(positions[pos][1] >= 180)
				targetZone = 2;

			if(currentZone != targetZone)
				{
				bool twoZones = false;
				bool rTl = true;
				shoulderMovement2 = true;
				elTime += 0.25;
				float mult = (multOffset / (elTime*1000));
				if((std::abs(targetZone - currentZone)) > 1)
					{
					twoZones = true;
					//add a second to each?
					shTime += 0.9;
					elTime += 0.7;
					wrTime += 0.9;
					shoulderMovement2 = false;
					}
				if(currentZone != 1)
					{
					if(currentZone > targetZone)
					rTl = false;
					}
				else
					{
					if(currentZone < targetZone)
					rTl = false;
					}

				if(rTl)
					{
					//abs 90 first
					if(shoulderStartPos >= 118)
						shTarget1 = shoulderStartPos;
					else
						{
						if(positions[pos][0] > 118)
							shTarget1 = positions[pos][0];
						else
							shTarget1 = 118;
						}
					shTime1 = invSigmod(positions[pos][1], elbowStartPos, mult,posOffset,0);

					//abs -90 second
					if(twoZones)
						{
						if(positions[pos][0] > 62)
							{
							if(positions[pos][0] < 62)
								shTarget2 = positions[pos][0];
							else
								shTarget2 = 62;
							shTime2 = invSigmod(positions[pos][1], elbowStartPos, mult,posOffset,180);
							shTime1 += 0.1;
							//Rounding
							shTime2 *= 100;
							shTime2 = (std::floor(shTime2)) / 100;
							}
						else
							shoulderMovement2 = true;
						}
					}
				else
					{
					//abs -90 first
					if(shoulderStartPos <= 67)
						shTarget1 = shoulderStartPos;
					else
						{
						if(positions[pos][0] < 67)
							shTarget1 = positions[pos][0];
						else
							shTarget1 = 67;
						}
					shTime1 = invSigmod(positions[pos][1], elbowStartPos, mult,posOffset,180);

					//abs 90 second
					if(twoZones)
						{
						if(positions[pos][0] < 113)
							{
							if(positions[pos][0] > 113)
								shTarget2 = positions[pos][0];
							else
								shTarget2 = 113;
							shTime2 = invSigmod(positions[pos][1], elbowStartPos, mult,posOffset,0);
							shTime1 += 0.1;
							//Rounding
							shTime2 *= 100;
							shTime2 = (std::floor(shTime2)) / 100;
							}
						else
							shoulderMovement2 = true;
						}
					}
				}
			else
				{
				shTime += 0.4;
				shoulderMovement2 = true;
				shoulderMovement3 = true;
				}

			moveCase += 1;
			}
			break;
		case 1:
			{
			updateArm();
			double currTime = frc::Timer::GetFPGATimestamp() - origTimeStamp;

			frc::SmartDashboard::PutNumber("Shoulder Time", shTime);
			frc::SmartDashboard::PutNumber("Elbow Time", elTime);
			frc::SmartDashboard::PutNumber("Wrist Time", wrTime);

			frc::SmartDashboard::PutNumber("Current Time", currTime);

			//Extra movements
			if((shTarget1 != 0) && (!shoulderMovement3))
				shoulderMovement3 = shoulderGoToPosition(shoulderStartPos,shTarget1,currTime,shTime1);//Move shoulder back
			if ((shTarget2 != 0) && shoulderMovement3 && (!shoulderMovement2))
				shoulderMovement2 = shoulderGoToPosition(shTarget1,shTarget2,currTime-shTime1,shTime2); //Move shoulder forward to 67 in what?
			
			//Normal movements
			if (shoulderMovement3 && shoulderMovement2 && (!shoulderMovement))
				{
				if (shTarget1 != 0)
					shoulderStartPos = shTarget1;
				if (shTarget2 != 0)
					shoulderStartPos = shTarget2;
				shoulderMovement = shoulderGoToPosition(shoulderStartPos,positions[pos][0],currTime-(shTime1+shTime2),shTime);
				}
			if (!elbowMovement)
				elbowMovement = elbowGoToPosition(elbowStartPos,positions[pos][1],currTime,elTime);
			if (!wristMovement)
				wristMovement = wristGoToPosition(wristStartPos,positions[pos][2],currTime,wrTime);
			
			//Movement Check
			if(shoulderMovement && elbowMovement && wristMovement)
				{
				moveCase++;
				prevPos = currPos;
				currPos = pos;
				return true;
				}
			}
			break;
		}
	return false;
	}

//***Set PID values***
void ManipulatorArm::setShoulderPID(double p, double i, double d, int zone, double f)
	{
	shoulder->Config_kF(0, f, 0);
	shoulder->Config_kP(0, p, 0);
	shoulder->Config_kI(0, i, 0);
	shoulder->Config_kD(0, d, 0);
	shoulder->Config_IntegralZone(0, zone, 0);
	}

void ManipulatorArm::setElbowPID(double p, double i, double d, int zone, double f)
	{
	elbow->Config_kF(0, f, 0);
	elbow->Config_kP(0, p, 0);
	elbow->Config_kI(0, i, 0);
	elbow->Config_kD(0, d, 0);
	elbow->Config_IntegralZone(0, zone, 0);
	}

void ManipulatorArm::setWristPID(double p, double i, double d, int zone, double f)
	{
	wrist->Config_kF(0, f, 0);
	wrist->Config_kP(0, p, 0);
	wrist->Config_kI(0, i, 0);
	wrist->Config_kD(0, d, 0);
	wrist->Config_IntegralZone(0, zone, 0);
	}

//***Get Encoder Values From Talons***
int ManipulatorArm::getShoulderAngular()
	{
	return shoulder->GetSelectedSensorPosition(0);
	}

int ManipulatorArm::getWristAngular()
	{
	return wrist->GetSensorCollection().GetPulseWidthPosition();
	}

int ManipulatorArm::getElbowAngular()
	{
	//frc::SmartDashboard::PutNumber("Elbow Current", elbow->GetOutputCurrent());
	//frc::SmartDashboard::PutNumber("Elbow Voltage", elbow->GetMotorOutputVoltage());
	return elbow->GetSelectedSensorPosition(0);
	}


//***Set Positions***
//These function send the talons a new setpoint
void ManipulatorArm::setShoulderAbsAngle(double angle)
	{
	double pos = convertRelAngleToEncoder(&shoulderSeg, angle);
	shoulder->Set(ControlMode::Position, pos);
	}
void ManipulatorArm::setElbowAbsAngle(double angle)
	{
	//Convert to relative
	double relAng = angle - shoulderSeg.absAngle;
	double pos = convertRelAngleToEncoder(&elbowSeg, relAng);
	frc::SmartDashboard::PutNumber("Elbow Set", pos);
	elbow->Set(ControlMode::Position, pos);
	}
void ManipulatorArm::setWristAbsAngle(double angle)
	{
	//Convert to relative
	double relAng = angle - elbowSeg.relAngle - shoulderSeg.absAngle;
	double pos = convertRelAngleToEncoder(&wristSeg, relAng);
	wrist->Set(ControlMode::Position, pos);
	}

void ManipulatorArm::setShoulderPosition(double position)
	{
	//shoulder->Set(ControlMode::Position, position);
	}

void ManipulatorArm::setElbowPosition(double position)
	{
	//elbow->Set(ControlMode::Position, position);
	}

void ManipulatorArm::setWristPosition(double position)
	{
	//wrist->Set(ControlMode::Position, position);
	}
void ManipulatorArm::squeeze() {
	claw->Set(frc::DoubleSolenoid::Value::kForward);
}
void ManipulatorArm::release() {
	claw->Set(frc::DoubleSolenoid::Value::kReverse);
}

//***Motion Control***
//These functions are used to get the arm to move to a given position in a certain amount of time
//This function must be called continuously at 50 times a second like the goToDistance code
//Parameters:
//	start - is the starting position of the joint
//		Note: start must remain constant throughout the movement!
//	position - The target position you want the joint to move to
//	current - is the amount of time that has past since this movement started
//		Before you start calling this function record the timestamp with this line of code: Timer::GetFPGATimestamp(); This timestamp must be constant throughout the movement
//		Now current is calculated like this: Timer::GetFPGATimestamp() - [recored timestamp from the beginning];
//	time - is the amount of time you want this motion to take in seconds

//Example say we are in TeleopPeriodic which is called 50 times a second
/*
switch(GoHere) {
	case 0:
		orgFPGA = Timer::GetFPGATimestamp(); //The timestamp at the beginning of the movement
		ShoulderStart = myShoulder->getAngular(); //Get the starting position
		GoHere++; //Go to the next state next time this function is called
		break;
	case 1:
		curTime = Timer::GetFPGATimestamp() - orgFPGA; //Calculate how much time has past
		myShoulder->goToPosition(ShoulderStart, -1000.0, curTime, 3); //Move the shoulder to -1000 in 3 seconds
		if (curTime > 3)
			GoHere++;
		break;
	case 2:
		//End of movement
		break;
}
*/
bool ManipulatorArm::shoulderGoToPosition(double start, double position, double current, double time)
	{
	//multOffset = (Sharpness + (offset - 4)) * 1000;
	double multiplier = (multOffset / (time*1000));
	//Calculate the new set point
	double NewSet = sigmod(position, start, multiplier, posOffset, current);

	//shoulderNewSet = (position - start) * current / time + start;
	setShoulderAbsAngle(NewSet);

	//Return true if the elapse time is complete
	if (current > time)
		return true;
	return false;
	}

bool ManipulatorArm::elbowGoToPosition(double start, double position, double current, double time)
	{
	//multOffset = (Sharpness + (offset - 4)) * 1000;
	double multiplier = (multOffset / (time*1000));
	//Calculate the new set point
	double NewSet = sigmod(position, start, multiplier, posOffset, current);

	//shoulderNewSet = (position - start) * current / time + start;
	setElbowAbsAngle(NewSet);

	//Return true if the elapse time is complete
	if (current > time)
		return true;
	return false;
	}

bool ManipulatorArm::wristGoToPosition(double start, double position, double current, double time)
	{
	//multOffset = (Sharpness + (offset - 4)) * 1000;
	double multiplier = (multOffset / (time*1000));
	//Calculate the new set point
	double NewSet = sigmod(position, start, multiplier, posOffset, current);

	//shoulderNewSet = (position - start) * current / time + start;
	setWristAbsAngle(NewSet);

	//Return true if the elapse time is complete
	if (current > time)
		return true;
	return false;
	}

double ManipulatorArm::sigmod(double end, double start, double mult, double offset, double x)
	{
	return (((end-start)/(1+exp((-(mult*x))+offset)))+start);
	}

double ManipulatorArm::invSigmod(double end, double start, double mult, double offset, double pnt)
    {
    return ((std::log(((end-start)/(pnt-start)) - 1) - offset)/(-mult)); 
    }	
	
//***Convert functions***
void ManipulatorArm::updateArm()
	{
	updateShoulder();
	updateElbow();
	updateWrist();
	updateEndEffector();
	}

void ManipulatorArm::updateShoulder()
	{
	shoulderSeg.encValue = getShoulderAngular();
	//Get angles
	shoulderSeg.relAngle = convertEncoderToRelAngle(&shoulderSeg, shoulderSeg.encValue);
	frc::SmartDashboard::PutNumber("Shoulder Relative", shoulderSeg.relAngle);
	shoulderSeg.absAngle = shoulderSeg.relAngle;
	frc::SmartDashboard::PutNumber("Shoulder Absolute", shoulderSeg.absAngle);
	}
void ManipulatorArm::updateElbow()
	{
	elbowSeg.encValue = getElbowAngular();
	frc::SmartDashboard::PutNumber("Elbow Encoder", elbowSeg.encValue);
	//Get angles
	elbowSeg.relAngle = convertEncoderToRelAngle(&elbowSeg, elbowSeg.encValue);
	frc::SmartDashboard::PutNumber("Elbow Relative", elbowSeg.relAngle);
	elbowSeg.absAngle = elbowSeg.relAngle + shoulderSeg.absAngle;
	frc::SmartDashboard::PutNumber("Elbow Absolute", elbowSeg.absAngle);
	//Position
	elbowSeg.posX = shoulderSeg.posX + shoulderSeg.length * std::cos(shoulderSeg.absAngle * (M_PI/180));
	frc::SmartDashboard::PutNumber("Elbow Pos X", elbowSeg.posX);
	elbowSeg.posY = shoulderSeg.posY + shoulderSeg.length * std::sin(shoulderSeg.absAngle * (M_PI/180));
	frc::SmartDashboard::PutNumber("Elbow Pos Y", elbowSeg.posY);
	}
void ManipulatorArm::updateWrist()
	{
	wristSeg.encValue = getWristAngular();
	frc::SmartDashboard::PutNumber("Wrist Encoder", wristSeg.encValue);
	//Get angles
	wristSeg.relAngle = convertEncoderToRelAngle(&wristSeg, wristSeg.encValue);
	frc::SmartDashboard::PutNumber("Wrist Relative", wristSeg.relAngle);
	wristSeg.absAngle = wristSeg.relAngle + elbowSeg.relAngle + shoulderSeg.absAngle;
	frc::SmartDashboard::PutNumber("Wrist Absolute", wristSeg.absAngle);
	//Position
	wristSeg.posX = elbowSeg.posX + elbowSeg.length * std::cos(elbowSeg.absAngle * (M_PI/180));
	frc::SmartDashboard::PutNumber("Wrist Pos X", wristSeg.posX);
	wristSeg.posY = elbowSeg.posY + elbowSeg.length * std::sin(elbowSeg.absAngle * (M_PI/180));
	frc::SmartDashboard::PutNumber("Wrist Pos Y", wristSeg.posY);
	}	
void ManipulatorArm::updateEndEffector()
	{
	endEffectorX = wristSeg.posX + wristSeg.length * std::cos(wristSeg.absAngle * (M_PI/180));
	frc::SmartDashboard::PutNumber("End Effector X", endEffectorX);
	endEffectorY = wristSeg.posY + wristSeg.length * std::sin(wristSeg.absAngle * (M_PI/180));
	frc::SmartDashboard::PutNumber("End Effector Y", endEffectorY);

	//Lights
	if ((endEffectorX > 28) || (endEffectorX < -28))
		{
		if ((endEffectorX > 32) || (endEffectorX < -32))
			{
			lightShowType = 1; //Outside boundary
			posOutTar = targetPos;
			posOutLast = prevPos;
			frc::SmartDashboard::PutNumber("Out of Bounds Target", posOutTar);
			frc::SmartDashboard::PutNumber("Out of Bounds Start", posOutLast);
			}
		else
			{
			if (lightShowType == 0)
				lightShowType = 2; //Near Boundary
			}
		}

	}

double ManipulatorArm::convertEncoderToRelAngle(tpArmSegment *Seg, double encoder)
	{
	return (encoder * Seg->convSlope + Seg->convIntercept);
	}
double ManipulatorArm::convertRelAngleToEncoder(tpArmSegment *Seg, double angle)
	{
	return (((angle) - (Seg->convIntercept))/(Seg->convSlope));
	}


// Put methods for controlling this subsystem
// here. Call these from Commands.

