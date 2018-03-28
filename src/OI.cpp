// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "OI.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "SmartDashboard/SmartDashboard.h"
#include "Commands/AutonomousCommand.h"
#include "Commands/ClampIntake.h"
#include "Commands/DriveRobot.h"
#include "Commands/FindCubes.h"
#include "Commands/GrabCube.h"
#include "Commands/IntakeWheelsControl.h"
#include "Commands/IntakeWheelsStop.h"
#include "Commands/LowerIntake.h"
#include "Commands/RaiseIntake.h"
#include "Commands/ReleaseCube.h"
#include "Commands/ShiftDown.h"
#include "Commands/ShiftToClimber.h"
#include "Commands/ShiftUp.h"
#include "Commands/ToggleIntakeClamp.h"
#include "Commands/ToggleIntakeWheels.h"
#include "Commands/TransferCube.h"
#include "Commands/UnclampIntake.h"
#include "Commands/moveArm.h"


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

OI::OI() {
    // Process operator interface input here.
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    operate.reset(new frc::Joystick(1));
    
    operatorBtn12_RJoy_MoveShDown.reset(new frc::JoystickButton(operate.get(), 12));
    operatorBtn12_RJoy_MoveShDown->WhileHeld(new moveArm(14));
    operatorBtn11_LJoy_MoveElDown.reset(new frc::JoystickButton(operate.get(), 11));
    operatorBtn11_LJoy_MoveElDown->WhileHeld(new moveArm(13));
    operatorBtn10_Start_Portal.reset(new frc::JoystickButton(operate.get(), 10));
    operatorBtn10_Start_Portal->WhenPressed(new moveArm(10));
    operatorBtn9_Back_Home.reset(new frc::JoystickButton(operate.get(), 9));
    operatorBtn9_Back_Home->WhenPressed(new moveArm(9));
    operatorBtn8_RTrigger_GrabCube.reset(new frc::JoystickButton(operate.get(), 8));
    operatorBtn8_RTrigger_GrabCube->WhenPressed(new GrabCube());
    operatorBtn7_LTrigger_Release.reset(new frc::JoystickButton(operate.get(), 7));
    operatorBtn7_LTrigger_Release->WhenPressed(new ReleaseCube());
    operatorBtn6_RBump_Pickup.reset(new frc::JoystickButton(operate.get(), 6));
    operatorBtn6_RBump_Pickup->WhenPressed(new moveArm(6));
    operatorBtn4_Y_ScaleHigh.reset(new frc::JoystickButton(operate.get(), 4));
    operatorBtn4_Y_ScaleHigh->WhenPressed(new moveArm(4));
    operatorBtn3_B_Switch.reset(new frc::JoystickButton(operate.get(), 3));
    operatorBtn3_B_Switch->WhenPressed(new moveArm(3));
    operatorBtn2_A_LowScale.reset(new frc::JoystickButton(operate.get(), 2));
    operatorBtn2_A_LowScale->WhenPressed(new moveArm(2));
    operatorBtn1_X_Carry.reset(new frc::JoystickButton(operate.get(), 1));
    operatorBtn1_X_Carry->WhenPressed(new moveArm(1));
    driver.reset(new frc::Joystick(0));
    
    driverBtn9_Back_Climber.reset(new frc::JoystickButton(driver.get(), 9));
	driverBtn9_Back_Climber->WhenPressed(new ShiftToClimber());
	driverBtn7_LTrigger_Unclamp.reset(new frc::JoystickButton(driver.get(), 7));
	driverBtn7_LTrigger_Unclamp->WhenPressed(new UnclampIntake());
	driverBtn4_Y_CubeTransfer.reset(new frc::JoystickButton(driver.get(), 4));
	driverBtn4_Y_CubeTransfer->WhenPressed(new TransferCube());
	driverBtn10_Start_FindCube.reset(new frc::JoystickButton(driver.get(), 10));
	driverBtn10_Start_FindCube->WhenPressed(new FindCubes());
	driverBtn12_RJoy_InTakeWheelsOut.reset(new frc::JoystickButton(driver.get(), 12));
	driverBtn12_RJoy_InTakeWheelsOut->WhenPressed(new IntakeWheelsControl());
	driverBtn1_X_StopWheels.reset(new frc::JoystickButton(driver.get(), 1));
	driverBtn1_X_StopWheels->WhenPressed(new IntakeWheelsStop());
	driverBtn6_RBump_ShiftUp.reset(new frc::JoystickButton(driver.get(), 6));
	driverBtn6_RBump_ShiftUp->WhenPressed(new ShiftUp());
	driverBtn5_LBump_ShiftDown.reset(new frc::JoystickButton(driver.get(), 5));
	driverBtn5_LBump_ShiftDown->WhenPressed(new ShiftDown());
	driverBtn2_A_LowerIntake.reset(new frc::JoystickButton(driver.get(), 2));
	driverBtn2_A_LowerIntake->WhenPressed(new LowerIntake());
	driverBtn3_B_RaiseIntake.reset(new frc::JoystickButton(driver.get(), 3));
	driverBtn3_B_RaiseIntake->WhenPressed(new RaiseIntake());
	driverBtn8_RTrigger_Clamp.reset(new frc::JoystickButton(driver.get(), 8));
	driverBtn8_RTrigger_Clamp->WhenPressed(new ClampIntake());

    // SmartDashboard Buttons
    frc::SmartDashboard::PutData("GrabCube", new GrabCube());
    frc::SmartDashboard::PutData("ReleaseCube", new ReleaseCube());
    frc::SmartDashboard::PutData("ToggleIntakeWheels", new ToggleIntakeWheels());
    frc::SmartDashboard::PutData("ShiftDown", new ShiftDown());
    frc::SmartDashboard::PutData("ShiftUp", new ShiftUp());
    frc::SmartDashboard::PutData("LowerIntake", new LowerIntake());
    frc::SmartDashboard::PutData("RaiseIntake", new RaiseIntake());
    frc::SmartDashboard::PutData("Autonomous Command", new AutonomousCommand());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS

    autoSwitch.reset(new frc::Joystick(2));
}

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS

std::shared_ptr<frc::Joystick> OI::getdriver() {
   return driver;
}

std::shared_ptr<frc::Joystick> OI::getoperate() {
   return operate;
}


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS

void OI::driverRumble(double power) {
	driver->SetRumble(frc::GenericHID::RumbleType::kLeftRumble, power);
}

std::shared_ptr<frc::Joystick> OI::getAutoSwitch() {
	return autoSwitch;
}
