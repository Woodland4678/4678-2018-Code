################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Subsystems/ClydeIntake.cpp \
../src/Subsystems/DriveTrain.cpp \
../src/Subsystems/Lidar.cpp \
../src/Subsystems/ManipulatorArm.cpp \
../src/Subsystems/ScaleDetection.cpp 

OBJS += \
./src/Subsystems/ClydeIntake.o \
./src/Subsystems/DriveTrain.o \
./src/Subsystems/Lidar.o \
./src/Subsystems/ManipulatorArm.o \
./src/Subsystems/ScaleDetection.o 

CPP_DEPS += \
./src/Subsystems/ClydeIntake.d \
./src/Subsystems/DriveTrain.d \
./src/Subsystems/Lidar.d \
./src/Subsystems/ManipulatorArm.d \
./src/Subsystems/ScaleDetection.d 


# Each subdirectory must supply rules for building sources it contributes
src/Subsystems/%.o: ../src/Subsystems/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Users\Cybercavs\eclipse-workspace/4678-2018-Code/src" -IC:\Users\Cybercavs/wpilib/cpp/current/sim/include -I/usr/include -I/usr/include/gazebo-5.0 -I/usr/include/sdformat-2.3 -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


