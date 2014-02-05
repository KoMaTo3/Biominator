################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/komato3/workspace/Biominator/jni/math/kangles.cpp \
/home/komato3/workspace/Biominator/jni/math/klib.cpp \
/home/komato3/workspace/Biominator/jni/math/kmath.cpp \
/home/komato3/workspace/Biominator/jni/math/kmatrix.cpp \
/home/komato3/workspace/Biominator/jni/math/kvector.cpp 

OBJS += \
./src/math/kangles.o \
./src/math/klib.o \
./src/math/kmath.o \
./src/math/kmatrix.o \
./src/math/kvector.o 

CPP_DEPS += \
./src/math/kangles.d \
./src/math/klib.d \
./src/math/kmath.d \
./src/math/kmatrix.d \
./src/math/kvector.d 


# Each subdirectory must supply rules for building sources it contributes
src/math/kangles.o: /home/komato3/workspace/Biominator/jni/math/kangles.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/math/klib.o: /home/komato3/workspace/Biominator/jni/math/klib.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/math/kmath.o: /home/komato3/workspace/Biominator/jni/math/kmath.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/math/kmatrix.o: /home/komato3/workspace/Biominator/jni/math/kmatrix.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/math/kvector.o: /home/komato3/workspace/Biominator/jni/math/kvector.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


