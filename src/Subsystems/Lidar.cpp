// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.



#include <stdio.h>
#include "Lidar.h"
#include "../RobotMap.h"
#include "../Commands/LidarScan.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

Lidar::Lidar() : frc::Subsystem("Lidar") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
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
	RPlidarDriver::DisposeDriver(drv);
}

void Lidar::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
	SetDefaultCommand(new LidarScan());
}

void Lidar::Periodic() {
    // Put code here to be run every loop
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
}

void Lidar::Start() {
	drv->startMotor();
	drv->startScan();
}

void Lidar::Stop() {
	drv->stop();
	drv->stopMotor();
}
// Put methods for controlling this subsystem
// here. Call these from Commands.

