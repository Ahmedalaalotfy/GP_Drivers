/*
 * Stm32_F103C6_TIM_Driver.c
 *
 *  Created on: Nov 27, 2023
 *      Author: Ahmedalaalotfy,Mohamed Nabil
 */


#include <Stm32_F103C8_TIM_Driver.h>



/*
 * =====================================================================================
 * 							Generic Variables
 * =====================================================================================
 */

TIM_Config_t Global_TIM_Config[3] ;

/*
 * =====================================================================================
 * 							Generic Macros
 * =====================================================================================
 */



/*
 * =====================================================================================
 * 							Generic Functions
 * =====================================================================================
 */




/*
 * =====================================================================================
 * 							APIS Supported by "MCAL TIM driver"
 * =====================================================================================
 */

/**================================================================
 * @Fn			 -MCAL_TIM_Init
 * @brief 		 -Initialize TIM according to the specified parameters in TIM_Config
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @param [in]   -PinConfig : pointer to TIM_Config Structure that Contains
 * 						      the configuration information for All TIMER.
 * @retval		 -none
 * Note			 -none
 */

Error_status MCAL_TIM_Init(TIM_TypeDef *TIMx,TIM_Config_t *TIM_Config)
{


	if (TIMx == TIM2)
	{
		RCC_TIM2_CLK_EN();
		Global_TIM_Config[0] = *TIM_Config ;
		NVIC_IQR28_TIM2_Enable();
	}
	else if (TIMx == TIM3)
	{
		RCC_TIM3_CLK_EN();
		Global_TIM_Config[1] = *TIM_Config ;
		NVIC_IQR29_TIM3_Enable();

	}
	else if (TIMx == TIM4)
	{
		RCC_TIM4_CLK_EN();
		Global_TIM_Config[2] = *TIM_Config ;
		NVIC_IQR30_TIM4_Enable();
	}
	else
	{
		return TIMx_NOT_Found ;
	}

	if (TIM_Config->TIM_Mode == TIM_Mode_UP_Count)
	{
		CLR_BIT(TIMx->CR1,4) ;
	}
	else if (TIM_Config->TIM_Mode == TIM_Mode_DOWN_Count)
	{
		SET_BIT(TIMx->CR1,4) ;
	}
	else if (TIM_Config->TIM_Mode == TIM_Mode_UP_DOWN_Count)
	{

	}
	else
	{
		return TIM_MODE_NOT_Found ;
	}

	CLR_BIT(TIMx->CR1,1) ; //Update disable

	TIMx->PSC = (uint16_t)(TIM_Config->Prescaler-1) ; // Prescaler set
	TIMx->CR1 |= TIM_Config->Auto_Reload_status ;  //auto reload status


	if (TIM_Config->Auto_Reload_Value < 0xffff)
	{
		TIMx->ARR = (uint16_t) (TIM_Config->Auto_Reload_Value); //auto reload Value
	}
	else
	{
		return TIM_Auto_Reload_Value_Exceeded ;

	}

	if (TIM_Config->IRQ_Enable == TIM_IRQ_MODE_None )
	{
		CLR_BIT(TIMx->DIER,0);
		SET_BIT(TIMx->CR1,1);
	}
	else if (TIM_Config->IRQ_Enable == TIM_IRQ_MODE_OverFlow )
	{
		SET_BIT(TIMx->DIER,0);
		CLR_BIT(TIMx->CR1,1);
	}
	else if (TIM_Config->IRQ_Enable == TIM_IRQ_MODE_UnderFlow )
	{
		SET_BIT(TIMx->DIER,0);
		CLR_BIT(TIMx->CR1,1);
	}

	MCAL_TIM_Count_Reset(TIMx);

}

/**================================================================
 * @Fn			 -MCAL_TIM_DeInit
 * @brief 		 -Reset Timer Registers and NVIC corresponding to IRQ Mask
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @retval		 -none
 * Note			 -none
 */
void MCAL_TIM_DeInit(TIM_TypeDef *TIMx)
{
	if (TIMx == TIM2)
	{
		RCC_TIM2_CLK_Reset();
		NVIC_IQR28_TIM2_Disable();
	}
	else if (TIMx == TIM3)
	{
		RCC_TIM3_CLK_Reset();
		NVIC_IQR29_TIM3_Disable();
	}
	else if (TIMx == TIM4)
	{
		RCC_TIM4_CLK_Reset();
		NVIC_IQR30_TIM4_Disable();
	}
}


/**================================================================
 * @Fn			 -MCAL_TIM_GPIO_Set_Pins
 * @brief 		 -Reset Timer Registers and NVIC corresponding to IRQ Mask
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @retval		 -none
 * Note			 -none
 */
void MCAL_TIM_GPIO_Set_Pins(TIM_TypeDef *TIMx)
{

}




/**================================================================
 * @Fn			 -MCAL_TIM_Start
 * @brief 		 -Start Timer counting
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @retval		 -none
 * Note			 -none
 */
Error_status MCAL_TIM_Start(TIM_TypeDef *TIMx)
{

/*
Bit 0 CEN: Counter enable
0: Counter disabled
1: Counter enabled
Note: External clock, gated mode and encoder mode can work only if the CEN bit has been
previously set by software. However trigger mode can set the CEN bit automatically by
hardware.
CEN is cleared automatically in one-pulse mode, when an update event occurs.
*/

	if(TIMx != NULL)
	{
		SET_BIT(TIMx->CR1,0) ; //Timer Enable
	}
	else
	{
		return TIMx_NOT_Found;
	}


}


/**================================================================
 * @Fn			 -MCAL_TIM_Stop
 * @brief 		 -Stop Timer counting
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @retval		 -none
 * Note			 -none
 */
Error_status MCAL_TIM_Stop(TIM_TypeDef *TIMx)
{
	/*
	Bit 0 CEN: Counter enable
	0: Counter disabled
	1: Counter enabled
	Note: External clock, gated mode and encoder mode can work only if the CEN bit has been
	previously set by software. However trigger mode can set the CEN bit automatically by
	hardware.
	CEN is cleared automatically in one-pulse mode, when an update event occurs.
	*/

		if(TIMx != NULL)
		{
			CLR_BIT(TIMx->CR1,0) ; //Timer Enable
		}
		else
		{
			return TIMx_NOT_Found;
		}



}

/**================================================================
 * @Fn			 -MCAL_TIM_Count_Reset
 * @brief 		 -Reset Timer counter register to zero
 * @param [in]   -TIMx :  were x can be (1,2,3,4 depending on device used)to select the Timer Peripheral
 * @retval		 -none
 * Note			 -none
 */
Error_status MCAL_TIM_Count_Reset(TIM_TypeDef *TIMx)
{


	if(TIMx != NULL)
	{
		TIMx->CNT = 0x0000;
		return TIMx_No_Error;
	}


	return TIMx_NOT_Found;

}


/*
 * =====================================================================================
 * 							IRQHandler Functions
 * =====================================================================================
 */

void TIM2_IRQHandler()
{
	TIM2->SR = 0x00 ;
	Global_TIM_Config[0].P_IRQ_CallBack();


}
void TIM3_IRQHandler()
{
	TIM3->SR = 0x00 ;
	Global_TIM_Config[1].P_IRQ_CallBack();
}
void TIM4_IRQHandler()
{
	TIM4->SR = 0x00 ;
	Global_TIM_Config[2].P_IRQ_CallBack();
}



