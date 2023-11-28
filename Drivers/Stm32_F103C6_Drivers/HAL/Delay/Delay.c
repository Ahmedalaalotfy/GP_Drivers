/*
 * Delay.c
 *
 *  Created on: Nov 28, 2023
 *      Author: ahmed
 */


#include "Delay.h"
#include "Stm32_F103C8_TIM_Driver.h"

volatile int flag_us ,flag_ms  ;

void timer1_us ()
{
  flag_us++ ;
}

void timer1_ms ()
{
  flag_ms ++ ;
}

void Delay_us(uint16_t Num)
{
	flag_us = 0 ;
	TIM_Config_t timer_config ;
	GPIO_PinConfig_t PinConfig ;

	timer_config.TIM_Mode = TIM_Mode_UP_Count ;
	timer_config.Prescaler = 8 ;
	timer_config.Auto_Reload_Value = Num ;
	timer_config.Auto_Reload_status = TIM_Auto_Reload_Not_Bufferd ;
	timer_config.IRQ_Enable = TIM_IRQ_MODE_OverFlow  ;
	timer_config.P_IRQ_CallBack = timer1_us ;

	MCAL_TIM_Init(TIM2,&timer_config );

	while(flag_us==0);
}
void Delay_ms(uint16_t Num)
{
	flag_ms = 0 ;
	TIM_Config_t timer_config ;
	GPIO_PinConfig_t PinConfig ;

	timer_config.TIM_Mode = TIM_Mode_UP_Count ;
	timer_config.Prescaler = 8 ;
	timer_config.Auto_Reload_Value = Num ;
	timer_config.Auto_Reload_status = TIM_Auto_Reload_Not_Bufferd ;
	timer_config.IRQ_Enable = TIM_IRQ_MODE_OverFlow  ;
	timer_config.P_IRQ_CallBack = timer1_ms ;

	MCAL_TIM_Init(TIM2,&timer_config );

	while(flag_ms==0);

}


