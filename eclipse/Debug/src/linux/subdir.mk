################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/komato3/workspace/Biominator/jni/linux/core.cpp \
/home/komato3/workspace/Biominator/jni/linux/entrypoint.cpp \
/home/komato3/workspace/Biominator/jni/linux/renderer.cpp 

OBJS += \
./src/linux/core.o \
./src/linux/entrypoint.o \
./src/linux/renderer.o 

CPP_DEPS += \
./src/linux/core.d \
./src/linux/entrypoint.d \
./src/linux/renderer.d 


# Each subdirectory must supply rules for building sources it contributes
src/linux/core.o: /home/komato3/workspace/Biominator/jni/linux/core.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/linux/entrypoint.o: /home/komato3/workspace/Biominator/jni/linux/entrypoint.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/linux/renderer.o: /home/komato3/workspace/Biominator/jni/linux/renderer.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


