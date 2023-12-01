/*
 * STM32f103c6_ADC.c
 *
 *  Created on: Nov 27, 2023
 *      Author: Acer
 */


#include "Stm32f103c6_ADC.h"

/*
 * ===============================================
 * Generic Macros
 * ===============================================
 */


#define Out_of_range 10

/*
 * ===============================================
 * Generic Variables
 * ===============================================
 */

ADC_Config_t Global_ADC_Cfg;

uint16_t idle = Out_of_range;

uint16_t Rank = 1;

uint16_t Data_for_IRQ = 0 ;


/*
 * ===============================================
 * Generic Functions decelerations
 * ===============================================
 */
uint16_t Get_the_channel_pin(uint16_t channel_num);


/*
 * ===============================================
 * 						APIs
 * ===============================================
 */


/**================================================================
 * @Fn            - MCAL_ADC_init
 * @brief         - Initialize ADC
 * @param [in]    - ADCx: where x can be 1 - 2 (depending on device used) to select the ADC peripheral
 * @param [in]    - ADC_Config: all ADC configuration
 * @retval        - none
 * Note           - none
 */
void MCAL_ADC_init(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config)
{
	uint32_t i=0;

	//copy the content to ADC_Cfg structure
	Global_ADC_Cfg = *ADC_Config;

	//clock ENABLE
	if(ADCx == ADC1)
	{
		RCC_ADC1_CLK_EN;
	}
	else
	{
		RCC_ADC2_CLK_EN;
	}

	//Clear all control registers
	ADCx->CR1 = 0;
	ADCx->CR2 = 0;

	//interrupt enable
	if(ADC_Config->Interrupt == ADC_Interrupt_enable)
	{
		NVIC_IRQ18_ADC_EN;
		ADCx->CR1 |= (ADC_Config->Interrupt);
	}

	//channel for conversion
	ADCx->SQR3 |= (ADC_Config->channels[Rank0].Channel_num)-1 ;

	//Continuous mode
	ADCx->CR2 |= (ADC_Config->Continous_Mode);

	//Alignment mode
	ADCx->CR2 |= (ADC_Config->Data_Alignment);

	//ADON bit
	ADCx->CR2 |= (1<<0);

	//Delay for Stabilization
	for(i=0 ; i<65 ; i++);

	//clear START bit
	ADCx->SR &= ~(1<<4);

}


/**================================================================
 * @Fn            - MCAL_ADC_Deinit
 * @brief         - DeInitialize ADC
 * @param [in]    - ADCx: where x can be 1 - 2 (depending on device used) to select the ADC peripheral
 * @param [in]    - Idle_mode: where used to perform the selected idle mode from the user to power off / DeInit the ADC
 * @retval        - none
 * Note           - none
 */
void MCAL_ADC_Deinit(ADC_TypeDef* ADCx , ADC_Idle Idle_mode)
{
	//To store the idle state for ReInit
	idle = Idle_mode;
	if(ADCx == ADC1)
	{
		if(Idle_mode == ADC_full_close)
		{
			//Disable ADON bit and reset control registers
			ADCx->CR1 = 0;
			ADCx->CR2 = 0;

			//Clock disable
			RCC_ADC1_CLK_DIS;

			idle = Out_of_range ;
		}
		else if(Idle_mode == ADC_Less_power)
		{
			//disable ADON bit
			ADCx->CR2 &= ~(1<<0);
		}
	}

	if(ADCx == ADC2)
	{
		if(Idle_mode == ADC_full_close)
		{
			//Disable ADON bit and reset control registers
			ADCx->CR1 = 0;
			ADCx->CR2 = 0;

			//Clock disable
			RCC_ADC2_CLK_DIS;

			//NVIC disable
			NVIC_IRQ18_ADC_DIS;
		}
		else if(Idle_mode == ADC_Less_power)
		{
			//disable ADON bit
			ADCx->CR2 &= ~(1<<0);
		}
	}
}


/**================================================================
 * @Fn            - MCAL_ADC_pins_set
 * @brief         - Set pins to analog mode
 * @param [in]    - ADCx: where x can be 1 - 2 (depending on device used) to select the ADC peripheral
 * @param [in]    - ADC_Config: all ADC configuration
 * @retval        - none
 * Note           - none
 */
void MCAL_ADC_pins_set(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config)
{
	uint16_t i = 0;
	GPIO_PinConfig_t PinCfg;

	for( ; i< (ADC_Config->Number_of_channels) ; i++)
	{
		if(ADC_Config->channels[i].Channel_num < 9)
		{
			//PORTA
			PinCfg.GPIO_PinNumber = Get_the_channel_pin(ADC_Config->channels[i].Channel_num);
			PinCfg.GPIO_MODE = GPIO_MODE_Analog;
			MCAL_GPIO_Init(GPIOA , &PinCfg );

			//Specify Sampling rate
			ADCx->SMPR2 = ((ADC_Config->channels[i].channel_Sampling_rate) << ((ADC_Config->channels[i].Channel_num - 1)*3));
		}
		else
		{
			//PORTB
			PinCfg.GPIO_PinNumber = Get_the_channel_pin(ADC_Config->channels[i].Channel_num);
			PinCfg.GPIO_MODE = GPIO_MODE_Analog;
			MCAL_GPIO_Init(GPIOB , &PinCfg );

			//Specify Sampling rate
			ADCx->SMPR2 = ((ADC_Config->channels[i].channel_Sampling_rate) << ((ADC_Config->channels[i].Channel_num - 1)*3));
		}
	}
}

/**================================================================
 * @Fn            - MCAL_ADC_READ
 * @brief         - Read the data that converted by ADC
 * @param [in]    - ADCx: where x can be 1 - 2 (depending on device used) to select the ADC peripheral
 * @param [in]    - ADC_Config: all ADC configuration
 * @param [in]    - data: pointer to the data buffer that will hold the ADC data after conversion
 * @retval        - none
 * Note           - none
 */
void MCAL_ADC_READ(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config ,uint16_t *data)
{
	uint16_t i;
	if(idle == ADC_Less_power)
	{
		//For power up the ADC again
		ADCx->CR2 |= (1<<0);
	}

	for(Rank=1; Rank<= (ADC_Config->Number_of_channels) ; Rank++)
	{
		//Start conversion

		//ADON bit to start conversion
		ADCx->CR2 |= (1<<0);

		if(ADC_Config->Interrupt == ADC_Interrupt_disable)
		{
			//polling of EOC
			while(!((ADCx->SR>>1)&1));

			//read the data from DR
			data[Rank-1] = (uint16_t)ADCx->DR;
		}

		//Delay for generating interrupt
		for(i=0 ; i<60 ; i++);

		//clear the STAT bit
		ADCx->SR &= ~(1<<4);

		ADCx->SQR3 = 0;
		ADCx->SQR3 |= (ADC_Config->channels[Rank].Channel_num)-1 ;

	}

	//Reset
	ADCx->SQR3 = (ADC_Config->channels[0].Channel_num)-1;

}


/**================================================================
 * @Fn            - Get_the_channel_pin
 * @brief         - Get the channel pin number for set pins
 * @param [in]    - channel_num: Channel number (1 - 10)
 * @retval        - none
 * Note           - none
 */
uint16_t Get_the_channel_pin(uint16_t channel_num)
{
	uint16_t channel_index;

	switch(channel_num)
	{
	case Ch_A0:
		channel_index = GPIO_PIN_0;
		break;

	case Ch_A1:
		channel_index = GPIO_PIN_1;
		break;

	case Ch_A2:
		channel_index = GPIO_PIN_2;
		break;

	case Ch_A3:
		channel_index = GPIO_PIN_3;
		break;

	case Ch_A4:
		channel_index = GPIO_PIN_4;
		break;

	case Ch_A5:
		channel_index = GPIO_PIN_5;
		break;

	case Ch_A6:
		channel_index = GPIO_PIN_6;
		break;

	case Ch_A7:
		channel_index = GPIO_PIN_7;
		break;

	case Ch_B0:
		channel_index = GPIO_PIN_0;
		break;

	case Ch_B1:
		channel_index = GPIO_PIN_1;
		break;
	}

	return channel_index;
}

/*
 * ===============================================
 * 						ISR
 * ===============================================
 */

void ADC1_2_IRQHandler(void)
{
	/* ADC global interrupt */
	Data_for_IRQ = ADC1->DR ;
	Global_ADC_Cfg.channels[Rank-1].Channel_IRQ_callback(Data_for_IRQ);
}
