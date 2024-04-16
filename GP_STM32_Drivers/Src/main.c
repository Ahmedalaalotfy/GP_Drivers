/**
 **
 * @file           : main.c
 * @author         : Ahmedalaalotfy
 * @brief          : Main program body
 **
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
 **
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
#include "Stm32_F103C6_USART_Driver.h"
#include "Stm32_F103C6_I2C_Driver.h"
#include <Pedal.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


/*****General Macros******/
#define TIM_Stop(X)  CLR_BIT(X->CR1,0)




/*****General Data******/
char Recieved_Data = 'A' ;

int counter = 0 ;

uint32_t Sensor1_distance = 0 ;
uint32_t Sensor2_distance = 0 ;
uint32_t Sensor3_distance = 0 ;
uint32_t Sensor4_distance = 0 ;
uint8_t Driver_State = 1 ;


uint32_t Sensor1_time = 0, Sensor2_time = 0 , Sensor3_time = 0 , Sensor4_time = 0;


uint8_t buffer[20]; // Buffer to store





TaskHandle_t vUART_ComunicationWithTab = NULL ;
TaskHandle_t vReadUltraonicSensor = NULL ;
TaskHandle_t vI2C_CommunicateWithAtmega32 = NULL ;




/******Tasks Prototype******/
void vUART_ComunicationWithTabHandler(void *parms);
void vReadUltraonicSensorHandler(void *parms);
void vI2C_CommunicateWithAtmega32Handler(void *parms);




void TIM3_Handler(void)
{
	counter++;
}



void clock_init(void)
{

	//Enable clock to AFIO
	AFIO_GPIO_CLK_EN();
	//Enable clock to port A
	RCC_GPIOA_CLK_EN();
	//Enable clock to port B
	RCC_GPIOB_CLK_EN();
	//Enable clock to port C
	RCC_GPIOC_CLK_EN();

}




void HW_Init(void)
{
	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////

	GPIO_PinConfig_t PinConfig;

	// Test Led
	PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_PP ;
	PinConfig.GPIO_Output_Speed = GPIO_SPEED_10M ;

	MCAL_GPIO_Init(GPIOC, &PinConfig);

	MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 , 1);

	///////////////////////////////

	GPIO_PinConfig_t PinConfig1;


	// TX , RX For Sensor 1
	PinConfig1.GPIO_PinNumber = GPIO_PIN_8;
	PinConfig1.GPIO_MODE = GPIO_MODE_OUTPUT_PP ;
	PinConfig1.GPIO_Output_Speed = GPIO_SPEED_10M ;
	MCAL_GPIO_Init(GPIOB, &PinConfig1);

	PinConfig.GPIO_PinNumber = GPIO_PIN_9;
	PinConfig.GPIO_MODE = GPIO_MODE_INPUT_PD;
	MCAL_GPIO_Init(GPIOB, &PinConfig);


	// TX , RX For Sensor 2
	PinConfig1.GPIO_PinNumber = GPIO_PIN_14;
	PinConfig1.GPIO_MODE = GPIO_MODE_OUTPUT_PP ;
	PinConfig1.GPIO_Output_Speed = GPIO_SPEED_10M ;
	MCAL_GPIO_Init(GPIOB, &PinConfig1);

	PinConfig1.GPIO_PinNumber = GPIO_PIN_15;
	PinConfig1.GPIO_MODE = GPIO_MODE_INPUT_PD;
	MCAL_GPIO_Init(GPIOB, &PinConfig1);


	// TX , RX For Sensor 3
	PinConfig1.GPIO_PinNumber = GPIO_PIN_12;
	PinConfig1.GPIO_MODE = GPIO_MODE_OUTPUT_PP ;
	PinConfig1.GPIO_Output_Speed = GPIO_SPEED_10M ;
	MCAL_GPIO_Init(GPIOB, &PinConfig1);

	PinConfig1.GPIO_PinNumber = GPIO_PIN_13;
	PinConfig1.GPIO_MODE = GPIO_MODE_INPUT_PD;
	MCAL_GPIO_Init(GPIOB, &PinConfig1);


	// TX , RX For Sensor 4
	PinConfig1.GPIO_PinNumber = GPIO_PIN_11;
	PinConfig1.GPIO_MODE = GPIO_MODE_OUTPUT_PP ;
	PinConfig1.GPIO_Output_Speed = GPIO_SPEED_10M ;
	MCAL_GPIO_Init(GPIOA, &PinConfig1);

	PinConfig1.GPIO_PinNumber = GPIO_PIN_12;
	PinConfig1.GPIO_MODE = GPIO_MODE_INPUT_PD;
	MCAL_GPIO_Init(GPIOA, &PinConfig1);


	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////
	//======================= I2C INIT =====================//
	// 			PB6:I2C1_SCL
	// 			PB7:I2C1_SDA

	I2C_Config_t I2C1CFG ;

	//I2C Controller act as Master

	I2C1CFG.General_Call_Adress_Detection = I2C_ENGC_Enable ;
	I2C1CFG.I2C_ACK_Control = I2C_ACK_Enable ;
	I2C1CFG.I2C_ClockSpeed = I2C_SCLK_SM_100K ;
	I2C1CFG.I2C_Mode =I2C_Mode_I2C ;
	I2C1CFG.P_Slave_Event_CallBack = NULL ;
	I2C1CFG.StrechMode = I2C_StretchMode_Enable ;

	MCAL_I2C_GPIO_Set_Pins(I2C1);
	MCAL_I2C_Init(I2C1, &I2C1CFG);



	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////
	//======================= UART INIT =====================//

	USART_Config_t USART1_config ;

	USART1_config.BaudRate = UART_BaudRate_115200 ;
	USART1_config.HwFlowCtl = UART_HWFlowCtrl_NONE ;
	USART1_config.IRQ_Enable = UART_IRQ_Enable_NONE ;
	USART1_config.Parity = UART_Parity_NONE ;
	USART1_config.Payload_Length = UART_Payload_Length_8B ;
	USART1_config.StopBits = UART_StopBits_1 ;
	USART1_config.USART_Mode = UART_Mode_RX;


	MCAL_UART_Init(USART1, &USART1_config);
	MCAL_UART_GPIO_Set_Pins(USART1);



	//	//////////////////////////*///////////////////////////////////////
	//	//////////////////////////*///////////////////////////////////////
	//	//======================= TIM2 INIT =====================//
	//	TIM_Config_t timer2_config ;
	//
	//	timer2_config.TIM_Mode =  TIM_IRQ_MODE_OverFlow;
	//	timer2_config.Counter.Count_Direction = Count_Direction_UP;
	//	timer2_config.Prescaler = 8 ;
	//	timer2_config.Auto_Reload_Value = 0x16A8 ;
	//	timer2_config.IRQ_Enable = TIM_IRQ_MODE_OverFlow ;
	//	timer2_config.P_IRQ_CallBack = TIM2_Handler ;
	//
	//
	//	MCAL_TIM_Init(TIM2,&timer2_config);
	//	MCAL_TIM_Count_Reset(TIM2);



	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////
	//======================= TIM3 INIT =====================//

	TIM_Config_t timer_config ;

	timer_config.TIM_Mode =  TIM_IRQ_MODE_OverFlow;
	timer_config.Counter.Count_Direction =Count_Direction_UP;
	timer_config.Prescaler = 8 ;
	timer_config.Auto_Reload_Value = 0xBBBB ;
	timer_config.IRQ_Enable = TIM_IRQ_MODE_OverFlow  ;
	timer_config.P_IRQ_CallBack =  TIM3_Handler ;

	MCAL_TIM_Init(TIM3,&timer_config);
	MCAL_TIM_Count_Reset(TIM3);
}


int main(void)
{

	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////

	clock_init();
	HW_Init();
	//HAL_Delay_Init();



	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////
	/***************Task Creation*////////////

	xTaskCreate(vUART_ComunicationWithTabHandler,"Driver State", 128 , NULL , 2 , vUART_ComunicationWithTab);
	xTaskCreate(vReadUltraonicSensorHandler,"UltrSonic", 128 , NULL , 3 , vReadUltraonicSensor);
	xTaskCreate(vI2C_CommunicateWithAtmega32Handler,"I2C", 128 , NULL , 4 , vI2C_CommunicateWithAtmega32);

	//////////////////////////*///////////////////////////////////////
	//////////////////////////*///////////////////////////////////////

	vTaskStartScheduler();

	while (1)
	{


		//		MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 , 0);
		//		Delay_ms(5000);
		//		MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 , 1);
		//		Delay_ms(5000);

	}

}


///////////////////******///////////////////

void vUART_ComunicationWithTabHandler(void *parms)
{
	while (1)
	{
		MCAL_UART_ReceiveData(USART1,&Recieved_Data, enable);

	}

}


///////////////////******///////////////////


void vReadUltraonicSensorHandler(void *parms)
{
	while (1)
	{
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 , 0); //Pull the trigger low
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 , 1); //Pull the trigger high
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 , 0); //Pull the trigger low

		while ( ! MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) ); //Wait for echo to go high

		MCAL_TIM_Start(TIM3); //Timer 3 Start to count

		while ( MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) ); //Wait for echo to go low


		Sensor1_time = TIM3->CNT ; //Get the value of timer 3

		MCAL_TIM_Stop(TIM3); //stop timer 3
		MCAL_TIM_Count_Reset(TIM3); // reset timer 3

		Sensor1_distance = (Sensor1_time/58) ; // calculate the distance

		if(counter == 1)
		{
			Sensor1_distance = 1200 ;
		}

		counter = 0 ;

		vTaskDelay(5);


		//////////////////////*//////////////////////////

		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 , 0); //Pull the trigger low
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 , 1); //Pull the trigger high
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 , 0); //Pull the trigger low

		while ( ! MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) ); //Wait for echo to go high

		MCAL_TIM_Start(TIM3); //Timer 3 Start to count

		while ( MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) ); //Wait for echo to go low


		Sensor2_time = TIM3->CNT ; //Get the value of timer 3

		MCAL_TIM_Stop(TIM3); //stop timer 3
		MCAL_TIM_Count_Reset(TIM3); // reset timer 3

		Sensor2_distance = (Sensor2_time/58) ; // calculate the distance

		if(counter == 1)
		{
			Sensor2_distance = 1200 ;
		}

		counter = 0 ;

		vTaskDelay(5);


		//////////////////////*//////////////////////////

		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 , 0); //Pull the trigger low
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 , 1); //Pull the trigger high
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 , 0); //Pull the trigger low

		while ( ! MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) ); //Wait for echo to go high

		MCAL_TIM_Start(TIM3); //Timer 3 Start to count

		while ( MCAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) ); //Wait for echo to go low


		Sensor3_time = TIM3->CNT ; //Get the value of timer 3

		MCAL_TIM_Stop(TIM3); //stop timer 3
		MCAL_TIM_Count_Reset(TIM3); // reset timer 3

		Sensor3_distance = (Sensor3_time/58) ; // calculate the distance

		if(counter == 1)
		{
			Sensor3_distance = 1200 ;
		}

		counter = 0 ;

		vTaskDelay(5);

		//////////////////////*//////////////////////////


		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 , 0); //Pull the trigger low
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 , 1); //Pull the trigger high
		vTaskDelay(1);
		MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 , 0); //Pull the trigger low

		while ( ! MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) ); //Wait for echo to go high

		MCAL_TIM_Start(TIM3); //Timer 3 Start to count

		while ( MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) ); //Wait for echo to go low


		Sensor4_time = TIM3->CNT ; //Get the value of timer 3

		MCAL_TIM_Stop(TIM3); //stop timer 3
		MCAL_TIM_Count_Reset(TIM3); // reset timer 3

		Sensor4_distance = (Sensor4_time/58) ; // calculate the distance

		if(counter == 1)
		{
			Sensor4_distance = 1200 ;
		}

		counter = 0 ;

		vTaskDelay(5);

	}
}



///////////////////******///////////////////


void vI2C_CommunicateWithAtmega32Handler(void *parms)
{
	while(1)
	{
		// Split the 32-bit number into four 8-bit chunks
		buffer[0] = (Sensor1_distance >> 24) & 0xFF; // Most significant byte
		buffer[1] = (Sensor1_distance >> 16) & 0xFF;
		buffer[2] = (Sensor1_distance >> 8) & 0xFF;
		buffer[3] = Sensor1_distance & 0xFF; // Least significant byte

		// Split the 32-bit number into four 8-bit chunks
		buffer[4] = (Sensor2_distance >> 24) & 0xFF; // Most significant byte
		buffer[5] = (Sensor2_distance >> 16) & 0xFF;
		buffer[6] = (Sensor2_distance >> 8) & 0xFF;
		buffer[7] = Sensor2_distance & 0xFF; // Least significant byte

		// Split the 32-bit number into four 8-bit chunks
		buffer[8] = (Sensor3_distance >> 24) & 0xFF; // Most significant byte
		buffer[9] = (Sensor3_distance >> 16) & 0xFF;
		buffer[10] = (Sensor3_distance >> 8) & 0xFF;
		buffer[11] = Sensor3_distance & 0xFF; // Least significant byte

		// Split the 32-bit number into four 8-bit chunks
		buffer[12] = (Sensor4_distance >> 24) & 0xFF; // Most significant byte
		buffer[13] = (Sensor4_distance >> 16) & 0xFF;
		buffer[14] = (Sensor4_distance >> 8) & 0xFF;
		buffer[15] = Sensor4_distance & 0xFF; // Least significant byte


		if( Recieved_Data == 'B' )
		{
			Driver_State = 1 ;
		}
		else if (Recieved_Data == 'A')
		{
			Driver_State = 0 ;
		}


		buffer[16] = Driver_State ;


		MCAL_I2C_Master_TX(I2C1, 0x10,buffer,17, WithStop, Start);// Write the data at the Slave address

		if (Sensor1_distance <= 20 ||  Sensor2_distance <= 20  )
		{
			MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 , 0);
		}

		else
		{
			MCAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 , 1);
		}

		vTaskDelay(30);


	}
}

///////////////////******///////////////////
