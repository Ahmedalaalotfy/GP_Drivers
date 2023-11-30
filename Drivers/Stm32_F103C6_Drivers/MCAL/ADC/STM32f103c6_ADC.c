/*
 * STM32f103c6_ADC.c
 *
 *  Created on: Nov 27, 2023
 *      Author: Acer
 */


#include "Stm32f103c6_ADC.h"


uint16_t Get_the_channel_pin(uint16_t channel_num);


void MCAL_ADC_init(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config)
{
	uint32_t i=0;

	//clock enable
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

	//if(ADC_Config->Continous_Mode)
	//ADON bit
	ADCx->CR2 |= (1<<0);

	//Delay
	for(i=0 ; i<6500 ; i++);

	//channel for conversion
	ADCx->SQR3 |= (ADC_Config->channels[0])-1 ;   //sqrt = a0=1  a1=2

	//Continous mode
	ADCx->CR2 |= (ADC_Config->Continous_Mode);

	//clear START bit
	ADCx->SR &= ~(1<<4);

}


void MCAL_ADC_pins_set(ADC_Config_t* ADC_Config)
{
	uint16_t i = 0;
	GPIO_PinConfig_t PinCfg;

	for( ; i< (ADC_Config->Number_of_channels) ; i++)
	{
		if(ADC_Config->channels[i] < 9)
		{
			//PORTA
			PinCfg.GPIO_PinNumber = Get_the_channel_pin(ADC_Config->channels[i]);
			PinCfg.GPIO_MODE = GPIO_MODE_Analog;
			MCAL_GPIO_Init(GPIOA , &PinCfg );
		}
		else
		{
			//PORTB
			PinCfg.GPIO_PinNumber = Get_the_channel_pin(ADC_Config->channels[i]);
			PinCfg.GPIO_MODE = GPIO_MODE_Analog;
			MCAL_GPIO_Init(GPIOB , &PinCfg );
		}
	}
}


void MCAL_ADC_READ(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config ,uint16_t *data)
{
	int i=1;
	//uint16_t data_buffer[ADC_Config->Number_of_channels];

	for(; i<= (ADC_Config->Number_of_channels) ; i++)
	{
		//start conversion

		//ADON bit
		ADCx->CR2 |= (1<<0);

		//polling of EOC
		while(!((ADCx->SR>>1)&1));

		//read the data from DR
		data[i-1] = (uint16_t)ADCx->DR;

		//clear the STAT bit
		ADCx->SR &= ~(1<<4);

		//from data buffer to data
		//*(data+i-1) = data_buffer[i-1];

		ADCx->SQR3 = 0;
		ADCx->SQR3 |= (ADC_Config->channels[i])-1 ;

	}

	//reset
	ADCx->SQR3 = (ADC_Config->channels[0])-1;
}


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

