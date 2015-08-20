################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Prime\ Number.cpp 

OBJS += \
./src/Prime\ Number.o 

CPP_DEPS += \
./src/Prime\ Number.d 


# Each subdirectory must supply rules for building sources it contributes
src/Prime\ Number.o: ../src/Prime\ Number.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Prime Number.d" -MT"src/Prime\ Number.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


