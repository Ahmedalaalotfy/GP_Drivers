/*
 * Delay.h
 *
 *  Created on: Nov 28, 2023
 *      Author: ahmed
 */

#ifndef HAL_DELAY_DELAY_H_
#define HAL_DELAY_DELAY_H_

#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO_Driver.h"
#include "Stm32_F103C8_TIM_Driver.h"

//Specifies Timer instance to be used in Delay driver
#define DELAY_Timer TIM2


void HAL_Delay_Init(void);

void Delay_us(uint16_t Num);
void Delay_ms(uint16_t Num);
void Delay_s(uint16_t Num);

#endif /* HAL_DELAY_DELAY_H_ */
