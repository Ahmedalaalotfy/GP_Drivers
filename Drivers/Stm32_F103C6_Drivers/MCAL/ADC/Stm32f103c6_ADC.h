/*
 * Stm32f103c6_ADC.h
 *
 *  Created on: Nov 27, 2023
 *      Author: Acer
 */

#ifndef INC_STM32F103C6_ADC_H_
#define INC_STM32F103C6_ADC_H_

//-----------------------------
//Includes
//-----------------------------
#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO_Driver.h"


//-----------------------------
//User type definitions (structures)
//-----------------------------

typedef struct
{
	uint16_t Number_of_channels  ; //specifiy the number of channels of ADC

	uint16_t channels[10]  		 ; //specify the number of channels to read

	uint16_t Continous_Mode	     ; //specify the Adc CONT mode
							      //this parameter can be a value of @ref ADC_CONT_Mode

	//void(* P_IRQ_callback)(struct S_IRQ_SRC IRQ_SRC) ; //pointer to function that called when IRQ happen

}ADC_Config_t;

//-----------------------------
//Macros Configuration References
//-----------------------------

//@ref ADC_Scan_Mode
#define ADC_Scan_disabled    0
#define ADC_Scan_enabled     (uint32_t)(0x100)

//@ref ADC_Watchdog_Mode
#define ADC_Watchdog_disabled  0




#define ADC_Watchdog_enabled_for_ch0 	(uint32_t)(1<<23)
#define ADC_Watchdog_enabled_for_ch1	(uint32_t)(0x800200)
#define ADC_Watchdog_enabled_for_ch2	(uint32_t)(0x800201)
#define ADC_Watchdog_enabled_for_ch3	(uint32_t)(0x800202)
#define ADC_Watchdog_enabled_for_ch4	(uint32_t)(0x800203)
#define ADC_Watchdog_enabled_for_ch5	(uint32_t)(0x800204)
#define ADC_Watchdog_enabled_for_ch6	(uint32_t)(0x800205)
#define ADC_Watchdog_enabled_for_ch7	(uint32_t)(0x800206)
#define ADC_Watchdog_enabled_for_ch8	(uint32_t)(0x800207)
#define ADC_Watchdog_enabled_for_ch9	(uint32_t)(0x800208)

#define ADC_Watchdog_enabled_for_all_channels	(uint32_t)(1<<23)


//@ref ADC_CONT_Mode
#define ADC_CONT_disabled    0
#define ADC_CONT_enabled     (uint32_t)(0x2)


//ADC Channels

#define CH_A0   GPIO_PIN0
#define CH_A1   GPIO_PIN1
#define CH_A2   GPIO_PIN2
#define CH_A3   GPIO_PIN3
#define CH_A4   GPIO_PIN4
#define CH_A5   GPIO_PIN5
#define CH_A6   GPIO_PIN6
#define CH_A7   GPIO_PIN7
#define CH_B0   GPIO_PIN0
#define CH_B1   GPIO_PIN2


typedef enum
{
	Ch_A0=1 , Ch_A1 , Ch_A2 , Ch_A3 ,Ch_A4 , Ch_A5 , Ch_A6 , Ch_A7 , Ch_B0 , Ch_B1
}ADC_channels;


/*
* ===============================================
* APIs Supported by "MCAL SPI DRIVER"
* ===============================================
*/
void MCAL_ADC_init(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config);
void MCAL_ADC_pins_set(ADC_Config_t* ADC_Config);
void MCAL_ADC_READ(ADC_TypeDef* ADCx , ADC_Config_t* ADC_Config ,uint16_t *data);


#endif /* INC_STM32F103C6_ADC_H_ */
