################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Stm32_F103C6_Drivers/MCAL/WWDG/Stm32_F103C8_WWDG_Driver.c 

OBJS += \
./Stm32_F103C6_Drivers/MCAL/WWDG/Stm32_F103C8_WWDG_Driver.o 

C_DEPS += \
./Stm32_F103C6_Drivers/MCAL/WWDG/Stm32_F103C8_WWDG_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
Stm32_F103C6_Drivers/MCAL/WWDG/Stm32_F103C8_WWDG_Driver.o: ../Stm32_F103C6_Drivers/MCAL/WWDG/Stm32_F103C8_WWDG_Driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/WWDG" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/AhmedLotfy_RTOS/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/CMSIS_v5" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/Delay" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/ADC" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/EXTI" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/GPIO" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/I2C" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/RCC" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/SPI" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/TIMER" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/USART" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/EEPROM" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/LCD_Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Stm32_F103C6_Drivers/MCAL/WWDG/Stm32_F103C8_WWDG_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

