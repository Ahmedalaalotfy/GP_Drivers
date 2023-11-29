################################################################################
# Automatically-generated file. Do not edit!
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
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/Delay" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/TIMER" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/AhmedLotfy_RTOS/inc" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/CMSIS_v5" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/EEPROM" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/I2C" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/SPI" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/USART" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/RCC" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/EXTI" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/GPIO" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/LCD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/Delay" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/TIMER" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/AhmedLotfy_RTOS/inc" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/CMSIS_v5" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/EEPROM" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/I2C" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/SPI" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/USART" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/RCC" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/EXTI" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/GPIO" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/LCD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/Delay" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/TIMER" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/AhmedLotfy_RTOS/inc" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/CMSIS_v5" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/EEPROM" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/I2C" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/SPI" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/USART" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/RCC" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/EXTI" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/MCAL/GPIO" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/HAL/LCD_Driver" -I"M:/Study/College/Graduation Project/ADAS/Drivers_GP/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

