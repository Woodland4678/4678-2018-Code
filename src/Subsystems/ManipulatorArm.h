// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef MANIPULATORARM_H
#define MANIPULATORARM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

#define FINECONTROLBOXSIZE	12.0f

/**
 *
 *
 * @author ExampleAuthor
 */
 
typedef struct ArmSeg
	{
	double posX;
	double posY;
	int length;
	
	double absAngle;
	double relAngle;
	double encValue;
	
	double convSlope;
	double convIntercept;
	}tpArmSegment;
 
class ManipulatorArm: public frc::Subsystem {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	std::shared_ptr<WPI_TalonSRX> shoulder;
	std::shared_ptr<WPI_TalonSRX> elbow;
	std::shared_ptr<WPI_TalonSRX> wrist;
	std::shared_ptr<WPI_VictorSPX> shoulderSlave;
	std::shared_ptr<WPI_VictorSPX> elbowSlave;
	std::shared_ptr<frc::DoubleSolenoid> claw;
	std::shared_ptr<frc::Relay> indicator;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	
	double shoulderNewSet;

	double origTimeStamp;
	double shoulderStartPos;
	double elbowStartPos;
	double wristStartPos;
	int moveCase;
	bool shoulderMovement;
	double shTarget1, shTarget2, shTime, shTime1, shTime2, elTime, wrTime;
	bool shoulderMovement2;
	bool shoulderMovement3;
	bool elbowMovement;
	bool wristMovement;
	double wrDegreePerSecond, elDegreePerSecond, shDegreePerSecond;
	double shMinTime, elMinTime, wrMinTime;
	
	int positions[13][3];

	double posOffset;
	double multOffset;
	
	double elbowDeg;
	double ShoulderDeg;

	int lightCounter;
	int lightCounter2;
	int lightShowType;
	int posOutTar;
	int posOutLast;

public:
	ManipulatorArm();
	void InitDefaultCommand() override;
	void Periodic() override;
	int getShoulderAngular();
	int getWristAngular();
	int getElbowAngular();
	void setShoulderPosition(double position);
	void setElbowPosition(double position);
	void setWristPosition(double position);
	bool shoulderGoToPosition(double start, double position, double current, double time);
	bool elbowGoToPosition(double start, double position, double current, double time);
	bool wristGoToPosition(double start, double position, double current, double time);
	double sigmod(double end, double start, double mult, double offset, double x);
	double invSigmod(double end, double start, double mult, double offset, double pnt);
	double convertShoulderToAngle(double encoder);
	double convertElbowToAngle(double encoder);
	double convertWristToAngle(double encoder);
	double convertShoulderToEncoder(double angle);
	double convertElbowToEncoder(double angle);
	double convertWristToEncoder(double angle);
	void initMovement();
	bool pickUpCube();
	bool raiseCube();
	void setShoulderPID(double p, double i, double d, int zone, double f);
	void setElbowPID(double p, double i, double d, int zone, double f);
	void setWristPID(double p, double i, double d, int zone, double f);
	
	bool moveTo(int pos, double addSh = 0, double addElTime = 0);
	void setShoulderAbsAngle(double angle);
	void setElbowAbsAngle(double angle);
	void setWristAbsAngle(double angle);
	void updateShoulder();
	void updateElbow();
	void updateWrist();
	void updateEndEffector();
	void updateArm();
	
	double convertEncoderToRelAngle(tpArmSegment *Seg, double encoder);
	double convertRelAngleToEncoder(tpArmSegment *Seg, double angle);
	bool fineMovement(double yDirection, double xDirection);

	void squeeze();
	void release();

	tpArmSegment shoulderSeg;
	tpArmSegment elbowSeg;
	tpArmSegment wristSeg;

	int currPos;
	int targetPos;
	int prevPos;
	double endEffectorX;
	double endEffectorY;
	int fineMoveCase;
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
};

#endif
