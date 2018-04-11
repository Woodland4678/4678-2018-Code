// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef LIDAR_H
#define LIDAR_H
#include <iostream>
#include <fstream>
#include "Commands/Subsystem.h"
#include "DriveTrain/DriveMotorCalculator.h"
#include "WPILib.h"

#define MAXANGLERANGE	45
#define MINANGLERANGE	45
#define OUTLIERCHECK	2
#define MAXDISTRANGE	100
#define MAXDISTNEWLINE	80

#define CUBEWIDTH		330.2
#define CUBEHIEGHT		265.0
#define CUBERANGEWIDTH	40
#define CUBERANGEHIEGHT	40
#define CENTRECUBE      233.5 // sqrt(2*CUBEWIDTH^2)/2

typedef struct Point_t {
	int x;
	int y;
	int tstamp;
}tpPoint;

typedef struct Line_T
	{
	tpPoint start;
	tpPoint end;

	double angle;
	int length;
	}tpLine;

typedef struct Cube_T
	{
	tpPoint location;
	int distance;
	double angle;
	}tpCube;

/**
 *
 *
 * @author ExampleAuthor
 */
class Lidar: public frc::Subsystem {
private:
	frc::SerialPort * serport;
	int glob_lidar_ready; // Gets set to 1 when lidar data is ready in lidat[].  Processing routines should
	// set this to 0 so they can detect when the next valid set of data is available.
	int glob_lidar_count; // Number of samples in lidat[] available for processing.

	typedef struct {
		int dist;
		int angle;
		int tstamp;
		} lidattp;

	lidattp lidat[1024]; // Allow up to 1024 samples in the final array prepared for processing.
	tpPoint lidatXY[1024];
	int xyCount;
	lidattp lidFiltered[1024];
	int filteredCount;
	tpLine lines[100]; // Allow up to 1024 samples in the final array prepared for processing.
	int linecnt;
	tpCube cubes[25]; // Allow up to 1024 samples in the final array prepared for processing.
	int cubecnt;

	lidattp tmpdat[1024]; // This is the sample we compile for copy to lidat
	lidattp cabdat[32]; // Need 32 of these for processing cabin packets

	int glob_lidar_may_run; // Set to 1 to start lidar, 0 to stop it.

	int txcmd = 0;
	int bytes_to_read,bytes_available;
	char rxbuf[1024];
	char payload[128]; // normally up to 84 bytes for a full express scan data set.
	int cstate = 0;
	int sendmode,bcnt,dcnt;
	unsigned int rdat,nbytes,dtype;
	unsigned int model,firmware1,firmware2,hardware,serial; // data from getinfo request.
	int prevangle,stangle,acnt,caboff;
	int chksum;
	int cabins,dist1,delta1,dist2,delta2;
	int samplecount,sss; // cumulative sample counter (max 1023), S bit in data packet 1 for new 360 degree scan.
	int tstamp,prevtstamp; // from fpga timer stuff.
	int pwmspeed,skcnt,tocnt,chcnt,rdcnt,loopcount,synchbyte;
	int txseq = -1; // Nothing to start with.
	double tim1,tim2;

	int readcnt = 0;
	int rightcm = 0;
	int	leftcm = 0;
	bool doneGo;

	bool diagonal = false;
	bool spinCW = false;
	int count = 0;

	DriveCalculatorPtrType m_calculator1_Ptr;
	bool m_calculator1_init;

	std::ofstream logfile; // stream for writing to file
	char buf[256]; // Buffer for writing data
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
public:
	Lidar();
	void InitDefaultCommand() override;
	void Periodic() override;
	void readLidar();
	bool readComplete();
	void convertToXY();
	void filterData(bool convertXY, double leftLimit, double rightLimit, double minDistance, double maxDistance);
	void FindLines();
	void checkLinesForCubes(double frangle,double toangle);
	void calculatePathToNearestCube();
	int findCubes(double frangle,double toangle);
	void startLidar();
	void stopLidar();
	int squareUpCube();
	void getDistanceToCube(int &leftDistCm, int &rightDistCm);
	int checkForCubeIntake();

	int cubeFindCase;
	int cubeSquaringCase;
	int cubeIntakeCase;

	bool isSwitchAuto = false;
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
};

#endif
