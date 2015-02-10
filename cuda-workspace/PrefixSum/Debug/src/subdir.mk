################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../src/PrefixSum.cu 

CU_DEPS += \
./src/PrefixSum.d 

OBJS += \
./src/PrefixSum.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-6.0/bin/nvcc -G -g -O0 -gencode arch=compute_10,code=sm_10  -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-6.0/bin/nvcc --compile -G -O0 -g -gencode arch=compute_10,code=compute_10 -gencode arch=compute_10,code=sm_10  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


