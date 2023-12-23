/*
 * PEDAL.h
 *
 *  Created on: Dec 9, 2023
 *      Author:  Mohamed Nabil Mohamed ,Omar Adel Mohamed
 */

#ifndef HAL_PEDAL_PEDAL_H_
#define HAL_PEDAL_PEDAL_H_


//Includes
#include "stm32f103x6.h"
#include <string.h>
#include "Stm32_F103C6_GPIO_Driver.h"
#include "Stm32f103c6_ADC.h"
#include "Stm32_F103C6_USART_Driver.h"
#include "Delay.h"

//-----------------------------
//User type definitions (structures)
//-----------------------------

typedef struct{
   uint16_t start;
   int16_t  steer;
   int16_t  speed;
   uint16_t checksum;
} SerialCommand;

typedef struct{
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;
} SerialFeedback;


//Defines

// [DEBUG receive] Comment to stop debug mode
// #define DEBUG_RX               // [-] Debug received data. Prints all bytes to serial (comment-out to disable)

//Pedal Config
#define PEDAL_ADC ADC1
#define PEDAL_ADC_Channel Ch_A0
#define PEDAL_USART USART1

//Communication defines
#define START_FRAME       0xABCD      // [-] Start frame definition for reliable serial communication




//================ APIs ================//
void HAL_PEDAL_Init			(void);
void HAL_PEDAL_DeInit		(void);

void HAL_PEDAL_DriveMotors	(void);




#endif /* HAL_PEDAL_PEDAL_H_ */
