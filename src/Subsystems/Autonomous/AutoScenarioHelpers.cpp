/*
 * AutoScenarioHelpers.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"

// MAGIC VALUES
// static const int  s_encoderPulsesPerCm(30);   // Value to use for Clyde
static const int  s_encoderPulsesPerCm(33);   // Value to use for Chevron-7

// FUNCTIONS
int  getEncoderPulsesPerCm()  {
	return s_encoderPulsesPerCm;
}


SwitchSideEnum  transformSwitchFms(unsigned char switchChar) {
    if (switchChar == 'l' || switchChar == 'L') {
        return SwitchSideLeft;
    }

    if (switchChar == 'r' || switchChar == 'R') {
        return SwitchSideRight;
    }

    return SwitchSideUnknown;
}


ScaleSideEnum   transformScaleFms(unsigned char scaleChar) {
    if (scaleChar == 'l' || scaleChar == 'L') {
        return ScaleSideLeft;
    }

    if (scaleChar == 'r' || scaleChar == 'R') {
        return ScaleSideRight;
    }

    return ScaleSideUnknown;
}


RobotStartEnum  transformConsoleSwitch1(int autoSide) {

	switch (autoSide) {
	case 0: return RobotStartLeft;
	case 1: return RobotStartMiddle;
	case 2: return RobotStartRight;
	}

    return RobotStartUnknown;
}

AutoObjectiveEnum   transformConsoleSwitch2(int autoMode) {

	switch (autoMode) {
	case 0: return AutoObjectiveNoAuto;
	case 1: return AutoObjectiveSwitch;
	case 2: return AutoObjectiveScale;
//	  case 3: return AutoObjectiveSwitchScale;
//    case 4: return AutoObjectiveMoveForward;
	}

    return AutoObjectiveUnknown;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
