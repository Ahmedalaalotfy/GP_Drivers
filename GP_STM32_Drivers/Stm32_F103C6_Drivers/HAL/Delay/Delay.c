/*
 * Delay.c
 *
 *  Created on: Nov 28, 2023
 *      Author: ahmed
 */


#include "Delay.h"


TIM_Config_t Delay_timer_config ;

/**================================================================
 * @Fn			 -HAL_Delay_Init
 * @brief 		 -Choose the Timer instance to use for delay functions
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @retval		 -none
 * Note			 -none
 */
void HAL_Delay_Init(void)
{
	Delay_timer_config.TIM_Mode =  TIM_Mode_Counter;
	Delay_timer_config.Counter.Count_Direction =Count_Direction_UP;
	Delay_timer_config.Prescaler = 8 ;
	Delay_timer_config.Auto_Reload_Value = 0xFFFF ;
	Delay_timer_config.IRQ_Enable = TIM_IRQ_MODE_None  ;
	Delay_timer_config.P_IRQ_CallBack = NULL ;

	MCAL_TIM_Init(DELAY_Timer,&Delay_timer_config);
	MCAL_TIM_Count_Reset(DELAY_Timer);
}




void Delay_us(uint16_t Num)
{

	DELAY_Timer->ARR = Num		;
	DELAY_Timer->CNT = 0x0000	;
	SET_BIT(DELAY_Timer->CR1,0) ; //Timer Enable

	//poll for timer Update interrupt flag (Counting Done)
	/*
Bit 0 UIF: Update interrupt flag
– This bit is set by hardware on an update event. It is cleared by software.
0: No update occurred.
1: Update interrupt pending. This bit is set by hardware when the registers are updated:
– At overflow or underflow and if the UDIS=0 in the TIMx_CR1 register.
– When CNT is reinitialized by software using the UG bit in TIMx_EGR register, if URS=0 and
UDIS=0 in the TIMx_CR1 register.
– When CNT is reinitialized by a trigger event (refer to the synchro control register description),
if URS=0 and UDIS=0 in the TIMx_CR1 register.
	*/

	while(GET_BIT(DELAY_Timer->SR,0) == 0);
	CLR_BIT(DELAY_Timer->CR1,0) ;
	CLR_BIT(DELAY_Timer->SR,0);


}


void Delay_ms(uint16_t Num)
{
	int i;

	for(i =0 ;i< Num;i++)
	{
		Delay_us(1000);
	}

}

void Delay_s(uint16_t Num)
{
	int i;

	for(i =0 ;i< Num;i++)
	{
		Delay_ms(1000);
	}

}


