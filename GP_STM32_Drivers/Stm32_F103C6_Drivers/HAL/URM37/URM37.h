/*
 * URM37.h
 *
 *  Created on: Dec 5, 2023
 *      Author: Acer
 */

#ifndef HAL_URM37_URM37_H_
#define HAL_URM37_URM37_H_

#include "Stm32_F103C6_GPIO_Driver.h"
#include "Stm32_F103C8_TIM_Driver.h"
#include "Delay.h"
#include "stm32f103x6.h"
#include "Stm32f103c6_ADC.h"

/*
 *  connection pins
 */


typedef struct
{
	GPIO_TypeDef* URM37_ADC_Mode_TRIG_Port  ;    //Select between GPIO_ports

	GPIO_TypeDef* URM37_PW_Mode_TRIG_Port	  ;   //Select between GPIO_ports
	GPIO_TypeDef* URM37_PW_MODE_ECHO_Port   ;

}URM37_ports;

typedef struct
{
	uint16_t URM37_ADC_Mode_TRIG_Pin  ;    //Select between GPIO_pins
	uint16_t URM37_ADC_Mode_Ch_Pin    ;    // Select between ADC_Channels

	uint16_t URM37_PW_Mode_TRIG_Pin	 ;   //Select between GPIO_pins
	uint16_t URM37_PW_MODE_ECHO_Pin   ;

}URM37_pins;

typedef struct
{
	uint8_t URM37_Mode     ; //specify the operation mode of URM37

	URM37_pins Urm37_pins  ; //specify pins of URM37

	URM37_ports Urm37_ports; //specify the ports of the pins for URM37

}URM37_Config;


//URM37_modes
#define URM37_ADC_Mode  0
#define URM37_PWM_Mode  1

//URM_Ports


void URM37_init(URM37_Config* URM37_cfg);
void URM37_ADC_Get_dis(uint32_t* Distance);
void URM37_PW_Get_dis(uint16_t* Distance);

#endif /* HAL_URM37_URM37_H_ */


