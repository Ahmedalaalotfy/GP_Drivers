/*
 * Stm32_F103C6_WWDG_Driver.h
 *
 *  Created on: Dec 6, 2023
 *      Author: Ahmedalaalotfy, Mohamed Nabil
 */

#ifndef INC_STM32_F103C8_WWDG_DRIVER_H_
#define INC_STM32_F103C8_WWDG_DRIVER_H_

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
	WWDG_OK,
	WWDG_Counter_Wrong_Range,
	WWDG_Window_Wrong_Range
}WWDG_Error_status;

typedef struct
{
	uint16_t   Counter_Start_VAL ;        //specifies the starting value to start down counting ,
	                                     // note : THis value must be between 127-65
	uint16_t   Prescaler ;           	 //specifies the Prescaler that will be divided on @ref WWDG_Prescaler_define

	uint16_t   Window_Value ;            //specifies the value to compare with the counter value to prevent counter refresh in specified time range
                                       	// note : THis value must be between 127-65

	uint16_t   EWI_IRQ_Enable ;          //Specifies the source of interrupt @ref WWDG_EWI_IRQ_Enable_define
	void (* P_IRQ_CallBack)(void);      // Set C Function() which will be called once the IRQ Happen .

}WWDG_Config_t;



//-----------------------------
//Macros Configuration References
//-----------------------------

//@ref WWDG_Prescaler_define
#define  WWDG_Counter_div_1          (0x0<<7)
#define  WWDG_Counter_div_2          (0x1<<7)
#define  WWDG_Counter_div_4          (0x2<<7)
#define  WWDG_Counter_div_8          (0x3<<7)

//@ref WWDG_EWI_IRQ_Enable_define
#define WWDG_EWI_IRQ_None                     0x0
#define WWDG_EWI_IRQ_Enable                   0x1




/*
* ===============================================
* APIs Supported by "MCAL WWDG DRIVER"
* ===============================================
*/

WWDG_Error_status MCAL_WWDG_Init(WWDG_Config_t *WWDG_Config);
WWDG_Error_status MCAL_WWDG_Start(void);
WWDG_Error_status MCAL_WWDG_Kick(void);


#endif /* INC_STM32_F103C6_WWDG_DRIVER_H_ */
