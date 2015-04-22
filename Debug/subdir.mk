################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HeartBeat.cpp \
../HoldbackQueue.cpp \
../Members.cpp \
../MessageHistory.cpp \
../buffer_parser.cpp \
../main.cpp \
../upd_listener.cpp 

OBJS += \
./HeartBeat.o \
./HoldbackQueue.o \
./Members.o \
./MessageHistory.o \
./buffer_parser.o \
./main.o \
./upd_listener.o 

CPP_DEPS += \
./HeartBeat.d \
./HoldbackQueue.d \
./Members.d \
./MessageHistory.d \
./buffer_parser.d \
./main.d \
./upd_listener.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


