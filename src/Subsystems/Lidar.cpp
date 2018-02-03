// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "Lidar.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

Lidar::Lidar() : frc::Subsystem("Lidar") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	currentSpeed = 0;
	frequency = 0;
	drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
	printf("Connecting to rplidar\n");
	if(!drv->connect("/dev/ttyUSB0", 115200)) {
		printf("Connected to rplidar\n");
		rplidar_response_device_info_t devinfo;
		if(!drv->getDeviceInfo(devinfo)) {
			printf("RPLIDAR S/N: ");
			for (int pos = 0; pos < 16 ;++pos) {
				printf("%02X", devinfo.serialnum[pos]);
			}

			printf("\n"
					"Version: "RPLIDAR_SDK_VERSION"\n"
					"Firmware Ver: %d.%02d\n"
					"Hardware Rev: %d\n"
					, devinfo.firmware_version>>8
					, devinfo.firmware_version & 0xFF
					, (int)devinfo.hardware_version);
		} else {
			fprintf(stderr, "Error, cannot get device info\n");
		}

		rplidar_response_device_health_t healthinfo;

		// check the device health
		////////////////////////////////////////
		if (!drv->getHealth(healthinfo)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
			printf("RPLidar health status : ");
			switch (healthinfo.status) {
			case RPLIDAR_STATUS_OK:
				printf("OK.");
				break;
			case RPLIDAR_STATUS_WARNING:
				printf("Warning.");
				break;
			case RPLIDAR_STATUS_ERROR:
				printf("Error.");
				break;
			}
			printf(" (errorcode: %d)\n", healthinfo.error_code);

		} else {
			fprintf(stderr, "Error, cannot retrieve the lidar health\n");
			return;
		}


		if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
			fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
			// enable the following code if you want rplidar to be reboot by software
			// drv->reset()
			return;
		}
	} else {
		fprintf(stderr, "Error, cannot connect to lidar\n");
	}
}

Lidar::~Lidar() {
	drv->stop();
	RPlidarDriver::DisposeDriver(drv);
}

void Lidar::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}

void Lidar::Periodic() {
    /* Put code here to be run every loop
	rplidar_response_measurement_node_t nodes[360*2];
	size_t count = _countof(nodes);
	if (!drv->grabScanData(nodes, count)) {
		drv->ascendScanData(nodes, count);

		for (unsigned int i = 0; i < count; ++i) {
			double rad = M_PI * ((nodes[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f) / 180;
			if (nodes[i].distance_q2) {
				int x = (nodes[i].distance_q2/4.0f) * std::sin(rad);
				int y = (nodes[i].distance_q2/4.0f) * std::cos(rad);
				printf(" %i,%i",x,y);
			}
		}
		printf("\n");
	}
	bool in4kMode;
	drv->getFrequency(false, count, frequency, in4kMode);*/
}

size_t Lidar::GetRawData(rplidar_response_measurement_node_t *nodes) {
	size_t count;
	int result = drv->grabScanData(nodes, count);
	printf("lidar result: %i, %i\n", result, count);
	if (!result)
		drv->ascendScanData(nodes, count);
	return count;
}

size_t Lidar::FilterRaw(rplidar_response_measurement_node_t *nodes, rplidar_response_measurement_node_t *filteredNodes, size_t count, double LeftLimit, double RightLimit, double MinDistance, double MaxDistance) {
	unsigned int startidx = 0, n=0;
	double dist, angle;
	//Finding the starting index for the new array
	for(startidx = (count-1);;startidx--){
		angle = ((nodes + startidx)->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
		if(angle < (360-LeftLimit)){
			break;
		}
	}
	startidx++;
	//Going from left limit to 0 degrees
	for(unsigned int i=startidx;i<count;i++){
		dist = (nodes + i)->distance_q2/4.0f;
		if((dist < MinDistance)||(dist > MaxDistance))
			continue;
		*(filteredNodes + n) = *(nodes + i);
		printf("Point angle=%f\n",((filteredNodes + n)->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f);
		n++;
	}
	//From 0 degrees to right limit
	for(unsigned int i=0;;i++){
		angle = ((nodes + i)->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
		if(angle > RightLimit)
			break;
		dist = (nodes + i)->distance_q2/4.0f;
		if((dist < MinDistance)||(dist > MaxDistance))
			continue;
		*(filteredNodes + n) = *(nodes + i);
		printf("Point angle=%f\n",((filteredNodes + n)->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f);
		n++;
	}
	//return number of data points
	return n;
}

size_t Lidar::ConvertToXY(rplidar_response_measurement_node_t *nodes, tpPoint *out, unsigned int count){
	size_t j = 0;
	for(unsigned int i=0;i<count;i++){
		double rad = M_PI * ((nodes[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f) / 180;
		if (nodes[i].distance_q2) {
			int x = std::round((nodes[i].distance_q2/4.0f) * std::sin(rad));
			int y = std::round((nodes[i].distance_q2/4.0f) * std::cos(rad));
			j++;
			out[j].x = x;
			out[j].y = y;
		}
	}
	return j;
}

/*
* Line finding algorithm
* 	The data is sorted based on their direction from (0,0), the first point
* 		for example is close to 90 degrees from (0,0) the last point is close
* 		to -270.
* 		The data must also be in (x,y) coord
* 	The following loop go through this data calculating the angles from one
* 		point to the next and determines if the two points are in a line.
* 		This is a very basic discription
*	Let me walk you through it a little
* 		Start at point 0:
* 			The code creates a new line and sets the start and end to point 0
* 			Then goes to the next point
* 		Point 1:
* 			Since there is no angle to compare to yet as the current line
* 				only exists on a single point
* 			The distance between this point and the line is checked
* 				If the distance is too large then the current line is
* 					set to point 1 and then goes to the next point
* 				If the distance is good then the end point of the current
* 					line is set to point 1 and the angle of the line is
* 					calculated. Then goes to the next point
* 		Point 2:
* 			Assuming that there is a valid angle for the line now
* 				The angle from the line's end point and this point is calculated
* 				Then compared to the angle of the line
* 				If the angle is out of range then the code checks if point 2 is
* 					an outlier by checking the next few points and comparing
* 					their angles to the current line
* 					If not an outlier, then a new line is created at point 2
* 						This new line becomes the current one and then the code
* 							goes to the next point
* 				If the angle is within the range then the end point of the current
* 					line is set to point 2 and the angle is recalculated
* 					Then the code goes to the next point
* 		This continues for all the data points
*/
int Lidar::FindLines(tpPoint *data, tpLine *lines, unsigned int cnt){
	//Find lines
	double distX, distY, totalDist, angle, angle2, diff;
	bool newline; //used for determine outliers and whether or not to create a new line
	int n,m,NumLines=0;
	int skipto = 0;
	//Loop through the data
	for(n=0;n<cnt;n++)
		{
		//Allows the code to skip over data points that are determine to
		//	be outliers
		if(skipto != 0)
			{
			if(n < skipto)
				continue; //Skip this point until n == skipto
			skipto = 0;//reset the skip value
			}
		//To start thing the code sets the first line to start and end
		//	at point 0
		if(n == 0)
			{
			//set start position
			(lines)->start.x = (data+n)->x;
			(lines)->start.y = (data+n)->y;
			//set end position
			(lines)->end.x = (data+n)->x;
			(lines)->end.y = (data+n)->y;
			//Set current line to 0
			NumLines = 0;
			continue; //Go to next point
			}

		//Calculate the differences between this point and the end of the
		//	current line
		distX = ((data+n)->x - (lines + NumLines)->end.x);
		distY = ((data+n)->y - (lines + NumLines)->end.y);
		//Total distance between this point and the end of the current line
		totalDist = sqrt(pow(distX,2)+pow(distY,2));

		//Angle calculation using inverse sin of the rise divided by the hypotenuse
		//	Then its converted from radians into degrees
		angle = (180 * (asin(distY / totalDist))) / M_PI;

		//Checks if there is a valid angle to compare
		//	If both start and end points are the same then there is no valid angle
		//	and the code must add it
		if(((lines + NumLines)->start.x == (lines + NumLines)->end.x)&&((lines + NumLines)->start.y == (lines + NumLines)->end.y))
			{
			//Check if this point is within range
			//	If it is too far away then reset this line and continue
			if(totalDist > MAXDISTNEWLINE)
				{
				//Set start position
				(lines + NumLines)->start.x = (data+n)->x;
				(lines + NumLines)->start.y = (data+n)->y;
				//Set end position
				(lines + NumLines)->end.x = (data+n)->x;
				(lines + NumLines)->end.y = (data+n)->y;
				continue;
				}
			//This point is in range, set the end point equal to it
			(lines + NumLines)->end.x = (data+n)->x;
			(lines + NumLines)->end.y = (data+n)->y;
			//Set the angle between this point and the line's end point
			//	as the line's new direction
			(lines + NumLines)->length = totalDist;
			(lines + NumLines)->angle = angle;
			continue;//Go to the next point
			}

		//Check if this point is suitable as a new end point of the current line
		//	The code checks of the angle and distance are with the range defined
		//	by the constants at the top.
		//		If angle is greater than the current line angle plus high range
		//		If angle is less than the current line angle minus low range
		//		if distance is greater than the max distance
		//			Then check if this is an outlier or the beginning of a new line
		if(((angle > ((lines + NumLines)->angle + MAXANGLERANGE))||(angle < ((lines + NumLines)->angle - MINANGLERANGE)))||(totalDist > MAXDISTRANGE))
			{
			//New line check: assume true
			newline = true;
			//To determine if this point is an outlier the next few points
			//	must be checked, if one of them is in line with the current
			//	line then this current point must be and outlier
			for(m=n+1;m<(OUTLIERCHECK + n + 1);m++)
				{
				if(m > cnt)
					{
					newline = false;
					break;
					}
				//Calculate the differences between this point and the end of the
				//	current line
				distX = ((data+m)->x - (lines + NumLines)->end.x);
				distY = ((data+m)->y - (lines + NumLines)->end.y);
				totalDist = sqrt(pow(distX,2)+pow(distY,2));
				//Angle calculation
				angle2 = (180 * (asin(distY / totalDist))) / M_PI;

				//Check if this point is in range
				if(((angle2 < ((lines + NumLines)->angle + MAXANGLERANGE))&&(angle2 > ((lines + NumLines)->angle - MINANGLERANGE)))&&(totalDist < MAXDISTRANGE))
					{
					//At this point the point is in range
					newline = false;//Do not create a new line, continue with current one
					skipto = m;//Skip the outliers
					break;//Exit this for loop back into the main one
					}
				}
			//Check if a new line needs to be made
			if (newline)
				{
				//increment the line count
				NumLines++;
				//Check that the limit hasn't been reached
				if (NumLines == 100)
					break;
				//Set start position for the new line
				(lines + NumLines)->start.x = (data+n)->x;
				(lines + NumLines)->start.y = (data+n)->y;
				//Set end position for the new line
				(lines + NumLines)->end.x = (data+n)->x;
				(lines + NumLines)->end.y = (data+n)->y;
				}
			continue;//Go to next point
			}

		//If this code is reached then this point must be in range
		//	set it as the new end point for the current line
		(lines + NumLines)->end.x = (data+n)->x;
		(lines + NumLines)->end.y = (data+n)->y;

		//The actual direction of this line changes and must be recalculated
		distX = ((lines + NumLines)->end.x - (lines + NumLines)->start.x);
		distY = ((lines + NumLines)->end.y - (lines + NumLines)->start.y);
		totalDist = sqrt(pow(distX,2)+pow(distY,2));

		(lines + NumLines)->length = totalDist;
		(lines + NumLines)->angle = (180 * (asin(distY / totalDist))) / M_PI;
		}
	distX = ((lines)->start.x - (lines + NumLines)->end.x);
	distY = ((lines)->start.y - (lines + NumLines)->end.y);
	totalDist = sqrt(pow(distX,2)+pow(distY,2));
	if(totalDist < MAXDISTRANGE)
		{
		diff = (lines)->angle - (lines + NumLines)->angle;
		if((diff > (-MINANGLERANGE))&&(diff < MAXANGLERANGE))
			{
			(lines)->start.x = (lines + NumLines)->start.x;
			(lines)->start.y = (lines + NumLines)->start.y;
			distX = ((lines)->end.x - (lines)->start.x);
			distY = ((lines)->end.y - (lines)->start.y);
			totalDist = 330;
			printf("\n%i %i\n",(lines+ NumLines)->end.x,(lines+ NumLines)->start.x);
			(lines)->length = totalDist;
			(lines)->angle = (180 * (asin(distY / totalDist))) / M_PI;
			NumLines--;
			}
		}
	NumLines++;
	return NumLines;
}

int Lidar::FindCubes(tpLine *lines, tpCube * cubes, unsigned int linecnt){
	int n = 0, prev = 0;
	double diffWidth, diffHieght, angle, dist, distX, distY;
	tpPoint center;
	for(unsigned int i = 0; i<linecnt; i++){
		if(prev == 1){
			//Check how far this line is from the prev cube
			distX = ((lines + i)->start.x - (lines + (i-1))->end.x);
			distY = ((lines + i)->start.y - (lines + (i-1))->end.y);
			dist = sqrt(pow(distX,2)+pow(distY,2));
			//If too close skip it
			if (dist < MAXDISTRANGE){
				prev = 0;
				continue;
			}
		}
		diffWidth = std::abs(CUBEWIDTH - (lines + i)->length);
		diffHieght = std::abs(CUBEHIEGHT - (lines + i)->length);
		if ((diffWidth < CUBERANGEWIDTH)||(diffHieght < CUBERANGEHIEGHT)){
			angle = (M_PI * (45 + (lines+i)->angle)) / 180;
			(cubes + n)->location.x = (lines + i)->start.x + CENTRECUBE * cos(angle);
			(cubes + n)->location.y = (lines + i)->start.y + CENTRECUBE * sin(angle);
			//Distance from 0,0 (lidar)
			(cubes + n)->distance = sqrt(pow((cubes + n)->location.x,2) + pow((cubes + n)->location.y,2));
			//Angle from the front from 0,0 (lidar)
			(cubes + n)->angle = (180 * (asin((double)((cubes + n)->location.x) / (double)((cubes + n)->distance)))) / M_PI;
			prev = 1;
			n++;
		}
	}

	return n;
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
void Lidar::Start() {
	drv->startMotor();
	drv->startScan();
}

void Lidar::Stop() {
	drv->stop();
	drv->stopMotor();
}

float Lidar::getFrequency() {
	return frequency;
}

void Lidar::setMotorSpeed(unsigned int pwm) {
	drv->setMotorPWM(pwm);
}
