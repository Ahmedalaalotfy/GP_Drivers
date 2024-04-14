################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/USART" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/FreeRTOS/portable/ARM_CM3" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/FreeRTOS/include" -I../Inc -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/Pedal" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/WWDG" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/AhmedLotfy_RTOS/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/CMSIS_v5" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/Delay" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/ADC" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/EXTI" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/GPIO" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/I2C" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/RCC" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/SPI" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/TIMER" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/EEPROM" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/LCD_Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

