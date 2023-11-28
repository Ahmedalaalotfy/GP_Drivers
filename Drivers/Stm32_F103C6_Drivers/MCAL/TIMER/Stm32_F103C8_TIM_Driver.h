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


//-----------------------------
//User type definitions (structures)

//-----------------------------

typedef enum
{
	TIMx_NOT_Found,
	TIM_MODE_NOT_Found,
	TIM_Auto_Reload_Value_Exceeded
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
	uint16_t   TIM_Mode ;             //specifies the TIM operation mode @ref TIM_Mode
	uint16_t   Prescaler ;            //specifies the Prescaler that will be divided on
	uint16_t   Auto_Reload_status ;   //specifies the auto reload buffer status @ref TIM_Auto_Reload_status
	uint16_t   Auto_Reload_Value ;    //specifies the reloed value that will be count to
	uint16_t   IRQ_Enable ;           //Specifies the source of interrupt @ref TIM_IRQ_Mode
	void (* P_IRQ_CallBack)(void);  // Set C Function() which will be called once the IRQ Happen .

}TIM_Config_t;


//-----------------------------
//Macros Configuration References
//-----------------------------

//@ref TIM_Mode
#define TIM_Mode_UP_Count                          0X01
#define TIM_Mode_DOWN_Count                        0X02
#define TIM_Mode_UP_DOWN_Count                     0X03
#define TIM_Mode_Output_Compare                    0X04
#define TIM_Mode_PWM                               0X05

//@ref TIM_Prescaler_option


//@ref TIM_Auto_Reload_status
#define TIM_Auto_Reload_Bufferd            (1<<7)
#define TIM_Auto_Reload_Not_Bufferd        (0<<7)

//@ref TIM_IRQ_Mode

#define TIM_IRQ_MODE_None                      0
#define TIM_IRQ_MODE_OverFlow                  1
#define TIM_IRQ_MODE_UnderFlow                 2





/*
 * ===============================================
 * APIs Supported by "MCAL TIM DRIVER"
 * ===============================================
 */
void MCAL_TIM_Init(TIM_TypeDef *TIMx,TIM_Config_t *TIM_Config);
void MCAL_TIM_DeInit(TIM_TypeDef *TIMx);

void MCAL_TIM_GPIO_Set_Pins(TIM_TypeDef *TIMx);



#endif /* INC_STM32_F103C8_TIM_DRIVER_H_ */
