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
    StartStationEnum    m_station;
    SwitchSideEnum      m_switchSide;
    ScaleSideEnum       m_scaleSide;
    AutoObjectiveEnum   m_objective;
    AutoScenarioEnum    m_scenario;
};

// Array defining the inputs and the scenarios
static AutoScenarioDefn  s_AutoScenarioDefnArray[] = {
    { StartStation1, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveCrossLine,   AutoScenario_Station1_CrossLine },
    { StartStation1, SwitchSideLeft,     ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_Station1_SwitchLeft },
    { StartStation1, SwitchSideRight,    ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_Station1_SwitchRight },
    { StartStation1, SwitchSideWildcard, ScaleSideLeft,     AutoObjectiveScale,       AutoScenario_Station1_ScaleLeft },
    { StartStation1, SwitchSideWildcard, ScaleSideRight,    AutoObjectiveScale,       AutoScenario_Station1_ScaleRight },
    { StartStation1, SwitchSideLeft,     ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_Station1_SwitchLeft_ScaleLeft },
    { StartStation1, SwitchSideLeft,     ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_Station1_SwitchLeft_ScaleRight },
    { StartStation1, SwitchSideRight,    ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_Station1_SwitchRight_ScaleLeft },
    { StartStation1, SwitchSideRight,    ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_Station1_SwitchRight_ScaleRight },

    { StartStation2, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveCrossLine,   AutoScenario_Station2_CrossLine },
    { StartStation2, SwitchSideLeft,     ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_Station2_SwitchLeft },
    { StartStation2, SwitchSideRight,    ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_Station2_SwitchRight },
    { StartStation2, SwitchSideWildcard, ScaleSideLeft,     AutoObjectiveScale,       AutoScenario_Station2_ScaleLeft },
    { StartStation2, SwitchSideWildcard, ScaleSideRight,    AutoObjectiveScale,       AutoScenario_Station2_ScaleRight },
    { StartStation2, SwitchSideLeft,     ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_Station2_SwitchLeft_ScaleLeft },
    { StartStation2, SwitchSideLeft,     ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_Station2_SwitchLeft_ScaleRight },
    { StartStation2, SwitchSideRight,    ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_Station2_SwitchRight_ScaleLeft },
    { StartStation2, SwitchSideRight,    ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_Station2_SwitchRight_ScaleRight },

    { StartStation3, SwitchSideWildcard, ScaleSideWildcard, AutoObjectiveCrossLine,   AutoScenario_Station3_CrossLine },
    { StartStation3, SwitchSideLeft,     ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_Station3_SwitchLeft },
    { StartStation3, SwitchSideRight,    ScaleSideWildcard, AutoObjectiveSwitch,      AutoScenario_Station3_SwitchRight },
    { StartStation3, SwitchSideWildcard, ScaleSideLeft,     AutoObjectiveScale,       AutoScenario_Station3_ScaleLeft },
    { StartStation3, SwitchSideWildcard, ScaleSideRight,    AutoObjectiveScale,       AutoScenario_Station3_ScaleRight },
    { StartStation3, SwitchSideLeft,     ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_Station3_SwitchLeft_ScaleLeft },
    { StartStation3, SwitchSideLeft,     ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_Station3_SwitchLeft_ScaleRight },
    { StartStation3, SwitchSideRight,    ScaleSideLeft,     AutoObjectiveSwitchScale, AutoScenario_Station3_SwitchRight_ScaleLeft },
    { StartStation3, SwitchSideRight,    ScaleSideRight,    AutoObjectiveSwitchScale, AutoScenario_Station3_SwitchRight_ScaleRight }
};


// MAGIC VALUES
static const int   s_AutoScenarioArraySize(sizeof(s_AutoScenarioDefnArray) / sizeof(AutoScenarioDefn));


// FUNCTIONS

AutoScenarioEnum  autoScenarioFinder(StartStationEnum station, SwitchSideEnum switchSide, ScaleSideEnum scaleSide, AutoObjectiveEnum objective) {

    for (int i = 0; i < s_AutoScenarioArraySize; ++i)  {
        if (station == s_AutoScenarioDefnArray[i].m_station &&
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
