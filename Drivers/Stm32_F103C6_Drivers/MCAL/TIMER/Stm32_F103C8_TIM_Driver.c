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


	//Clock & Configurations
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



	//Mode Selection

	switch(TIM_Config->TIM_Mode)
	{

	case TIM_Mode_Counter:

		//TIM_Auto_Reload_Not_Bufferd
		//	Bit 7 ARPE: Auto-reload preload enable
		//	0: TIMx_ARR register is not buffered
		//	1: TIMx_ARR register is buffered
		CLR_BIT(TIMx->CR1,7);

		if(TIM_Config->Counter.Count_Direction == Count_Direction_UP)
		{
			/*
Bit 2 URS: Update request source
This bit is set and cleared by software to select the UEV event sources.
0: Any of the following events generate an update interrupt or DMA request if enabled.
These events can be:
– Counter overflow/underflow
– Setting the UG bit
– Update generation through the slave mode controller
1: Only counter overflow/underflow generates an update interrupt or DMA request if
enabled.
			 */

			SET_BIT(TIMx->CR1,2) ;

			//Bit 4 DIR: Direction Selection [Upcount]
			CLR_BIT(TIMx->CR1,4) ;


		}
		else if(TIM_Config->Counter.Count_Direction == Count_Direction_DOWN)
		{
			//Bit 2 URS: Update request source
			SET_BIT(TIMx->CR1,2) ;

			//Bit 4 DIR: Direction [DOWN_Count]
			SET_BIT(TIMx->CR1,4) ;

		}
		else if(TIM_Config->Counter.Count_Direction == Count_Direction_UP_DOWN)
		{
			//Bit 2 URS: Update request source
			SET_BIT(TIMx->CR1,2) ;
		}

		break;




	case TIM_Mode_PWM:

		//TIM_Auto_Reload_Bufferd
		//	Bit 7 ARPE: Auto-reload preload enable
		//	0: TIMx_ARR register is not buffered
		//	1: TIMx_ARR register is buffered
		SET_BIT(TIMx->CR1,7);

		/*
		 Bit 0 UG: Update generation
This bit can be set by software, it is automatically cleared by hardware.
0: No action
1: Re-initialize the counter and generates an update of the registers. Note that the prescaler
counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if
the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload
value (TIMx_ARR) if DIR=1 (downcounting).
		 */
		SET_BIT(TIMx->EGR,0);


		/******************** Channel Configuration ********************/

		if(TIM_Config->PWM.Channel == TIM_CHANNEL_1 ){


			/*
			 Bits 1:0 CC1S: Capture/Compare 1 selection
			This bit-field defines the direction of the channel (input/output) as well as the used input.
			00: CC1 channel is configured as output.
			 */
			TIMx->CCMR1 |= (0b00 << 0);




			/****** Mode Configuration ********/
			if(TIM_Config->PWM.Mode == PWM_Mode_FROZEN){


				/*
				Bits 6:4 OC1M: Output compare 1 mode

These bits define the behavior of the output reference signal OC1REF from which OC1 and
OC1N are derived. OC1REF is active high whereas OC1 and OC1N active level depends on CC1P and CC1NP bits.

	000: Frozen - The comparison between the output compare register TIMx_CCR1 and the
	counter TIMx_CNT has no effect on the outputs.(this mode is used to generate a timing base).
	001: Set channel 1 to active level on match. OC1REF signal is forced high when the counter
	TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
	010: Set channel 1 to inactive level on match. OC1REF signal is forced low when the
	counter TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
	011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.
	100: Force inactive level - OC1REF is forced low.
	101: Force active level - OC1REF is forced high.
	110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
	else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
	TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
	111: PWM mode 2 - In upcounting, channel 1 is inactive as long as TIMx_CNT<TIMx_CCR1
	else active. In downcounting, channel 1 is active as long as TIMx_CNT>TIMx_CCR1 else inactive.

Note: In PWM mode 1 or 2, the OCREF level changes only when the result of the
comparison changes or when the output compare mode switches from “frozen” mode to “PWM” mode.

				 */
				//000: Frozen
				TIMx->CCMR1 |= (0b000 << 4);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_UP){

				/*
	Bit 2 URS: Update request source
	This bit is set and cleared by software to select the UEV event sources.
	0: Any of the following events generate an update interrupt or DMA request if enabled.
	These events can be:
	– Counter overflow/underflow
	– Setting the UG bit
	– Update generation through the slave mode controller
	1: Only counter overflow/underflow generates an update interrupt or DMA request if
	enabled.
				 */
				// Update request source Enable
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction Selection [Upcount]
				CLR_BIT(TIMx->CR1,4) ;

				//****************************************************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//111: PWM mode 2
					TIMx->CCMR1 |= (0b111 << 4);

				}else{
					//110: PWM mode 1
					TIMx->CCMR1 |= (0b110 << 4);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,1);


			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_DOWN){

				//Bit 2 URS: Update request source
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction [DOWN_Count]
				SET_BIT(TIMx->CR1,4) ;

				//****************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//110: PWM mode 1
					TIMx->CCMR1 |= (0b110 << 4);

				}else{
					//111: PWM mode 2
					TIMx->CCMR1 |= (0b111 << 4);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,1);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_Centre_Aligned){

				//todo PWM_Mode_Centre_Aligned

			}



			/*
 	 Bits 15:0 CCR1[15:0]: Capture/Compare 1 value
If channel CC1 is configured as output:
CCR1 is the value to be loaded in the actual capture/compare 1 register (preload value).
It is loaded permanently if the preload feature is not selected in the TIMx_CCMR1 register
(bit OC1PE). Else the preload value is copied in the active capture/compare 1 register when an update event occurs.
The active capture/compare register contains the value to be compared to the counter
TIMx_CNT and signaled on OC1 output.
			 */
			if(TIM_Config->PWM.Compare_value <= TIM_Config->Auto_Reload_Value){

				TIMx->CCR1 = TIM_Config->PWM.Compare_value ;

			}
			else
			{
				return TIM_PWM_Auto_Reload_Value_Exceeded;
			}

			/*
			 Bit 3 OC1PE: Output compare 1 preload enable

0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written at anytime, the
new value is taken in account immediately.
1: Preload register on TIMx_CCR1 enabled. Read/Write operations access the preload
register. TIMx_CCR1 preload value is loaded in the active register at each update event.

Note:
1: These bits can not be modified as long as LOCK level 3 has been programmed
(LOCK bits in TIMx_BDTR register) and CC1S=00 (the channel is configured in output).
2: The PWM mode can be used without validating the preload register only
in one pulse mode (OPM bit set in TIMx_CR1 register). Else the behavior is not guaranteed.
			 */
			SET_BIT(TIMx->CCMR1,3);



			/*
			Bit 0 CC1E: Capture/Compare 1 output enable

CC1 channel configured as output:
0: Off - OC1 is not active.
1: On - OC1 signal is output on the corresponding output pin.
			 */

			//Capture/Compare output enable
			SET_BIT(TIMx->CCER,0);

		}else if(TIM_Config->PWM.Channel == TIM_CHANNEL_2){


			//channel is configured as output.
			TIMx->CCMR1 |= (0x00 << 8);


			/****** Mode Configuration ********/

			if(TIM_Config->PWM.Mode == PWM_Mode_FROZEN){

				//000: Frozen
				TIMx->CCMR1 |= (0x000 << 12);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_UP){

				// Update request source Enable
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction Selection [Upcount]
				CLR_BIT(TIMx->CR1,4) ;

				//****************************************************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//111: PWM mode 2
					TIMx->CCMR1 |= (0b111 << 12);

				}else{
					//110: PWM mode 1
					TIMx->CCMR1 |= (0b110 << 12);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,5);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_DOWN){

				//Bit 2 URS: Update request source
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction [DOWN_Count]
				SET_BIT(TIMx->CR1,4) ;

				//****************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//110: PWM mode 1
					TIMx->CCMR1 |= (0b110 << 12);

				}else{
					//111: PWM mode 2
					TIMx->CCMR1 |= (0b111 << 12);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,5);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_Centre_Aligned){

			}

			//Set Capture/Compare value
			if(TIM_Config->PWM.Compare_value <= TIM_Config->Auto_Reload_Value){

				TIMx->CCR2 = TIM_Config->PWM.Compare_value ;

			}
			else
			{
				return TIM_PWM_Auto_Reload_Value_Exceeded;
			}

			// Output compare preload enable
			SET_BIT(TIMx->CCMR1,11);

			//Capture/Compare output enable
			SET_BIT(TIMx->CCER,4);

		}else if(TIM_Config->PWM.Channel == TIM_CHANNEL_3){

			//channel is configured as output.
			TIMx->CCMR2 |= (0x00 << 0);

			/****** Mode Configuration ********/

			if(TIM_Config->PWM.Mode == PWM_Mode_FROZEN){

				//000: Frozen
				TIMx->CCMR2 |= (0x000 << 4);


			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_UP){

				// Update request source Enable
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction Selection [Upcount]
				CLR_BIT(TIMx->CR1,4) ;

				//****************************************************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//111: PWM mode 2
					TIMx->CCMR2 |= (0b111 << 4);

				}else{
					//110: PWM mode 1
					TIMx->CCMR2 |= (0b110 << 4);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,9);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_DOWN){

				//Bit 2 URS: Update request source
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction [DOWN_Count]
				SET_BIT(TIMx->CR1,4) ;

				//****************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//110: PWM mode 1
					TIMx->CCMR2 |= (0b110 << 4);

				}else{
					//111: PWM mode 2
					TIMx->CCMR2 |= (0b111 << 4);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,9);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_Centre_Aligned){

			}

			//Set Capture/Compare value
			if(TIM_Config->PWM.Compare_value <= TIM_Config->Auto_Reload_Value){

				TIMx->CCR3 = TIM_Config->PWM.Compare_value ;

			}
			else
			{
				return TIM_PWM_Auto_Reload_Value_Exceeded;
			}


			// Output compare preload enable
			SET_BIT(TIMx->CCMR2,3);

			//Capture/Compare output enable
			SET_BIT(TIMx->CCER,8);

		}else if(TIM_Config->PWM.Channel == TIM_CHANNEL_4){

			//channel is configured as output.
			TIMx->CCMR2 |= (0x00 << 8);

			/****** Mode Configuration ********/

			if(TIM_Config->PWM.Mode == PWM_Mode_FROZEN){

				//000: Frozen
				TIMx->CCMR2 |= (0x000 << 12);


			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_UP){

				// Update request source Enable
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction Selection [Upcount]
				CLR_BIT(TIMx->CR1,4) ;

				//****************************************************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//111: PWM mode 2
					TIMx->CCMR2 |= (0b111 << 12);

				}else{
					//110: PWM mode 1
					TIMx->CCMR2 |= (0b110 << 12);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,13);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_EDGE_DOWN){

				//Bit 2 URS: Update request source
				SET_BIT(TIMx->CR1,2) ;

				//Bit 4 DIR: Direction [DOWN_Count]
				SET_BIT(TIMx->CR1,4) ;

				//****************************************

				if(TIM_Config->PWM.Ouptut_On_Compare_Match == PWM_Compare_Match_High){

					//110: PWM mode 1
					TIMx->CCMR2 |= (0b110 << 12);

				}else{
					//111: PWM mode 2
					TIMx->CCMR2 |= (0b111 << 12);
				}

				//0: CCxP: Capture/Compare output polarity OCx active high
				CLR_BIT(TIMx->CCER,13);

			}else if(TIM_Config->PWM.Mode == PWM_Mode_Centre_Aligned){

			}

			//Set Capture/Compare value
			if(TIM_Config->PWM.Compare_value <= TIM_Config->Auto_Reload_Value){

				TIMx->CCR4 = TIM_Config->PWM.Compare_value ;

			}
			else
			{
				return TIM_PWM_Auto_Reload_Value_Exceeded;
			}


			// Output compare preload enable
			SET_BIT(TIMx->CCMR2,11);

			//Capture/Compare output enable
			SET_BIT(TIMx->CCER,12);

		}




		break;


	default:
		CLR_BIT(TIMx->CR1,2) ;
		return TIM_MODE_NOT_Found ;
		break;

	}






	SET_BIT(TIMx->CR1,1) ; //Update enable Bit 1 UDIS: Update disable

	TIMx->PSC = (uint16_t)(TIM_Config->Prescaler-1) ; // Prescaler set


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



return 0;

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
Error_status MCAL_TIM_GPIO_Set_Pins(TIM_TypeDef *TIMx,uint8_t TIM_Channel,uint8_t TIM_Mode)
{

	GPIO_PinConfig_t PinConfig;



	if(TIMx == TIM2){

		switch(TIM_Mode)
		{
		case TIM_Mode_PWM:

			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;

			if(TIM_Channel == TIM_CHANNEL_1){
				PinConfig.GPIO_PinNumber = GPIO_PIN_0;
			}else if(TIM_Channel == TIM_CHANNEL_2){
				PinConfig.GPIO_PinNumber = GPIO_PIN_1;
			}else if(TIM_Channel == TIM_CHANNEL_3){
				PinConfig.GPIO_PinNumber = GPIO_PIN_2;
			}else if(TIM_Channel == TIM_CHANNEL_4){
				PinConfig.GPIO_PinNumber = GPIO_PIN_3;
			}

			MCAL_GPIO_Init(GPIOA, &PinConfig);
			break;

		case TIM_Mode_Input_Capture:
			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;

			if(TIM_Channel == TIM_CHANNEL_1){
				PinConfig.GPIO_PinNumber = GPIO_PIN_0;
			}else if(TIM_Channel == TIM_CHANNEL_2){
				PinConfig.GPIO_PinNumber = GPIO_PIN_1;
			}else if(TIM_Channel == TIM_CHANNEL_3){
				PinConfig.GPIO_PinNumber = GPIO_PIN_2;
			}else if(TIM_Channel == TIM_CHANNEL_4){
				PinConfig.GPIO_PinNumber = GPIO_PIN_3;
			}

			MCAL_GPIO_Init(GPIOA, &PinConfig);
			break;

		default:
			return TIM_MODE_NOT_Found;
			break;
		}

	}
	else if(TIMx == TIM3)
	{
		switch(TIM_Mode)
		{
		case TIM_Mode_PWM:

			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;

			if(TIM_Channel == TIM_CHANNEL_1){
				PinConfig.GPIO_PinNumber = GPIO_PIN_6;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
			}else if(TIM_Channel == TIM_CHANNEL_2){
				PinConfig.GPIO_PinNumber = GPIO_PIN_7;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
			}else if(TIM_Channel == TIM_CHANNEL_3){
				PinConfig.GPIO_PinNumber = GPIO_PIN_0;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
			}else if(TIM_Channel == TIM_CHANNEL_4){
				PinConfig.GPIO_PinNumber = GPIO_PIN_1;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
			}


			break;

		case TIM_Mode_Input_Capture:

			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;

			if(TIM_Channel == TIM_CHANNEL_1){
				PinConfig.GPIO_PinNumber = GPIO_PIN_6;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
			}else if(TIM_Channel == TIM_CHANNEL_2){
				PinConfig.GPIO_PinNumber = GPIO_PIN_7;
				MCAL_GPIO_Init(GPIOA, &PinConfig);
			}else if(TIM_Channel == TIM_CHANNEL_3){
				PinConfig.GPIO_PinNumber = GPIO_PIN_0;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
			}else if(TIM_Channel == TIM_CHANNEL_4){
				PinConfig.GPIO_PinNumber = GPIO_PIN_1;
				MCAL_GPIO_Init(GPIOB, &PinConfig);
			}

			MCAL_GPIO_Init(GPIOB, &PinConfig);
			break;

		default:
			return TIM_MODE_NOT_Found;
			break;
		}
	}
	else if(TIMx == TIM4)
	{
		switch(TIM_Mode)
		{
		case TIM_Mode_PWM:

			PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M;

			if(TIM_Channel == TIM_CHANNEL_1){
				PinConfig.GPIO_PinNumber = GPIO_PIN_6;
			}else if(TIM_Channel == TIM_CHANNEL_2){
				PinConfig.GPIO_PinNumber = GPIO_PIN_7;
			}else if(TIM_Channel == TIM_CHANNEL_3){
				PinConfig.GPIO_PinNumber = GPIO_PIN_8;
			}else if(TIM_Channel == TIM_CHANNEL_4){
				PinConfig.GPIO_PinNumber = GPIO_PIN_9;
			}

			MCAL_GPIO_Init(GPIOB, &PinConfig);
			break;

		case TIM_Mode_Input_Capture:

			PinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;

			if(TIM_Channel == TIM_CHANNEL_1){
				PinConfig.GPIO_PinNumber = GPIO_PIN_6;
			}else if(TIM_Channel == TIM_CHANNEL_2){
				PinConfig.GPIO_PinNumber = GPIO_PIN_7;
			}else if(TIM_Channel == TIM_CHANNEL_3){
				PinConfig.GPIO_PinNumber = GPIO_PIN_8;
			}else if(TIM_Channel == TIM_CHANNEL_4){
				PinConfig.GPIO_PinNumber = GPIO_PIN_9;
			}

			MCAL_GPIO_Init(GPIOB, &PinConfig);
			break;

		default:
			return TIM_MODE_NOT_Found;
			break;
		}
	}
	else
	{
		return TIMx_NOT_Found;
	}

	return 0;

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

	return 0;

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


	return 0;

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



