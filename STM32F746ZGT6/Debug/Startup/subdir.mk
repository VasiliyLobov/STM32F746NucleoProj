################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f746zgtx.s 

OBJS += \
./Startup/startup_stm32f746zgtx.o 

S_DEPS += \
./Startup/startup_stm32f746zgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"G:/STM32CubeIDE/workspace_1.11.2/STM32F746NucleoProj/STM32F746ZGT6/Drivers/CMSIS/Include" -I"G:/STM32CubeIDE/workspace_1.11.2/STM32F746NucleoProj/STM32F746ZGT6/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f746zgtx.d ./Startup/startup_stm32f746zgtx.o

.PHONY: clean-Startup

