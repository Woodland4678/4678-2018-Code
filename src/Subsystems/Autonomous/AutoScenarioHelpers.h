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
// Defines what station the robot is starting from
enum StartStationEnum  {
    StartStationUnknown = 0,
    StartStation1,
    StartStation2,
    StartStation3,
    StartStationMax
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
    AutoObjectiveCrossLine,
    AutoObjectiveSwitch,
    AutoObjectiveScale,
    AutoObjectiveSwitchScale,
    AutoObjectiveMax
};

// Defines what scenarion we want the robot to run for autonomous
enum AutoScenarioEnum  {
    AutoScenarioUnknown = 0,

    AutoScenario_Station1_CrossLine,
    AutoScenario_Station1_SwitchLeft,
    AutoScenario_Station1_SwitchRight,
    AutoScenario_Station1_ScaleLeft,
    AutoScenario_Station1_ScaleRight,
    AutoScenario_Station1_SwitchLeft_ScaleLeft,
    AutoScenario_Station1_SwitchLeft_ScaleRight,
    AutoScenario_Station1_SwitchRight_ScaleLeft,
    AutoScenario_Station1_SwitchRight_ScaleRight,

    AutoScenario_Station2_CrossLine,
    AutoScenario_Station2_SwitchLeft,
    AutoScenario_Station2_SwitchRight,
    AutoScenario_Station2_ScaleLeft,
    AutoScenario_Station2_ScaleRight,
    AutoScenario_Station2_SwitchLeft_ScaleLeft,
    AutoScenario_Station2_SwitchLeft_ScaleRight,
    AutoScenario_Station2_SwitchRight_ScaleLeft,
    AutoScenario_Station2_SwitchRight_ScaleRight,

    AutoScenario_Station3_CrossLine,
    AutoScenario_Station3_SwitchLeft,
    AutoScenario_Station3_SwitchRight,
    AutoScenario_Station3_ScaleLeft,
    AutoScenario_Station3_ScaleRight,
    AutoScenario_Station3_SwitchLeft_ScaleLeft,
    AutoScenario_Station3_SwitchLeft_ScaleRight,
    AutoScenario_Station3_SwitchRight_ScaleLeft,
    AutoScenario_Station3_SwitchRight_ScaleRight,

	AutoScenario_LeftSide_LeftScale,
	AutoScenario_LeftSide_LeftSwitch,

    AutoScenarioMax
};

// FUNCTIONS
int  getEncoderPulsesPerCm();

SwitchSideEnum      transformSwitchFms(unsigned char switchChar);
ScaleSideEnum       transformScaleFms(unsigned char scaleChar);
StartStationEnum    transformConsoleSide(int autoSide);
AutoObjectiveEnum   transformConsoleMode(int autoMode);

AutoScenarioEnum  autoScenarioFinder(StartStationEnum station, SwitchSideEnum switchSide, ScaleSideEnum scaleSide, AutoObjectiveEnum objective);

AutoScenarioPtrType   autoScenarioFactory(AutoScenarioEnum autoScenario);

#endif // AUTO_SCENARIO_HELPERS_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
