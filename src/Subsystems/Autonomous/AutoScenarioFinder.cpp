/*
 * AutoScenarioFinder.cpp
 *
 *  Created on: Feb 23, 2018
 *      Author: wchs
 */

// Developer notes:
// This file contains the code for translating the different inputs (station, switch, scale, objective) into a game scenario

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"

// STRUCTS
struct AutoScenarioDefn {
    RobotStartEnum      m_robotStart;
    SwitchSideEnum      m_switchSide;
    ScaleSideEnum       m_scaleSide;
    AutoObjectiveEnum   m_objective;
    AutoScenarioEnum    m_scenario;
};

// Array defining the inputs and the scenarios
static AutoScenarioDefn  s_AutoScenarioDefnArray[] = {
    { RobotStartLeft, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveNoAuto,      AutoScenario_RobotLeft_NoAuto },
//    { RobotStartLeft, SwitchSideLeft,     ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_RobotLeft_SwitchLeft },
//    { RobotStartLeft, SwitchSideRight,    ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_RobotLeft_SwitchRight },
    { RobotStartLeft, SwitchSideWildcard, ScaleSideLeft,     AutoObjectiveScale,       AutoScenario_RobotLeft_ScaleLeft },
    { RobotStartLeft, SwitchSideWildcard, ScaleSideRight,    AutoObjectiveScale,       AutoScenario_RobotLeft_ScaleRight },
//    { RobotStartLeft, SwitchSideLeft,     ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_RobotLeft_SwitchLeft_ScaleLeft },
//    { RobotStartLeft, SwitchSideLeft,     ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_RobotLeft_SwitchLeft_ScaleRight },
//    { RobotStartLeft, SwitchSideRight,    ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_RobotLeft_SwitchRight_ScaleLeft },
//    { RobotStartLeft, SwitchSideRight,    ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_RobotLeft_SwitchRight_ScaleRight },
//    { RobotStartLeft, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveMoveForward, AutoScenario_RobotLeft_MoveForward },

    { RobotStartMiddle, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveNoAuto,      AutoScenario_RobotMiddle_NoAuto },
    { RobotStartMiddle, SwitchSideLeft,     ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_RobotMiddle_SwitchLeft },
    { RobotStartMiddle, SwitchSideRight,    ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_RobotMiddle_SwitchRight },
//    { RobotStartMiddle, SwitchSideWildcard, ScaleSideLeft,     AutoObjectiveScale,       AutoScenario_RobotMiddle_ScaleLeft },
//    { RobotStartMiddle, SwitchSideWildcard, ScaleSideRight,    AutoObjectiveScale,       AutoScenario_RobotMiddle_ScaleRight },
//    { RobotStartMiddle, SwitchSideLeft,     ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_RobotMiddle_SwitchLeft_ScaleLeft },
//    { RobotStartMiddle, SwitchSideLeft,     ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_RobotMiddle_SwitchLeft_ScaleRight },
//    { RobotStartMiddle, SwitchSideRight,    ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_RobotMiddle_SwitchRight_ScaleLeft },
//    { RobotStartMiddle, SwitchSideRight,    ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_RobotMiddle_SwitchRight_ScaleRight },
//    { RobotStartMiddle, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveMoveForward, AutoScenario_RobotMiddle_MoveForward },

    { RobotStartRight, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveNoAuto,      AutoScenario_RobotRight_NoAuto },
//    { RobotStartRight, SwitchSideLeft,     ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_RobotRight_SwitchLeft },
//    { RobotStartRight, SwitchSideRight,    ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_RobotRight_SwitchRight },
//    { RobotStartRight, SwitchSideWildcard, ScaleSideLeft,     AutoObjectiveScale,       AutoScenario_RobotRight_ScaleLeft },
//    { RobotStartRight, SwitchSideWildcard, ScaleSideRight,    AutoObjectiveScale,       AutoScenario_RobotRight_ScaleRight },
//    { RobotStartRight, SwitchSideLeft,     ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_RobotRight_SwitchLeft_ScaleLeft },
//    { RobotStartRight, SwitchSideLeft,     ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_RobotRight_SwitchLeft_ScaleRight },
//    { RobotStartRight, SwitchSideRight,    ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_RobotRight_SwitchRight_ScaleLeft },
//    { RobotStartRight, SwitchSideRight,    ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_RobotRight_SwitchRight_ScaleRight },
//    { RobotStartRight, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveMoveForward, AutoScenario_RobotRight_MoveForward }
};


// MAGIC VALUES
static const int   s_AutoScenarioArraySize(sizeof(s_AutoScenarioDefnArray) / sizeof(AutoScenarioDefn));


// PUBLIC FUNCTIONS

AutoScenarioEnum  autoScenarioFinder(RobotStartEnum robotStart, SwitchSideEnum switchSide, ScaleSideEnum scaleSide, AutoObjectiveEnum objective) {

    for (int i = 0; i < s_AutoScenarioArraySize; ++i)  {
        if (robotStart == s_AutoScenarioDefnArray[i].m_robotStart &&
            (s_AutoScenarioDefnArray[i].m_switchSide == SwitchSideWildcard || switchSide == s_AutoScenarioDefnArray[i].m_switchSide) &&
            (s_AutoScenarioDefnArray[i].m_scaleSide == ScaleSideWildcard || scaleSide == s_AutoScenarioDefnArray[i].m_scaleSide) &&
             objective == s_AutoScenarioDefnArray[i].m_objective)  {
            // We have a winner!
           	return s_AutoScenarioDefnArray[i].m_scenario;
        }
    }

    return AutoScenarioUnknown;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
