################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Stm32_F103C6_Drivers/MCAL/WWDG/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/USART/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/TIMER/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/SPI/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/RCC/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/I2C/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/GPIO/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/EXTI/subdir.mk
-include Stm32_F103C6_Drivers/MCAL/ADC/subdir.mk
-include Stm32_F103C6_Drivers/HAL/LCD_Driver/subdir.mk
-include Stm32_F103C6_Drivers/HAL/KPD_Driver/subdir.mk
-include Stm32_F103C6_Drivers/HAL/EEPROM/subdir.mk
-include Stm32_F103C6_Drivers/HAL/Delay/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
GP_STM32_Drivers.axf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
GP_STM32_Drivers.list \

OBJCOPY_BIN += \
GP_STM32_Drivers.bin \


# All Target
all: GP_STM32_Drivers.axf secondary-outputs

# Tool invocations
GP_STM32_Drivers.axf: $(OBJS) $(USER_OBJS) C:\Users\ahmed\STM32CubeIDE\workspace_1.4.0\GP_STM32_Drivers\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "GP_STM32_Drivers.axf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\ahmed\STM32CubeIDE\workspace_1.4.0\GP_STM32_Drivers\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="GP_STM32_Drivers.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

GP_STM32_Drivers.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "GP_STM32_Drivers.list"
	@echo 'Finished building: $@'
	@echo ' '

GP_STM32_Drivers.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "GP_STM32_Drivers.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
