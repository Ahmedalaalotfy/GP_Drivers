################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.c 

OBJS += \
./Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.o 

C_DEPS += \
./Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
Stm32_F103C6_Drivers/MCAL/USART/%.o Stm32_F103C6_Drivers/MCAL/USART/%.su Stm32_F103C6_Drivers/MCAL/USART/%.cyclo: ../Stm32_F103C6_Drivers/MCAL/USART/%.c Stm32_F103C6_Drivers/MCAL/USART/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/USART" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/FreeRTOS/portable/ARM_CM3" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/FreeRTOS/include" -I../Inc -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/Pedal" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/WWDG" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/AhmedLotfy_RTOS/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/CMSIS_v5" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/Delay" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/ADC" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/EXTI" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/GPIO" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/I2C" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/RCC" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/SPI" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/MCAL/TIMER" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/EEPROM" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/KPD_Driver" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/GP_STM32_Drivers/Stm32_F103C6_Drivers/HAL/LCD_Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Stm32_F103C6_Drivers-2f-MCAL-2f-USART

clean-Stm32_F103C6_Drivers-2f-MCAL-2f-USART:
	-$(RM) ./Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.cyclo ./Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.d ./Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.o ./Stm32_F103C6_Drivers/MCAL/USART/Stm32_F103C6_USART_Driver.su

.PHONY: clean-Stm32_F103C6_Drivers-2f-MCAL-2f-USART

