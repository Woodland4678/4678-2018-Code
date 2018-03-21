/*
 * AutoScenarioHelpers.h
 *
 *  Created on: Feb 23, 2018
 *      Author: wchs
 */

#ifndef AUTO_SCENARIO_HELPERS_H_
#define AUTO_SCENARIO_HELPERS_H_

// PROJECT INCLUDES
#include "AutoScenario.h"

// ENUMS
// Defines where along the wall the robot is starting from
enum RobotStartEnum  {
    RobotStartUnknown = 0,
    RobotStartLeft,
    RobotStartMiddle,
    RobotStartRight,
    RobotStartMax
};

// Defines what side of the switch we own
enum SwitchSideEnum  {
    SwitchSideUnknown = 0,
    SwitchSideLeft,
    SwitchSideRight,
    SwitchSideWildcard,		// Only used in the ScenarioFinder algorithm
    SwitchSideMax
};

// Defines what side of the scale we own
enum ScaleSideEnum  {
    ScaleSideUnknown = 0,
    ScaleSideLeft,
    ScaleSideRight,
    ScaleSideWildcard,		// Only used in the ScenarioFinder algorithm
    ScaleSideMax
};

// Defines what the objective is for autonomous
enum AutoObjectiveEnum  {
    AutoObjectiveUnknown = 0,
    AutoObjectiveNoAuto,
    AutoObjectiveSwitch,
    AutoObjectiveScale,
//    AutoObjectiveSwitchScale,
//    AutoObjectiveMoveForward,
    AutoObjectiveMax
};

// Defines what scenarion we want the robot to run for autonomous
enum AutoScenarioEnum  {
    AutoScenarioUnknown = 0,

    AutoScenario_RobotLeft_NoAuto,
//    AutoScenario_RobotLeft_SwitchLeft,
//    AutoScenario_RobotLeft_SwitchRight,
    AutoScenario_RobotLeft_ScaleLeft,
    AutoScenario_RobotLeft_ScaleRight,
//    AutoScenario_RobotLeft_SwitchLeft_ScaleLeft,
//    AutoScenario_RobotLeft_SwitchLeft_ScaleRight,
//    AutoScenario_RobotLeft_SwitchRight_ScaleLeft,
//    AutoScenario_RobotLeft_SwitchRight_ScaleRight,
//    AutoScenario_RobotLeft_MoveForward,

    AutoScenario_RobotMiddle_NoAuto,
    AutoScenario_RobotMiddle_SwitchLeft,
    AutoScenario_RobotMiddle_SwitchRight,
//    AutoScenario_RobotMiddle_ScaleLeft,
//    AutoScenario_RobotMiddle_ScaleRight,
//    AutoScenario_RobotMiddle_SwitchLeft_ScaleLeft,
//    AutoScenario_RobotMiddle_SwitchLeft_ScaleRight,
//    AutoScenario_RobotMiddle_SwitchRight_ScaleLeft,
//    AutoScenario_RobotMiddle_SwitchRight_ScaleRight,
//    AutoScenario_RobotMiddle_MoveForward,

    AutoScenario_RobotRight_NoAuto,
//    AutoScenario_RobotRight_SwitchLeft,
//    AutoScenario_RobotRight_SwitchRight,
//    AutoScenario_RobotRight_ScaleLeft,
//    AutoScenario_RobotRight_ScaleRight,
//    AutoScenario_RobotRight_SwitchLeft_ScaleLeft,
//    AutoScenario_RobotRight_SwitchLeft_ScaleRight,
//    AutoScenario_RobotRight_SwitchRight_ScaleLeft,
//    AutoScenario_RobotRight_SwitchRight_ScaleRight,
//    AutoScenario_RobotRight_MoveForward,

    AutoScenarioMax
};

// FUNCTIONS
int  getEncoderPulsesPerCm();

SwitchSideEnum      transformSwitchFms(unsigned char switchChar);
ScaleSideEnum       transformScaleFms(unsigned char scaleChar);
RobotStartEnum      transformConsoleSwitch1(int autoSide);
AutoObjectiveEnum   transformConsoleSwitch2(int autoMode);

AutoScenarioEnum  autoScenarioFinder(RobotStartEnum robotStart, SwitchSideEnum switchSide,
                                     ScaleSideEnum scaleSide, AutoObjectiveEnum objective);

AutoScenarioPtrType   autoScenarioFactory(AutoScenarioEnum autoScenario);

#endif // AUTO_SCENARIO_HELPERS_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
