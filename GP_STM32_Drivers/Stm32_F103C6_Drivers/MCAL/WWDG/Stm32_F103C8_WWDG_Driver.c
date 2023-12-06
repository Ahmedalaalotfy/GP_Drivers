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

	SET_BIT(WWDG->CR,7);

	WWDG->CR |=  WWDG_Config->Counter_Start_VAL ;

	WWDG->CFR = 0 ;
	WWDG->CFR |=  WWDG_Config->Window_Value ;


	WWDG->CFR |= WWDG_Config->Prescaler ; //Prescaler



	G_Config = *WWDG_Config ;



	if (WWDG_Config->EWI_IRQ_Enable == WWDG_EWI_IRQ_Enable)
	{
		NVIC_IRQ0_WWDG_Enable();
		SET_BIT(WWDG->CFR,9);
	}
	else if (WWDG_Config->EWI_IRQ_Enable == WWDG_EWI_IRQ_None)
	{
		NVIC_IRQ0_WWDG_Disable();
	}





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








