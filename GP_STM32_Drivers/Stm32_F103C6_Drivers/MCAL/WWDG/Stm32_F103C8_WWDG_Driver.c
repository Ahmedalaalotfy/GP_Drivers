/*
 * Stm32_F103C6_WWDG_Driver.c
 *
 *  Created on: Dec 6, 2023
 *      Author: Ahmedalaalotfy , Mohamed Nabil
 */


#include <Stm32_F103C8_WWDG_Driver.h>




/*
 * =======================================================================================
 * 							Generic Variables
 * =======================================================================================
 */

WWDG_Config_t G_Config  ;

/*
 * =======================================================================================
 * 							Generic Macros
 * =======================================================================================
 */







/*
 * =======================================================================================
 * 							Generic Functions
 * =======================================================================================
 */

WWDG_Error_status MCAL_WWDG_Init(WWDG_Config_t *WWDG_Config)
{

	G_Config = *WWDG_Config ;


	uint16_t tempCR = 0,
			tempCFR = 0;


	//Enable Clock To WWDG
	RCC_WWDG_CLK_EN();

/*
Bit 9 DBG_WWDG_STOP: Debug window watchdog stopped when core is halted
0: The window watchdog counter clock continues even if the core is halted
1: The window watchdog counter clock is stopped when the core is halted
*/

	//DBGMCU_CR |= (uint32_t)(1<<9);


	tempCR |=  WWDG_Config->Counter_Start_VAL ;

	tempCFR |=  WWDG_Config->Window_Value ;

	tempCFR |= WWDG_Config->Prescaler ; //Prescaler




	if (WWDG_Config->EWI_IRQ_Enable == WWDG_EWI_IRQ_Enable)
	{
		NVIC_IRQ0_WWDG_Enable();
		SET_BIT(tempCFR,9);
	}
	else if (WWDG_Config->EWI_IRQ_Enable == WWDG_EWI_IRQ_None)
	{
		NVIC_IRQ0_WWDG_Disable();
	}


	//Set actual registers values

	//WWDG->CR |=1<<7;

	//Important condition
	//wait until T6:0 < W6:0
	while((WWDG->CFR & 0x7F) < (WWDG->CR & 0x7F));

	WWDG->CR = tempCR;

	WWDG->CFR = tempCFR;

	WWDG->SR = 0;


	return WWDG_OK ;
}


WWDG_Error_status MCAL_WWDG_Start(void)
{

	//	Bit 7 WDGA: Activation bit
	//	This bit is set by software and only cleared by hardware after a reset. When WDGA = 1, the
	//	watchdog can generate a reset.
	//	0: Watchdog disabled
	//	1: Watchdog enabled

	SET_BIT(WWDG->CR,7);


	return WWDG_OK ;
}


WWDG_Error_status MCAL_WWDG_Kick(void)
{
	WWDG->CR |=  G_Config.Counter_Start_VAL ;

	return WWDG_OK ;
}



void WWDG_IRQHandler ()
{
	CLR_BIT(WWDG->SR,0);
	G_Config.P_IRQ_CallBack();

}








