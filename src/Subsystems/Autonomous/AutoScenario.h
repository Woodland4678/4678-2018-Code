/*
 * AutoScenario.h
 *
 *  Created on: Feb 24, 2018
 *      Author: wchs
 */

#ifndef AUTO_SCENARIO_H_
#define AUTO_SCENARIO_H_

// SYSTEM INCLUDES
#include <memory>                     // robot code version

// FORWARD DECLARATION
class AutoScenario;

// TYPEDEFS
typedef std::shared_ptr<AutoScenario>        AutoScenarioPtrType;     // robot code version

// ENUMS
// The following enum can be shared among all derived scenario classes for keeping track of the state machines
// If more scenarios are required then add them before ScenarioStateMax
enum AutoScenarioStateEnum {
	ScenarioStateUnknown = 0,
	ScenarioState1,
	ScenarioState2,
	ScenarioState3,
	ScenarioState4,
	ScenarioState5,
	ScenarioState6,
	ScenarioState7,
	ScenarioState8,
	ScenarioState9,
	ScenarioState10,
    ScenarioState11,
    ScenarioState12,
    ScenarioState13,
    ScenarioState14,
    ScenarioState15,
    ScenarioState16,
	ScenarioStateMax
};


// CLASS DECLARATION
class AutoScenario
{
public:
// LIFECYCLE
	AutoScenario();
    virtual ~AutoScenario();

// VIRTUAL METHODS - default implementations provided to do nothing
    virtual void  initialize();
    virtual void  execute();
    virtual bool  isFinished();
    virtual void  end();
    virtual void  interrupted();

protected:
    void  setFinished(bool value = true);

private:
// LIFECYCLE
    AutoScenario(const AutoScenario &);		// Hide the copy constructor

// OPERATORS
    AutoScenario & operator=(const AutoScenario &);  // Hide the assignment operator

// MEMBERS
    bool    m_finished;
};

#endif // AUTO_SCENARIO_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
