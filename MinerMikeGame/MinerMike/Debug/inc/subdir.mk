################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../inc/InputMgr.cpp 

OBJS += \
./inc/InputMgr.o 

CPP_DEPS += \
./inc/InputMgr.d 


# Each subdirectory must supply rules for building sources it contributes
inc/%.o: ../inc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/samba/sinouye/cs381/assignments/Assignment2/as4/inc" -I/usr/include/ois -I/usr/local/include/OGRE -I/usr/local/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


