/*
  * AutoScenarioFactory.cpp
 *
 *  Created on: Feb 23, 2018
 *      Author: wchs
 */

// Developer notes:
// This file contains the factory code for generating a game scenario

// SYSTEM INCLUDES
// PROJECT INCLUDES
#include "AutoScenarioHelpers.h"


// PUBLIC FUNCTIONS
AutoScenarioPtrType   autoScenarioFactory(AutoScenarioEnum autoScenario)  {
	AutoScenarioPtrType   scenario_ptr;

    switch (autoScenario) {

    // **********  RobotLeft cases  **********

    case AutoScenario_RobotLeft_NoAuto:
        // Do nothing
        break;

//    case AutoScenario_RobotLeft_SwitchLeft:
//        scenario_ptr.reset(new Station1SwitchLeft);
//        break;

//    case AutoScenario_RobotLeft_SwitchRight:
//        scenario_ptr.reset(new Station1SwitchRight);
//        break;

    case AutoScenario_RobotLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_RobotLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

//    case AutoScenario_RobotLeft_SwitchLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotLeft_SwitchLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotLeft_SwitchRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotLeft_SwitchRight_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotLeft_MoveForward:
//        scenario_ptr.reset(new RobotCrossLine);
//        break;

    // **********  RobotMiddle cases  **********

    case AutoScenario_RobotMiddle_NoAuto:
        // Do nothing
        break;

    case AutoScenario_RobotMiddle_SwitchLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_RobotMiddle_SwitchRight:
//        scenario_ptr.reset(new XXX);
        break;

//    case AutoScenario_RobotMiddle_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotMiddle_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotMiddle_SwitchLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotMiddle_SwitchLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotMiddle_SwitchRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotMiddle_SwitchRight_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotMiddle_MoveForward:
//        scenario_ptr.reset(new XXX);
//        break;

    // **********  RobotRight cases  **********

    case AutoScenario_RobotRight_NoAuto:
        // Do nothing
        break;

//    case AutoScenario_RobotRight_SwitchLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_SwitchRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_SwitchLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_SwitchLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_SwitchRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_SwitchRight_ScaleRight:
//        scenario_ptr.reset(new XXX);
//        break;

//    case AutoScenario_RobotRight_MoveForward:
//        scenario_ptr.reset(new RobotCrossLine);
//        break;

    default:
        break;
    }

    return scenario_ptr;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
