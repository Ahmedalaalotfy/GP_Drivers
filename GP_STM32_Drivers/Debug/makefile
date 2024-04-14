################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
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
-include Stm32_F103C6_Drivers/HAL/URM37/subdir.mk
-include Stm32_F103C6_Drivers/HAL/Pedal/subdir.mk
-include Stm32_F103C6_Drivers/HAL/LCD_Driver/subdir.mk
-include Stm32_F103C6_Drivers/HAL/KPD_Driver/subdir.mk
-include Stm32_F103C6_Drivers/HAL/EEPROM/subdir.mk
-include Stm32_F103C6_Drivers/HAL/Delay/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include FreeRTOS/portable/MemMang/subdir.mk
-include FreeRTOS/portable/ARM_CM3/subdir.mk
-include FreeRTOS/subdir.mk
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := GP_STM32_Drivers
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
GP_STM32_Drivers.axf \

MAP_FILES += \
GP_STM32_Drivers.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
GP_STM32_Drivers.list \

OBJCOPY_HEX += \
GP_STM32_Drivers.hex \

OBJCOPY_BIN += \
GP_STM32_Drivers.bin \


# All Target
all: main-build

# Main-build Target
main-build: GP_STM32_Drivers.axf secondary-outputs

# Tool invocations
GP_STM32_Drivers.axf GP_STM32_Drivers.map: $(OBJS) $(USER_OBJS) C:\Users\ahmed\STM32CubeIDE\workspace_1.4.0\GP_STM32_Drivers\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -gdwarf-2 -o "GP_STM32_Drivers.axf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\ahmed\STM32CubeIDE\workspace_1.4.0\GP_STM32_Drivers\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="GP_STM32_Drivers.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

GP_STM32_Drivers.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "GP_STM32_Drivers.list"
	@echo 'Finished building: $@'
	@echo ' '

GP_STM32_Drivers.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "GP_STM32_Drivers.hex"
	@echo 'Finished building: $@'
	@echo ' '

GP_STM32_Drivers.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "GP_STM32_Drivers.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) GP_STM32_Drivers.axf GP_STM32_Drivers.bin GP_STM32_Drivers.hex GP_STM32_Drivers.list GP_STM32_Drivers.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
