################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BufferParser.cpp \
../HeartBeat.cpp \
../HoldbackQueue.cpp \
../Members.cpp \
../MessageStore.cpp \
../UDP.cpp \
../main.cpp 

OBJS += \
./BufferParser.o \
./HeartBeat.o \
./HoldbackQueue.o \
./Members.o \
./MessageStore.o \
./UDP.o \
./main.o 

CPP_DEPS += \
./BufferParser.d \
./HeartBeat.d \
./HoldbackQueue.d \
./Members.d \
./MessageStore.d \
./UDP.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


