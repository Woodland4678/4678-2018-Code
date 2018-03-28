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

#include <iostream>
#include <fstream>
#include <ctime>
#include "ManipulatorArm.h"
#include "../Commands/moveArm.h"
#include "../RobotMap.h"

#define SHOULDERLOWLIMIT	47.0f
#define SHOULDERHIGHLIMIT	127.0f

#define ELBOWHIGHLIMIT		164.0f

#define WRISTLOWLIMIT		-120.0f
#define WRISTHIGHLIMIT		120.0f

#if FLIGHTBOT
#define SHOULDERLOWENCODER	-4875.0f
#define	SHOULDERHIGENCODER	4283.0f

#define ELBOWLOWLIMIT		-156.2f
#define ELBOWLOWENCODER		1581.0f
#define	ELBOWHIGENCODER		-9689.0f

#define WRISTLOWENCODER		5386.0f
#define	WRISTHIGENCODER		-1308.0f
#else
#define SHOULDERLOWENCODER	-6382.0f
#define	SHOULDERHIGENCODER	2743.0f

#define ELBOWLOWLIMIT		-159.2f
#define ELBOWLOWENCODER		2757.0f
#define	ELBOWHIGENCODER		-11237.0f

#define WRISTLOWENCODER		-1743.0f
#define	WRISTHIGENCODER		4553.0f
#endif
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
    cubeDetector = RobotMap::manipulatorArmcubeDetector;
    shoulderClimber = RobotMap::manipulatorArmshoulderClimber;

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
	
	int shAngCnt = 0;
	int pulse = 0;
	double enc = 0;
	do {
		startPosition = (shoulder->GetSensorCollection().GetPulseWidthPosition() % 4096);
		if (startPosition < 0)
			startPosition += 4096;
		shoulder->GetSensorCollection().SetPulseWidthPosition(startPosition, 0);
		shoulder->GetSensorCollection().SetQuadraturePosition(startPosition*0.776527742265, 0);

		pulse = shoulder->GetSensorCollection().GetPulseWidthPosition();
		enc = getShoulderAngular();
		if (std::abs(pulse - (enc/0.776527742265)) < 100)
			frc::SmartDashboard::PutBoolean("Shoulder Encoders", true);
		else
			frc::SmartDashboard::PutBoolean("Shoulder Encoders", false);

		shAngCnt++;
	}while((std::abs(pulse - (enc/0.776527742265)) > 100)&&(shAngCnt < 10));
	printf("Shoulder Encoders Took %i loop to be the same\n",shAngCnt);
	shoulderSeg.posX = 0;
	shoulderSeg.posY = 0;
	shoulderSeg.length = 37; //inches
	shoulderSeg.convSlope = (SHOULDERLOWLIMIT - SHOULDERHIGHLIMIT)/(SHOULDERLOWENCODER - SHOULDERHIGENCODER);//0.008594429939077;
	shoulderSeg.convIntercept = SHOULDERHIGHLIMIT - SHOULDERHIGENCODER * shoulderSeg.convSlope;//102.68635770235;
	shoulderSeg.setAbsAngle = 109;
	shoulderSeg.setRelAngle = 109;
	shoulderSeg.setEncValue = convertRelAngleToEncoder(&shoulderSeg, 109);

	shDegreePerSecond = 50;
	shMinTime = 1;
	updateShoulder();
	if(std::abs(shoulderSeg.absAngle - 109) < 15)
		frc::SmartDashboard::PutBoolean("Shoulder Angle", true);
	else
		frc::SmartDashboard::PutBoolean("Shoulder Angle", false);

	ShoulderDeg = shoulderSeg.absAngle;

    //Elbow
	elbowSlave->Follow(*elbow);
	elbow->SelectProfileSlot(0,0);
	elbow->ConfigVoltageCompSaturation(12, 0);
	elbow->EnableVoltageCompensation(false);
	elbow->EnableCurrentLimit(false);
	setElbowPID(3,0,0.2,0,0);
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
	elbowSeg.convSlope = (ELBOWLOWLIMIT - ELBOWHIGHLIMIT)/(ELBOWLOWENCODER - ELBOWHIGENCODER);//-0.023020774;
	elbowSeg.convIntercept = ELBOWLOWLIMIT - getElbowAngular()*elbowSeg.convSlope;
#if FLIGHTBOT
	elbowSeg.setAbsAngle = -46;
	elbowSeg.setRelAngle = -155;
	elbowSeg.setEncValue = convertRelAngleToEncoder(&elbowSeg, -155);
#else
	elbowSeg.setAbsAngle = -49;
	elbowSeg.setRelAngle = -159;
	elbowSeg.setEncValue = convertRelAngleToEncoder(&elbowSeg, -159);
#endif


	elDegreePerSecond = 90;
	elMinTime = 1.5;
	updateElbow();
	
	if(std::abs(elbowSeg.absAngle - (-50.2)) < 15)
		frc::SmartDashboard::PutBoolean("Elbow Angle", true);
	else
		frc::SmartDashboard::PutBoolean("Elbow Angle", false);

	elbowDeg = elbowSeg.absAngle;

    //Wrist
	wrist->EnableCurrentLimit(false);
	wrist->ConfigVoltageCompSaturation(12, 0);
	wrist->EnableVoltageCompensation(false);
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
	wristSeg.convSlope = (WRISTLOWLIMIT - WRISTHIGHLIMIT)/(WRISTLOWENCODER - WRISTHIGENCODER);//0.037453761
	wristSeg.convIntercept = WRISTHIGHLIMIT - WRISTHIGENCODER * wristSeg.convSlope;//-56.25369914;
	wristSeg.setAbsAngle = 0;
	wristSeg.setRelAngle = 50;
	wristSeg.setEncValue = convertRelAngleToEncoder(&wristSeg, 50);

	wrDegreePerSecond = 90; // 125; // 150; // was 90
	wrMinTime = 1.5; // 1.0; //  was 1.5
	updateWrist();
	
	if(std::abs(wristSeg.absAngle - 0) < 15)
		frc::SmartDashboard::PutBoolean("Wrist Angle", true);
	else
		frc::SmartDashboard::PutBoolean("Wrist Angle", false);

	posOffset = 4.25;
	multOffset = (9 + (posOffset - 4)) * 1000;
	
	indicator->Set(frc::Relay::Value::kOff);
	lightCounter = 0;
	lightCounter2 = 0;
	lightShowType = 0;

	//Positions:
	//Home Position
	positions[0][0] = 109;   //e:624     r:109
#if FLIGHTBOT
    positions[0][1] = -46;
#else
    positions[0][1] = -49;
#endif
    positions[0][2] = 0;     //e:2970    r:55
    
    //Scale front Max
    positions[1][0] = 87;    //e:-2634   r:87
    positions[1][1] = 75;    //e:-3673   r:-12
    positions[1][2] = 15;    //e:-99     r:-60
    
    //Scale front Min
    positions[2][0] = 122;   //e:1660    r:116
    positions[2][1] = 42;  //e:-1096   r:-71
    positions[2][2] = 14;    //e:687     r:-30
    
    //Scale back Max
    positions[3][0] = 93;    //e:-1745   r:93
    positions[3][1] = 105;   //e:-4714   r:12 
    positions[3][2] = 165;   //e:3103    r:60
    
    //scale back min
    positions[4][0] = 57;    //e:-6042   r:64
    positions[4][1] = 142;   //e:-7272   r:71
    positions[4][2] = 165;   //e:2303    r:30
    
    //Pick up cube
    positions[5][0] = 124;    //e:-1745   r:93
    positions[5][1] = -22;   //e:2478    r:-154
    positions[5][2] = -25;   //e:754     r:-28
    
    //Pick up cube middle
    positions[6][0] = 98;    //e:-2784   r:86
    positions[6][1] = -50;   //e:2134    r:-146
    positions[6][2] = 21;    //e:728     r:-29
    
    //Switch Front
    positions[7][0] = 113;   //e:2697    r:123
    positions[7][1] = -15;   //e:1614    r:-134
    positions[7][2] = 19;    //e:2302    r:30
    
    //Switch Back
    positions[8][0] = 68;    //e:-7080   r:123
    positions[8][1] = 196;   //e:-10001  r:134
    positions[8][2] = 158;   //e:728     r:-30
    
    //Grab climber
    positions[9][0] = 69;    //e:-5007   r:71
    positions[9][1] = 223;   //e:-10957  r:155
    positions[9][2] = 279;   //e:2649    r:43
    
    //Place climber
    positions[10][0] = 52;   //e:-859    r:99
    positions[10][1] = 117;   //e:-4154   r:0
    positions[10][2] = 143;  //e:3691    r:82
    
    positions[14][0] = 63;
    positions[14][1] = 124;
    positions[14][2] = 163;

    positions[15][0] = 63;
    positions[15][1] = 128;
    positions[15][2] = 182;

    //Carry
    positions[11][0] = 124;  //e:3291    r:127
    positions[11][1] = -16;  //e:2479    r:-154 SDW -14 (was-17)
    positions[11][2] = 71;   //e:3851    r:88

    //Arm Position For when climbing
    positions[12][0] = 93;
    positions[12][1] = -24;
    positions[12][2] = -139;

    positions[13][0] = 72;
    positions[13][1] = -73;
    positions[13][2] = -173;

    currPos = 0;
    targetPos = 0;
    prevPos = 0;
    fineMovingGoing = 0;

    //frc::SmartDashboard::PutNumber("Shoulder Degree", ShoulderDeg);
    //frc::SmartDashboard::PutNumber("Elbow Degree", elbowDeg);
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
	//frc::SmartDashboard::PutNumber("Light Type", lightShowType);
	//frc::SmartDashboard::PutNumber("Light cnt 1", lightCounter);
	//frc::SmartDashboard::PutNumber("Light cnt 2", lightCounter2);
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

	if (logfile.is_open())
		{
		sprintf(buf,"B,%f,%f,%f,%f,%i,%i,%i\n",origTimeStamp,shoulderStartPos,elbowStartPos,wristStartPos,positions[targetPos][0],positions[targetPos][1],positions[targetPos][2]);
		logfile.write(buf,strlen(buf));
		}

	//frc::SmartDashboard::PutNumber("Shoulder Start", shoulderStartPos);
	//frc::SmartDashboard::PutNumber("Elbow Start", elbowStartPos);
	//frc::SmartDashboard::PutNumber("Wrist Start", wristStartPos);

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

void ManipulatorArm::moveWrist(double joyValue)
	{
	double ang = wristSeg.absAngle + (joyValue*4 );
	if(ang < -80)
		ang = -80;
	if (ang > 143)
		ang = 143;
	setWristAbsAngle(ang);
	}

bool ManipulatorArm::fineMovement(double joyX, double joyY) {

	//Get current end point
	updateArm();
	double elbPosX = shoulderSeg.length * std::cos(positions[targetPos][0] * (M_PI/180));
	double elbPosY = shoulderSeg.length * std::sin(positions[targetPos][0] * (M_PI/180));

	double currX = elbPosX + elbowSeg.length * std::cos(positions[targetPos][1] * (M_PI/180));
	double currY = elbPosY + elbowSeg.length * std::sin(positions[targetPos][1] * (M_PI/180));
	//Calculate Target Position
	double tarX = std::abs(currX);
	double tarY = std::abs(currY);

	tarX += FINECONTROLBOXSIZE * joyX;
	tarY += FINECONTROLBOXSIZE * joyY;

	double newTarX = tarX;
	double newTarY = tarY;

	updateArm();
	//We have our target, and current location
	//Find direction vector
	double dirX = tarX - wristSeg.posX;
	double dirY = tarY - wristSeg.posY;
	//Find total distance
	double dist = std::sqrt((dirX*dirX)+(dirY*dirY));
	//Speed stuff
	dist /= (3 * 0.02); //3 Inches per second, 0.02 each loop's time 50 Hz
	//Disance calculation
	newTarX = (dirX / dist) + wristSeg.posX;
	newTarY = (dirY / dist) + wristSeg.posY;

	if((std::abs(wristSeg.posX - tarX) < 1) && (std::abs(wristSeg.posY - tarY) < 1))
		{
		newTarX = tarX;
		newTarY = tarY;
		}

	//Check limits
	if(newTarX > 26)
		newTarX = 26;
	if(newTarX < -26)
		newTarX = -26;
	if(newTarY > 77)
		newTarY = 77;
	//if(tarY < 50)
	//	tarY = 50;
	if((newTarX < 1)&&(newTarY > 50))
		newTarX = 1;

	//printf("Targets: %f, %f\nPosition: %f, %f\nCircle: %f, %f\nNew Targets: %f, %f\n",tarX,tarY,wristSeg.posX,wristSeg.posY,currX,currY,newTarX,newTarY);

	//Inverse Kitematics
	//Elbow
	double casBrakets = ((std::pow(newTarX,2) + std::pow(newTarY,2)) - std::pow(37,2) - std::pow(40,2))/(-2*(37)*(40));
	//Check value incase it is too far
	if((casBrakets < -1)||(casBrakets > 1))
		casBrakets = std::floor(casBrakets);
	double angElbow = M_PI - std::acos(casBrakets);
	//Shoulder
	double angShoulder = std::atan(newTarY / newTarX) - std::atan((40 * std::sin(-angElbow))/(37 + 40 * std::cos(-angElbow)));
	//Convert radians to degrees
	elbowDeg = (angElbow * 180) / M_PI;
	ShoulderDeg = (angShoulder * 180) / M_PI;

	//Check for negative
	if((currX) < 0)
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

	//Lets just do some quick tests to make sure we aren't attempting to move
	//	too far too quickly
	if (std::abs(ShoulderDeg - shoulderSeg.absAngle) > 20)
		return false;
	if (std::abs(elbowDeg - elbowSeg.absAngle) > 20)
		return false;

	frc::SmartDashboard::PutNumber("Shoulder Degree", ShoulderDeg);
	frc::SmartDashboard::PutNumber("Elbow Degree", elbowDeg);
	printf("Degrees SH: %f , EL: %f\n\n",ShoulderDeg,elbowDeg);

	setShoulderAbsAngle(ShoulderDeg);
	setElbowAbsAngle(elbowDeg);

	/*double sldrTravDis = ShoulderDeg - shoulderSeg.absAngle;
	double elbTravDis = elbowDeg - elbowSeg.absAngle;
	printf("ShDist = %f, ElSidt = %f\n",sldrTravDis,elbTravDis);
	double incrementalSh = 0.1;
	double incrementalEl = 0.1;

	if((std::abs(sldrTravDis) < incrementalSh) && (std::abs(elbTravDis) < incrementalEl))
		{
		setShoulderAbsAngle(ShoulderDeg);
		setElbowAbsAngle(elbowDeg);
		}
	else
		{
		if (sldrTravDis > elbTravDis)
			{
			printf("Sh > El\n");
			if(sldrTravDis < 0)
				incrementalSh *= -1;
			setShoulderAbsAngle(shoulderSeg.absAngle + incrementalSh);
			printf("Shoulder Angle = %f\n",shoulderSeg.absAngle + incrementalSh);
			if(elbTravDis < 0)
				incrementalEl *= -1;
			setElbowAbsAngle(elbowSeg.absAngle + (incrementalEl * (elbTravDis / sldrTravDis)));
			printf("Elbow Angle = %f\n",elbowSeg.absAngle + (incrementalEl * (elbTravDis / sldrTravDis)));
			}
		else
			{
			printf("El > Sh\n");
			if(elbTravDis < 0)
				incrementalEl *= -1;
			setElbowAbsAngle(elbowSeg.absAngle + incrementalEl);
			printf("Elbow Angle = %f\n",elbowSeg.absAngle + incrementalEl);
			if(sldrTravDis < 0)
				incrementalSh *= -1;
			setShoulderAbsAngle(shoulderSeg.absAngle + (incrementalSh * (sldrTravDis / elbTravDis)));
			printf("Shoulder Angle = %f\n",shoulderSeg.absAngle + (incrementalSh * (sldrTravDis / elbTravDis)));
			}
		}*/
	return true;
	}

bool ManipulatorArm::moveTo(int pos, double addShTime, double addElTime)
	{
	frc::SmartDashboard::PutNumber("Arm Case", moveCase);
	if(fineMovingGoing)
		return true;
	if(isClimber)
		return true;
	if((pos != targetPos) && (moveCase != 0))
		moveCase = 0;
	switch(moveCase)
		{
		case 0:
			targetPos = pos;
			if (logfile.is_open())
				{
				sprintf(buf,"M,%i,%i\n",targetPos,currPos);
				logfile.write(buf,strlen(buf));
				}
			initMovement();
			moveCase = 1;
			break;
		case 1:
			{
			//Pre calculations here, timing and shoulder movements
			//Timing
			wrTime = (std::abs(wristStartPos - positions[pos][2])) / wrDegreePerSecond;
			elTime = (std::abs(elbowStartPos - positions[pos][1])) / elDegreePerSecond;
			shTime = (std::abs(shoulderStartPos - positions[pos][0])) / shDegreePerSecond;

			if (logfile.is_open())
				{
				sprintf(buf,"Tb,%f,%f,%f\n",shTime,elTime,wrTime);
				logfile.write(buf,strlen(buf));
				}

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
					shTime += 0.7;
					elTime += 0.5;
					wrTime += 0.7;
					shoulderMovement2 = false;
					}
				if((std::abs(targetZone - currentZone)) == 1)
					{
					shTime += 0.3;
					elTime += 0.3;
					wrTime += 0.3;
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
				shoulderMovement2 = true;
				shoulderMovement3 = true;
				}
			//Special case:
			if((currPos == 11) && (targetPos == 5))
				{
				pickupCount = 0;
				pickupCount2 = 0;
				//setWristAbsAngle(positions[5][2]);
				//moveCase = 4;
				//return false;
				}
			if (logfile.is_open())
				{
				sprintf(buf,"Ta,%f,%f,%f,%f,%f\n",shTime,elTime,wrTime,shTime1,shTime2);
				logfile.write(buf,strlen(buf));

				sprintf(buf,"EM,%f,%f\n",shTarget1,shTarget2);
				logfile.write(buf,strlen(buf));
				}

			moveCase += 1;
			}
			break;
		case 2:
			{
			updateArm();
			double currTime = frc::Timer::GetFPGATimestamp() - origTimeStamp;

			if (logfile.is_open())
				{
				sprintf(buf,"T,%f\n",currTime);
				logfile.write(buf,strlen(buf));
				}

			//frc::SmartDashboard::PutNumber("Shoulder Time", shTime);
			//frc::SmartDashboard::PutNumber("Elbow Time", elTime);
			//frc::SmartDashboard::PutNumber("Wrist Time", wrTime);

			//frc::SmartDashboard::PutNumber("Current Time", currTime);

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
			
			//frc::SmartDashboard::PutNumber("Shoulder Complete", shoulderMovement);
			//frc::SmartDashboard::PutNumber("Elbow Complete", elbowMovement);
			//frc::SmartDashboard::PutNumber("Wrist Complete", wristMovement);

			//Movement Check
			if(shoulderMovement && elbowMovement && wristMovement)
				{
				moveCase++;
				prevPos = currPos;
				currPos = pos;
				moveCase = 3;
				if (logfile.is_open())
					{
					sprintf(buf,"C\n");
					logfile.write(buf,strlen(buf));
					}
				if(pos == 9)
					{
					pickupCount = 0;
					pickupCount2 = 0;
					moveCase = 5;
					}
				}
			}
			break;
		case 3:
			moveCase = 0;
			return true;
			break;
		case 4:
			{
			updateArm();
			//Pickup special case
			//Lets move straight down until we detect a cube in the claw
			if(std::abs(positions[5][2] - wristSeg.absAngle) < 2)
				{
				if(pickupCount == 0)
					{
					setElbowAbsAngle(positions[5][1]);
					setShoulderAbsAngle(positions[5][0]);
					pickupCount = 1;
					}
				}
			if(pickupCount != 0)
				{
				pickupCount++;
				if(checkForCube())
					pickupCount2++;
				else
					pickupCount2 = 0;
				if(pickupCount2 >= 10)
					{
					//Once we detect that there is a cube, clamp down on it
					squeeze();
					Robot::intake->release();
					moveCase = 0;
					return true;
					}
				if(pickupCount > 50)
					{
					if(checkForCube())
						squeeze();
					moveCase = 0;
					return true;
					}
				}
			break;
			}
		case 5:
			pickupCount++;
			if(pickupCount > 20)
				squeeze();
			//I have to assume that we have it
			//Move straight up and then turn wrist
			if(pickupCount > 40)//Wait a moment to ensure the claw clamped down
				{
				pickupCount2++;
				setShoulderAbsAngle(positions[9][0] - (pickupCount2 * 0.3));
				setElbowAbsAngle(positions[9][1] - (pickupCount2 * 0.3));
				if(pickupCount2 > 10)
					{
					setWristAbsAngle(positions[9][2] - 90);
					moveCase = 3;
					}
				}
			break;
		}
	return false;
	}

bool ManipulatorArm::calibrate()
	{
	static int mainCase = 0, shCase = 0, elCase = 0, wrCase = 0, count = 0;
	switch(mainCase)
		{
		case 0:
			shCase = 0;
			elCase = 0;
			wrCase = 0;
			count = 0;
			mainCase = 1;
			break;
		case 1:
			switch(shCase)
				{
				case 0:
					//Set shoulder speed
					count++;
					setShoulderSpeed(0.2);
					if((shoulder->GetSensorCollection().GetQuadratureVelocity() > 0.2)||(count > 20))
						shCase = 1;
					break;
				case 1:
					//Check speed
					if (shoulder->GetSensorCollection().GetQuadratureVelocity() < 0.1)
						shCase = 2;
					break;
				case 2:
					setShoulderSpeed(0);
					if(std::abs(getShoulderAngular() - (SHOULDERHIGENCODER+400)) > 300) //Check if there may have been a chain jump
						shChainJump = true;
					shoulder->SetSelectedSensorPosition(SHOULDERHIGENCODER+400, 0, 0);
					//shoulderSeg.convIntercept = 127 - getShoulderAngular() * shoulderSeg.convSlope;
					updateArm();
					shCase = 3;
					mainCase = 2;
					count = 0;
					break;
				}
			break;
		case 2:
			count++;
			switch(elCase)
				{
				case 0:
					//Set Elbow speed
					setElbowSpeed(-0.2);
					if((elbow->GetSensorCollection().GetQuadratureVelocity() > 0.2)||(count > 20))
						elCase = 1;
					break;
				case 1:
					//Check speed
					if(std::abs(getElbowAngular() - (ELBOWLOWENCODER)) > 300) //Check if there may have been a chain jump
						elChainJump = true;
					if (elbow->GetSensorCollection().GetQuadratureVelocity() < 0.1)
						elCase = 2;
					break;
				case 2:
					//Angle is -159.2
					setElbowSpeed(0);
					elbow->SetSelectedSensorPosition(ELBOWLOWENCODER, 0, 0);
					elCase = 3;
					break;
				}
			switch(wrCase)
				{
				case 0:
					//Set wrist speed
#if FLIGHTBOT
					wrist->Set(ControlMode::PercentOutput, -0.2);
#else
					wrist->Set(ControlMode::PercentOutput, 0.2);
#endif
					if((wrist->GetSensorCollection().GetQuadratureVelocity() > 0.1)||(count > 20))
						wrCase = 1;
					break;
				case 1:
					//Check speed
					if (wrist->GetSensorCollection().GetQuadratureVelocity() < 0.1)
						wrCase = 2;
					break;
				case 2:
					wrist->Set(ControlMode::PercentOutput, 0);
					elbow->SetSelectedSensorPosition(WRISTHIGENCODER+200, 0, 0);
					wrCase = 3;
					break;
				}
			if((elCase == 3)&&(wrCase == 3))
				{
				mainCase = 0;
				return true;
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
void ManipulatorArm::setElbowSpeed(double speed)
	{
	elbow->Set(ControlMode::PercentOutput, speed);
	}
void ManipulatorArm::setShoulderSpeed(double speed)
	{
	shoulder->Set(ControlMode::PercentOutput, speed);
	}

//These function send the talons a new setpoint
void ManipulatorArm::setShoulderAbsAngle(double angle)
	{
	double pos = convertRelAngleToEncoder(&shoulderSeg, angle);
	shoulderSeg.setAbsAngle = angle;
	shoulderSeg.setEncValue = pos;
	if (logfile.is_open())
		{
		sprintf(buf,"S,%f,%f,%i,%f\n",shoulderSeg.absAngle, angle, getShoulderAngular(), pos);
		logfile.write(buf,strlen(buf));
		}

	shoulder->Set(ControlMode::Position, pos);
	}
void ManipulatorArm::setElbowAbsAngle(double angle)
	{
	//Convert to relative
	double relAng = angle - shoulderSeg.setAbsAngle;
	double pos = convertRelAngleToEncoder(&elbowSeg, relAng);
	elbowSeg.setAbsAngle = angle;
	elbowSeg.setRelAngle = relAng;
	elbowSeg.setEncValue = pos;
	if (logfile.is_open())
		{
		sprintf(buf,"E,%f,%f,%f,%f,%i,%f\n",elbowSeg.absAngle,angle,elbowSeg.relAngle, relAng, getElbowAngular(), pos);
		logfile.write(buf,strlen(buf));
		}

	elbow->Set(ControlMode::Position, pos);
	}
void ManipulatorArm::setWristAbsAngle(double angle)
	{
	//Convert to relative
	double relAng = angle - elbowSeg.setRelAngle - shoulderSeg.setAbsAngle;
	double pos = convertRelAngleToEncoder(&wristSeg, relAng);
	wristSeg.setAbsAngle = angle;
	wristSeg.setRelAngle = relAng;
	wristSeg.setEncValue = pos;
	if (logfile.is_open())
		{
		sprintf(buf,"W,%f,%f,%f,%f,%i,%f\n",wristSeg.absAngle,angle,wristSeg.relAngle, relAng, getWristAngular(), pos);
		logfile.write(buf,strlen(buf));
		}

	wrist->Set(ControlMode::Position, pos);
	}

void ManipulatorArm::setShoulderPosition(double position)
	{
	shoulder->Set(ControlMode::Position, position);
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
void ManipulatorArm::climber(){
	shoulderClimber->Set(frc::DoubleSolenoid::Value::kForward);
	isClimber = true;
}
void ManipulatorArm::shiftToShoulder(){
	shoulderClimber->Set(frc::DoubleSolenoid::Value::kReverse);
	isClimber = false;
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
	frc::SmartDashboard::PutNumber("Shoulder Encoder", shoulderSeg.encValue);
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
	frc::SmartDashboard::PutNumber("Elbow Encoder Set", elbowSeg.setEncValue);
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

	//Check Claw
	frc::SmartDashboard::PutBoolean("Cube In Claw",checkForCube());
	//Check intake
	if((wristSeg.posX > 0) && (wristSeg.posY < 14))
		frc::SmartDashboard::PutBoolean("Intake moveable", false);
	else
		frc::SmartDashboard::PutBoolean("Intake moveable", true);

	//Lights
	if ((endEffectorX > 28) || (endEffectorX < -28))
		{
		if ((endEffectorX > 34) || (endEffectorX < -34))
			{
			lightShowType = 1; //Outside boundary
			posOutTar = targetPos;
			posOutLast = prevPos;
			//frc::SmartDashboard::PutNumber("Out of Bounds Target", posOutTar);
			//frc::SmartDashboard::PutNumber("Out of Bounds Start", posOutLast);
			}
		else
			{
			if (lightShowType == 0)
				lightShowType = 2; //Near Boundary
			}
		}

	}

std::string ManipulatorArm::getPositionStr(int pos)
	{
	std::string str;
	switch(pos)
		{
		case 0:
			str = "Home";
			break;
		case 1:
			str = "Front Scale High";
			break;
		case 2:
			str = "Front Scale Low";
			break;
		case 3:
			str = "Back Scale High";
			break;
		case 4:
			str = "Back Scale Low";
			break;
		case 5:
			str = "Cube Pickup";
			break;
		case 6:
			str = "Portal";
			break;
		case 7:
			str = "Front Switch";
			break;
		case 8:
			str = "Back Switch";
			break;
		case 9:
			str = "Climber";
			break;
		case 10:
			str = "Place Climber";
			break;
		case 11:
			str = "Carry";
			break;
		case 12:
			str = "Climbing";
			break;
		}
	return str;
	}

bool ManipulatorArm::checkForCube()
	{
	return cubeDetector->Get() ^ 1;
	}

double ManipulatorArm::convertEncoderToRelAngle(tpArmSegment *Seg, double encoder)
	{
	return (encoder * Seg->convSlope + Seg->convIntercept);
	}
double ManipulatorArm::convertRelAngleToEncoder(tpArmSegment *Seg, double angle)
	{
	return (((angle) - (Seg->convIntercept))/(Seg->convSlope));
	}

void ManipulatorArm::openLog()
	{
	time_t t=time(0);
	struct tm *now;
	now = localtime(&t);

	if(!logfile.is_open())
		{
		sprintf(buf,"/media/sda1/ARM%02d%02d%02d%02d%02d%02d.txt",now->tm_year,now->tm_mon,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		logfile.open(buf,std::ios::out | std::ios::binary);
		}
	else
		{
		sprintf(buf,"/media/sdb1/LL%02d%02d%02d%02d%02d%02d.txt",now->tm_year,now->tm_mon,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		logfile.open(buf,std::ios::out | std::ios::binary);
		}
	}
void ManipulatorArm::closeLog()
	{
	if (logfile.is_open())
		logfile.close();
	}
// Put methods for controlling this subsystem
// here. Call these from Commands.

