/**
 ****
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ****
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ****
 */

#if !defined(_SOFT_FP) && defined(_ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "Stm32_F103C6_GPIO_Driver.h"
#include "Stm32_F103C6_EXTI_Driver.h"
#include "stm32f103x6.h"
#include "core_cm3.h"
#include "Scheduler.h"
#include "Stm32_F103C8_TIM_Driver.h"
#include "Stm32f103c6_ADC.h"
#include "Stm32_F103C8_WWDG_Driver.h"
#include <Pedal.h>



void clock_init(void)
{

	//Enable clock to AFIO
	AFIO_GPIO_CLK_EN();
	//Enable clock to port A
	RCC_GPIOA_CLK_EN();
	//Enable clock to port B
	RCC_GPIOB_CLK_EN();



}


int main(void)
{

GPIO_PinConfig_t PinConfig;
PinConfig.GPIO_PinNumber = GPIO_PIN_2;
PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_PP ;
PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M ;

MCAL_GPIO_Init(GPIOA, &PinConfig);

	clock_init();
	HAL_Delay_Init();


	while (1)
	{



        Delay_us(1);

        GPIOA->ODR ^= 1<<2 ;


	}

}
