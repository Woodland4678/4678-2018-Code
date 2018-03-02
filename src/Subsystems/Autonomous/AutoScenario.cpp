/*
 * AutoScenario.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

// SYSTEM INCLUDES
//#include <iostream>

// PROJECT INCLUDES
#include "AutoScenario.h"

// //////////////////////////////////  PUBLIC ////////////////////////////////////////////////
// ********************************  LIFECYCLE  *********************************************

AutoScenario::AutoScenario() :
    m_finished(false)
{ }

AutoScenario::~AutoScenario() {
}

// **********************************  METHODS  **********************************************

void  AutoScenario::initialize() {

//    std::cout << "AutoScenario::initialize() called" << std::endl;
}

void  AutoScenario::execute() {

//    std::cout << "AutoScenario::execute() called" << std::endl;
}

bool  AutoScenario::isFinished() {

//    std::cout << "AutoScenario::isFinished() called" << std::endl;
    return m_finished;
}

void  AutoScenario::end() {

//    std::cout << "AutoScenario::end() called" << std::endl;
}

void  AutoScenario::interrupted() {

//    std::cout << "AutoScenario::interrupted() called" << std::endl;
}

// /////////////////////////////////  PROTECTED //////////////////////////////////////////////
// **********************************  METHODS  **********************************************

void  AutoScenario::setFinished(bool value) {

	m_finished = value;
}

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
