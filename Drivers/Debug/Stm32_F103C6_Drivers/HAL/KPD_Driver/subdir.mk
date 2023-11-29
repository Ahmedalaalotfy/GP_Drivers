################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Stm32_F103C6_Drivers/HAL/KPD_Driver/KPD_program.c 

OBJS += \
./Stm32_F103C6_Drivers/HAL/KPD_Driver/KPD_program.o 

C_DEPS += \
./Stm32_F103C6_Drivers/HAL/KPD_Driver/KPD_program.d 


# Each subdirectory must supply rules for building sources it contributes
Stm32_F103C6_Drivers/HAL/KPD_Driver/KPD_program.o: ../Stm32_F103C6_Drivers/HAL/KPD_Driver/KPD_program.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/HAL/Delay" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/TIMER" -I"M:/Study/College/Graduation Project/ADAS/Drivers/AhmedLotfy_RTOS/inc" -I"M:/Study/College/Graduation Project/ADAS/Drivers/CMSIS_v5" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/HAL/EEPROM" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/I2C" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/SPI" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/USART" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/RCC" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/EXTI" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/MCAL/GPIO" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/HAL/LCD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Stm32_F103C6_Drivers/HAL/KPD_Driver/KPD_program.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

