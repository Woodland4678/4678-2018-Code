################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sdk/src/arch/linux/net_serial.cpp \
../src/sdk/src/arch/linux/timer.cpp 

OBJS += \
./src/sdk/src/arch/linux/net_serial.o \
./src/sdk/src/arch/linux/timer.o 

CPP_DEPS += \
./src/sdk/src/arch/linux/net_serial.d \
./src/sdk/src/arch/linux/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdk/src/arch/linux/%.o: ../src/sdk/src/arch/linux/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Users\Cybercavs\eclipse-workspace/4678-2018-Code/src" -IC:\Users\Cybercavs/wpilib/cpp/current/sim/include -I/usr/include -I/usr/include/gazebo-5.0 -I/usr/include/sdformat-2.3 -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


