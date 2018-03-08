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
#include "Station1CrossLine.h"
#include "Station1SwitchLeft.h"
#include "Station1SwitchRight.h"
#include "LeftSideLeftScale.h"
#include "LeftSideLeftSwitch.h"
//#include "Station1ScaleLeft.h"
#include "Station1ScaleRight.h"
#include "LeftSideLeftScaleRightSwitch.h"

//#include "Station1SwitchLeftScaleLeft.h"
//#include "Station1SwitchLeftScaleRight.h"
//#include "Station1SwitchRightScaleLeft.h"
//#include "Station1SwitchRightScaleRight.h"

//#include "Station2CrossLine.h"
//#include "Station21SwitchLeft.h"
//#include "Station21SwitchRight.h"
//#include "Station21ScaleLeft.h"
//#include "Station21ScaleRight.h"
//#include "Station21SwitchLeftScaleLeft.h"
//#include "Station21SwitchLeftScaleRight.h"
//#include "Station21SwitchRightScaleLeft.h"
//#include "Station2SwitchRightScaleRight.h"

#include "Station3CrossLine.h"
//#include "Station3SwitchLeft.h"
//#include "Station3SwitchRight.h"
//#include "Station3ScaleLeft.h"
//#include "Station3ScaleRight.h"
//#include "Station3SwitchLeftScaleLeft.h"
//#include "Station3SwitchLeftScaleRight.h"
//#include "Station3SwitchRightScaleLeft.h"
//#include "Station3SwitchRightScaleRight.h"


AutoScenarioPtrType   autoScenarioFactory(AutoScenarioEnum autoScenario)  {
	AutoScenarioPtrType   scenario_ptr;

    switch (autoScenario) {

    case AutoScenario_Station1_CrossLine:
        scenario_ptr.reset(new Station1CrossLine);
        break;

    case AutoScenario_Station1_SwitchLeft:
        scenario_ptr.reset(new Station1SwitchLeft);
        break;

    case AutoScenario_Station1_SwitchRight:
        scenario_ptr.reset(new Station1SwitchRight);
        break;

    case AutoScenario_LeftSide_LeftScale:
        scenario_ptr.reset(new LeftSideLeftScale);
        break;

    case AutoScenario_LeftSide_LeftSwitch:
		scenario_ptr.reset(new LeftSideLeftSwitch);
		break;

    case AutoScenario_LeftSide_LeftScale_RightSwitch:
		scenario_ptr.reset(new LeftSideLeftScaleRightSwitch);
		break;

    case AutoScenario_Station1_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station1_ScaleRight:
        scenario_ptr.reset(new Station1ScaleRight);
        break;

    case AutoScenario_Station1_SwitchLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station1_SwitchLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station1_SwitchRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station1_SwitchRight_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_CrossLine:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_SwitchLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_SwitchRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_SwitchLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_SwitchLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_SwitchRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station2_SwitchRight_ScaleRight:
 //       scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_CrossLine:
        scenario_ptr.reset(new Station3CrossLine);
        break;

    case AutoScenario_Station3_SwitchLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_SwitchRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_SwitchLeft_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_SwitchLeft_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_SwitchRight_ScaleLeft:
//        scenario_ptr.reset(new XXX);
        break;

    case AutoScenario_Station3_SwitchRight_ScaleRight:
//        scenario_ptr.reset(new XXX);
        break;

    default:
        break;
    }

    return scenario_ptr;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
