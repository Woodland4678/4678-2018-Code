// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"
#include "ctre/Phoenix.h"

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION
std::shared_ptr<frc::SpeedController> RobotMap::driveTrainrightMotor;
std::shared_ptr<frc::SpeedController> RobotMap::driveTrainleftMotor;
std::shared_ptr<frc::Encoder> RobotMap::driveTrainrightEncoderDrive;
std::shared_ptr<frc::Encoder> RobotMap::driveTrainleftEncoderDrive;
std::shared_ptr<frc::Compressor> RobotMap::driveTrainCompressor;
std::shared_ptr<frc::DoubleSolenoid> RobotMap::driveTrainshoulderClimber;
std::shared_ptr<frc::Solenoid> RobotMap::driveTrainshifter;
std::shared_ptr<frc::PowerDistributionPanel> RobotMap::driveTrainpdp;
std::shared_ptr<WPI_TalonSRX> RobotMap::manipulatorArmshoulder;
std::shared_ptr<WPI_TalonSRX> RobotMap::manipulatorArmelbow;
std::shared_ptr<WPI_TalonSRX> RobotMap::manipulatorArmwrist;
std::shared_ptr<WPI_VictorSPX> RobotMap::manipulatorArmshoulderSlave;
std::shared_ptr<WPI_VictorSPX> RobotMap::manipulatorArmelbowSlave;
std::shared_ptr<frc::DoubleSolenoid> RobotMap::manipulatorArmclaw;
std::shared_ptr<WPI_TalonSRX> RobotMap::intakelifter;
std::shared_ptr<frc::SpeedController> RobotMap::intakerightWheels;
std::shared_ptr<frc::SpeedController> RobotMap::intakeleftWheels;
std::shared_ptr<frc::Solenoid> RobotMap::intakereleaser;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION

void RobotMap::init() {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    frc::LiveWindow *lw = frc::LiveWindow::GetInstance();

    driveTrainrightMotor.reset(new frc::VictorSP(1));
    lw->AddActuator("DriveTrain", "rightMotor", std::static_pointer_cast<frc::VictorSP>(driveTrainrightMotor));
    
    driveTrainleftMotor.reset(new frc::VictorSP(0));
    lw->AddActuator("DriveTrain", "leftMotor", std::static_pointer_cast<frc::VictorSP>(driveTrainleftMotor));
    
    driveTrainrightEncoderDrive.reset(new frc::Encoder(0, 1, false, frc::Encoder::k4X));
    lw->AddSensor("DriveTrain", "rightEncoderDrive", driveTrainrightEncoderDrive);
    driveTrainrightEncoderDrive->SetDistancePerPulse(1.0);
    driveTrainrightEncoderDrive->SetPIDSourceType(frc::PIDSourceType::kRate);
    driveTrainleftEncoderDrive.reset(new frc::Encoder(2, 3, false, frc::Encoder::k4X));
    lw->AddSensor("DriveTrain", "leftEncoderDrive", driveTrainleftEncoderDrive);
    driveTrainleftEncoderDrive->SetDistancePerPulse(1.0);
    driveTrainleftEncoderDrive->SetPIDSourceType(frc::PIDSourceType::kRate);
    driveTrainCompressor.reset(new frc::Compressor(0));
    lw->AddActuator("DriveTrain", "Compressor", driveTrainCompressor);
    
    driveTrainshoulderClimber.reset(new frc::DoubleSolenoid(0, 2, 3));
    lw->AddActuator("DriveTrain", "shoulderClimber", driveTrainshoulderClimber);
    
    driveTrainshifter.reset(new frc::Solenoid(0, 5));
    lw->AddActuator("DriveTrain", "shifter", driveTrainshifter);
    
    driveTrainpdp.reset(new frc::PowerDistributionPanel(8));
    lw->AddSensor("DriveTrain", "pdp", driveTrainpdp);
    
    manipulatorArmshoulder.reset(new WPI_TalonSRX(3));
    
    
    manipulatorArmelbow.reset(new WPI_TalonSRX(5));
    
    
    manipulatorArmwrist.reset(new WPI_TalonSRX(2));
    
    
    manipulatorArmshoulderSlave.reset(new WPI_VictorSPX(4));
    
    
    manipulatorArmelbowSlave.reset(new WPI_VictorSPX(6));
    
    
    manipulatorArmclaw.reset(new frc::DoubleSolenoid(0, 0, 1));
    lw->AddActuator("ManipulatorArm", "claw", manipulatorArmclaw);
    
    intakelifter.reset(new WPI_TalonSRX(1));
    
    
    intakerightWheels.reset(new frc::VictorSP(2));
    lw->AddActuator("Intake", "rightWheels", std::static_pointer_cast<frc::VictorSP>(intakerightWheels));
    
    intakeleftWheels.reset(new frc::VictorSP(3));
    lw->AddActuator("Intake", "leftWheels", std::static_pointer_cast<frc::VictorSP>(intakeleftWheels));
    
    intakereleaser.reset(new frc::Solenoid(0, 4));
    lw->AddActuator("Intake", "releaser", intakereleaser);
    


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
}
