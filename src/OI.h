// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
private:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	std::shared_ptr<frc::Joystick> operate;
	std::shared_ptr<frc::JoystickButton> operatorBtn12_RJoy_PlaceHook;
	std::shared_ptr<frc::JoystickButton> operatorBtn11_LJoy_GrabHook;
	std::shared_ptr<frc::JoystickButton> operatorBtn10_Start_Portal;
	std::shared_ptr<frc::JoystickButton> operatorBtn9_Back_Home;
	std::shared_ptr<frc::JoystickButton> operatorBtn8_RTrigger_GrabCube;
	std::shared_ptr<frc::JoystickButton> operatorBtn7_LTrigger_Release;
	std::shared_ptr<frc::JoystickButton> operatorBtn6_RBump_Pickup;
	std::shared_ptr<frc::JoystickButton> operatorBtn4_Y_ScaleHigh;
	std::shared_ptr<frc::JoystickButton> operatorBtn3_B_Switch;
	std::shared_ptr<frc::JoystickButton> operatorBtn2_A_LowScale;
	std::shared_ptr<frc::JoystickButton> operatorBtn1_X_Carry;
	std::shared_ptr<frc::Joystick> driver;
	std::shared_ptr<frc::JoystickButton> driverBtn4_Y_ToggleIntakeClamp;
	std::shared_ptr<frc::JoystickButton> driverBtn10_Start_IntakeWheelsStop;
	std::shared_ptr<frc::JoystickButton> driverBtn12_RJoy_InTakeWheelsOut;
	std::shared_ptr<frc::JoystickButton> driverBtn1_X_IntakeWheelsIn;
	std::shared_ptr<frc::JoystickButton> driverBtn6_RBump_ShiftDown;
	std::shared_ptr<frc::JoystickButton> driverBtn5_LBump_ShiftUp;
	std::shared_ptr<frc::JoystickButton> driverBtn9;
	std::shared_ptr<frc::JoystickButton> driverBtn2_A_LowerIntake;
	std::shared_ptr<frc::JoystickButton> driverBtn3_B_RaiseIntake;
	std::shared_ptr<frc::JoystickButton> driverBtn8;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

	std::shared_ptr<frc::Joystick> autoSwitch;
public:
	OI();

	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=PROTOTYPES

	std::shared_ptr<frc::Joystick> getdriver();
	std::shared_ptr<frc::Joystick> getoperate();

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=PROTOTYPES
	std::shared_ptr<frc::Joystick> getAutoSwitch();

	void driverRumble(double power);

};

#endif
