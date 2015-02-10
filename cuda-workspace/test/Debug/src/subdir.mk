################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/histogram_gold.cpp \
../src/main.cpp 

CU_SRCS += \
../src/histogram256.cu \
../src/histogram64.cu 

CU_DEPS += \
./src/histogram256.d \
./src/histogram64.d 

OBJS += \
./src/histogram256.o \
./src/histogram64.o \
./src/histogram_gold.o \
./src/main.o 

CPP_DEPS += \
./src/histogram_gold.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-6.0/bin/nvcc -I"/usr/local/cuda-6.0/samples/3_Imaging" -I"/usr/local/cuda-6.0/samples/common/inc" -I"/home/max/cuda-workspace/test" -G -g -O0 -gencode arch=compute_10,code=sm_10  -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-6.0/bin/nvcc --compile -G -I"/usr/local/cuda-6.0/samples/3_Imaging" -I"/usr/local/cuda-6.0/samples/common/inc" -I"/home/max/cuda-workspace/test" -O0 -g -gencode arch=compute_10,code=compute_10 -gencode arch=compute_10,code=sm_10  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-6.0/bin/nvcc -I"/usr/local/cuda-6.0/samples/3_Imaging" -I"/usr/local/cuda-6.0/samples/common/inc" -I"/home/max/cuda-workspace/test" -G -g -O0 -gencode arch=compute_10,code=sm_10  -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-6.0/bin/nvcc -I"/usr/local/cuda-6.0/samples/3_Imaging" -I"/usr/local/cuda-6.0/samples/common/inc" -I"/home/max/cuda-workspace/test" -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


