/*
 * DriveStateObserver.h
 *
 *  Created on: Feb 18, 2018
 *      Author: wchs
 */

#ifndef DRIVE_STATE_OBSERVER_H_
#define DRIVE_STATE_OBSERVER_H_

// SYSTEM INCLUDES
#include <memory>                     // robot code version

// FORWARD DECLARATION
class DriveStateObserver;

// TYPEDEFS
typedef std::shared_ptr<DriveStateObserver>    StateObserverPtrType;     // robot code version

// CLASS DECLARATION
class DriveStateObserver
{
public:
// LIFECYCLE
    DriveStateObserver();
    virtual ~DriveStateObserver();

// VIRTUAL METHODS - default implementations provided to do nothing
    virtual void  startStartUpState();
    virtual void  startRampUpState();
    virtual void  startTravelState();
    virtual void  startRampDownState();
    virtual void  startFinishState();

private:
    // TODO: change the constructors and operators to c11
// LIFECYCLE
    DriveStateObserver(const DriveStateObserver &);		// Hide the copy constructor

// OPERATORS
    DriveStateObserver & operator=(const DriveStateObserver &);  // Hide the assignment operator
};

#endif // DRIVE_STATE_OBSERVER_H_

// //////////////////////////////////////  EOF ////////////////////////////////////////////////////
