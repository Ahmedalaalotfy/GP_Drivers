/*
 * Stm32_F103C6_TIM_Driver.h
 *
 *  Created on: Nov 27, 2023
 *      Author: Ahmedalaalotfy,Mohamed Nabil
 */

#ifndef INC_STM32_F103C8_TIM_DRIVER_H_
#define INC_STM32_F103C8_TIM_DRIVER_H_

//-----------------------------
//Includes
//-----------------------------

#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO_Driver.h"
#include <BIT_MATH.h>

//-----------------------------
//User type definitions (structures)

//-----------------------------

typedef enum
{
	TIMx_No_Error,
	TIMx_NOT_Found,
	TIM_MODE_NOT_Found,
	TIM_Auto_Reload_Value_Exceeded,
	TIM_Counter_Auto_Reload_Value_Exceeded,
	TIM_PWM_Auto_Reload_Value_Exceeded
}Error_status;

//struct S_IRQ_SRC
//{
//	uint8_t TXE:1 ;
//	uint8_t RXNE:1 ;
//	uint8_t ERRI:1 ;
//	uint8_t Reserved:5 ;
//};


typedef struct
{
	uint16_t   TIM_Mode ;            	 //specifies the TIM operation mode @ref TIM_Mode
	uint16_t   Prescaler ;           	 //specifies the Prescaler that will be divided on
	uint16_t   Counter_Preload_Value;
	uint16_t   Auto_Reload_Value ;    	//specifies the reloed value that will be count to


	struct {
		uint8_t Count_Direction; 		//specifies the Timer Counting Direction @ref Count_Direction_Define
	}Counter;


	struct {
		uint8_t	Channel; 				 //specifies the PWM MODE operating Channel
									  	 //[You Can only select one channel at a time]  @ref TIM_CHANNEL_Define

		uint8_t Mode; 				 	 //specifies the PWM MODE and Counter Direction @ref PWM_Mode_Define

		uint16_t Compare_value;			//specifies the value to be loaded in the actual capture/compare register (preload value).
										//The active capture/compare register contains the value to be compared to the counter
										//TIMx_CNT and signaled on OC1 output.
										// NOTE: Compare Value Must Not exceed The Auto Reload Value Set by the user Or else
										//       There will be no PWM generated.


		uint8_t Ouptut_On_Compare_Match; //specifies the PWM Output pin State [High/Low] On Compare match  @ref PWM_Compare_Match_Define

	}PWM;

	uint16_t   IRQ_Enable ;             //Specifies the source of interrupt @ref TIM_IRQ_Mode
	void (* P_IRQ_CallBack)(void);      // Set C Function() which will be called once the IRQ Happen .

}TIM_Config_t;


//-----------------------------
//Macros Configuration References
//-----------------------------

//@ref TIM_Mode


		/*TIM_Mode_Counter*/
#define TIM_Mode_Counter								0X01

// @ref Count_Direction_Define
#define Count_Direction_UP			0X00
#define Count_Direction_DOWN		0X01
#define Count_Direction_UP_DOWN		0X02


		/*TIM_Mode_PWM*/
#define TIM_Mode_PWM									0X02

// @ref PWM_Mode_Define
#define PWM_Mode_FROZEN					0X00
#define PWM_Mode_EDGE_UP				0X01	
#define PWM_Mode_EDGE_DOWN				0X02	
#define PWM_Mode_Centre_Aligned			0X03


//@ref PWM_Compare_Match_Define
#define PWM_Compare_Match_Low  	 0
#define PWM_Compare_Match_High   1

//@ref TIM_CHANNEL_Define
#define	TIM_CHANNEL_1				0X01	
#define	TIM_CHANNEL_2				0X02	
#define	TIM_CHANNEL_3				0X03	
#define	TIM_CHANNEL_4				0X04

/*TIM_Mode_Input_Capture*/
#define TIM_Mode_Input_Capture						0x3

//@ref TIM_Prescaler_option



//@ref TIM_IRQ_Mode

#define TIM_IRQ_MODE_None                      0
#define TIM_IRQ_MODE_OverFlow                  1
#define TIM_IRQ_MODE_UnderFlow                 2


//@ref Counter_Peeload_Value
//specifies the first cycle starting Value to be added to the counter register [TIMx counter (TIMx_CNT)]
// Note: This value must not exceed The Auto_Reload_Value  @ref Counter_Peeload_Value
// if no preload value is needed set it to Zero
#define Counter_Preload_Value 				0
/*
 * ===============================================
 * APIs Supported by "MCAL TIM DRIVER"
 * ===============================================
 */
Error_status MCAL_TIM_Init(TIM_TypeDef *TIMx,TIM_Config_t *TIM_Config);
void MCAL_TIM_DeInit(TIM_TypeDef *TIMx);

Error_status MCAL_TIM_GPIO_Set_Pins(TIM_TypeDef *TIMx,uint8_t TIM_Channel,uint8_t TIM_Mode);

Error_status MCAL_TIM_Start(TIM_TypeDef *TIMx);
Error_status MCAL_TIM_Stop(TIM_TypeDef *TIMx);
Error_status MCAL_TIM_Count_Reset(TIM_TypeDef *TIMx);



#endif /* INC_STM32_F103C8_TIM_DRIVER_H_ */
