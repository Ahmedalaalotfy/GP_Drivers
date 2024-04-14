/*
 * URM37.c
 *
 *  Created on: Dec 5, 2023
 *      Author: Acer
 */

#include "URM37.h"

/*
 * ===============================================
 * Generic Macros
 * ===============================================
 */



/*
 * ===============================================
 * Generic Variables
 * ===============================================
 */

GPIO_PinConfig_t PinCfg;
ADC_Config_t adc_cfg;
TIM_Config_t Tim_cfg;
URM37_Config URM37_GLOBAL_Cfg;

/*
 * ===============================================
 *         Generic Functions decelerations
 * ===============================================
 */

/*
 * ===============================================
 * 						APIs
 * ===============================================
 */


/**================================================================
 * @Fn            - URM37_init
 * @brief         - Initialize URM37 sensor
 * @param [in]    - URM37_cfg: all URM37 Sensor configuration
 * @retval        - none
 * Note           - none
 */
void URM37_init(URM37_Config* URM37_cfg)
{
	URM37_GLOBAL_Cfg = *URM37_cfg;

	if(URM37_cfg->URM37_Mode == URM37_ADC_Mode)
	{
		//Initialize the configured pins

		//TRIG Pin
		PinCfg.GPIO_PinNumber = URM37_cfg->Urm37_pins.URM37_ADC_Mode_TRIG_Pin;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(URM37_cfg->Urm37_ports.URM37_ADC_Mode_TRIG_Port, &PinCfg );

		MCAL_GPIO_WritePin(URM37_cfg->Urm37_ports.URM37_ADC_Mode_TRIG_Port, URM37_cfg->Urm37_pins.URM37_ADC_Mode_TRIG_Pin , GPIO_PIN_SET);

		//Analog Pin
		adc_cfg.Continous_Mode = ADC_CONT_disabled;
		adc_cfg.Data_Alignment = ADC_Right_alignment;
		adc_cfg.Interrupt = ADC_Interrupt_disable;
		adc_cfg.Number_of_channels = 1;

		adc_cfg.channels[Rank0].Channel_num = URM37_cfg->Urm37_pins.URM37_ADC_Mode_Ch_Pin;
		adc_cfg.channels[Rank0].channel_Sampling_rate = SR_7_cycles;

		MCAL_ADC_init(ADC1, &adc_cfg);
		MCAL_ADC_pins_set(ADC1,&adc_cfg);

		//Initialize delay
		HAL_Delay_Init();

		//Delay for stabilization
		Delay_ms(500);
	}
	else if(URM37_cfg->URM37_Mode == URM37_PWM_Mode)
	{
		//TRIG Pin
		PinCfg.GPIO_PinNumber = URM37_cfg->Urm37_pins.URM37_PW_Mode_TRIG_Pin;
		PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;

		MCAL_GPIO_Init(URM37_cfg->Urm37_ports.URM37_PW_Mode_TRIG_Port , &PinCfg );

		MCAL_GPIO_WritePin(URM37_cfg->Urm37_ports.URM37_PW_Mode_TRIG_Port, URM37_cfg->Urm37_pins.URM37_PW_Mode_TRIG_Pin , GPIO_PIN_SET);


		//ECHO pin
		PinCfg.GPIO_PinNumber = URM37_cfg->Urm37_pins.URM37_PW_MODE_ECHO_Pin;
		PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
		MCAL_GPIO_Init(URM37_cfg->Urm37_ports.URM37_PW_MODE_ECHO_Port , &PinCfg );

		//Initialize delay
		HAL_Delay_Init();

		//Delay for stabilization
		Delay_ms(500);

		//Timer initialization
		Tim_cfg.Counter.Count_Direction = Count_Direction_UP;
		Tim_cfg.Prescaler = 8;
		Tim_cfg.Auto_Reload_Value = 60000;
		Tim_cfg.IRQ_Enable = TIM_IRQ_MODE_None;
		Tim_cfg.P_IRQ_CallBack = NULL;
		MCAL_TIM_Init(TIM3,&Tim_cfg);
	}

}

/**================================================================
 * @Fn            - URM37_ADC_Get_dis
 * @brief         - Get distance from object through ADC reading operation from DAC pin of sensor
 * @param [in]    - Distance: Pointer to ADC data register
 * @retval        - none
 * Note           - none
 */
void URM37_ADC_Get_dis(uint32_t* Distance)
{

	//TRIG Pulse
	MCAL_GPIO_WritePin(URM37_GLOBAL_Cfg.Urm37_ports.URM37_ADC_Mode_TRIG_Port, URM37_GLOBAL_Cfg.Urm37_pins.URM37_ADC_Mode_TRIG_Pin, GPIO_PIN_RESET);
	Delay_us(1);
	MCAL_GPIO_WritePin(URM37_GLOBAL_Cfg.Urm37_ports.URM37_ADC_Mode_TRIG_Port, URM37_GLOBAL_Cfg.Urm37_pins.URM37_ADC_Mode_TRIG_Pin, GPIO_PIN_SET);

	//Delay 200ms for processing the data through DAC pin
	Delay_ms(200);

	//Read data from ADC
	MCAL_ADC_READ(ADC1, &adc_cfg, Distance);

	//for transfer to cm
	*Distance = (((*Distance * (5000*1000))/4030) / 4125);
}

/**================================================================
 * @Fn            - URM37_PW_Get_dis
 * @brief         - Get distance from pulse width that calculated through counter
 * @param [in]    - Distance: Pointer to ADC data register
 * @retval        - none
 * Note           - none
 */
void URM37_PW_Get_dis(uint16_t* Distance)
{
	//TRIG Pulse
	MCAL_GPIO_WritePin(URM37_GLOBAL_Cfg.Urm37_ports.URM37_PW_Mode_TRIG_Port, URM37_GLOBAL_Cfg.Urm37_pins.URM37_PW_Mode_TRIG_Pin, GPIO_PIN_RESET);
	Delay_us(1);
	MCAL_GPIO_WritePin(URM37_GLOBAL_Cfg.Urm37_ports.URM37_PW_Mode_TRIG_Port, URM37_GLOBAL_Cfg.Urm37_pins.URM37_PW_Mode_TRIG_Pin, GPIO_PIN_SET);

	//Timer start
	MCAL_TIM_Start(TIM3);

	//Polling mechanism
	while(GET_BIT(URM37_GLOBAL_Cfg.Urm37_ports.URM37_PW_MODE_ECHO_Port->ODR ,URM37_GLOBAL_Cfg.Urm37_pins.URM37_PW_MODE_ECHO_Pin) != 1);

	//Timer stop
	MCAL_TIM_Stop(TIM3);

	//Read data from counter register
	*Distance = TIM3->CNT;
	*Distance /= 50;

	//Reset the timer
	MCAL_TIM_Count_Reset(TIM3);
}

