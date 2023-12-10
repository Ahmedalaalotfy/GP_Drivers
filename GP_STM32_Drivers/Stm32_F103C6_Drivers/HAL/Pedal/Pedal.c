/*
 * Pedal.c
 *
 *  Created on: Dec 9, 2023
 *      Author: Mohamed Nabil Mohamed ,Omar Adel Mohamed
 *
 */


#include <Pedal.h>

//Global Variables
ADC_Config_t G_ADC_Config;
USART_Config_t G_USART_Config;

// Global variables
uint8_t idx = 0;                        // Index for new data pointer
uint8_t *p;                              // Pointer declaration for the new received data
uint16_t incomingByte;
uint16_t incomingBytePrev;
uint16_t bufStartFrame;                 // Buffer Start Frame


SerialCommand Command;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

//------------------------------------------


void HAL_PEDAL_Init(void){


	//============= ADC Init ================


	G_ADC_Config.Continous_Mode = ADC_CONT_disabled;
	G_ADC_Config.Data_Alignment = ADC_Right_alignment;
	G_ADC_Config.Number_of_channels = 1;
	G_ADC_Config.channels[0].Channel_num = PEDAL_ADC_Channel;
	G_ADC_Config.channels[0].channel_Sampling_rate = SR_7_cycles;
	G_ADC_Config.channels[0].Channel_IRQ_callback = NULL;
	G_ADC_Config.Interrupt = ADC_Interrupt_disable;

	MCAL_ADC_init(PEDAL_ADC,&G_ADC_Config);

	MCAL_ADC_pins_set(PEDAL_ADC,&G_ADC_Config);


	//============= USART Init ================

	G_USART_Config.BaudRate 		= UART_BaudRate_115200;
	G_USART_Config.USART_Mode 		= UART_Mode_TX_RX;
	G_USART_Config.Parity 			= UART_Parity_NONE;
	G_USART_Config.Payload_Length 	= UART_Payload_Length_8B;
	G_USART_Config.StopBits			= UART_StopBits_1;
	G_USART_Config.HwFlowCtl		= UART_HWFlowCtrl_NONE;

	G_USART_Config.IRQ_Enable		= UART_IRQ_Enable_NONE;
	G_USART_Config.P_IRQ_CallBack	= NULL;

	MCAL_UART_Init(PEDAL_USART, &G_USART_Config);



}


void HAL_PEDAL_Read_Disable(void){


}


void HAL_PEDAL_DriveMotors(void){

	static uint16_t Pedal_data;
	static int16_t speed;          //Range [ 1000 --> -1000 ]

	// Check for new received data
	Receive();

	MCAL_ADC_READ(PEDAL_ADC, &G_ADC_Config, &Pedal_data);

	speed = ((Pedal_data*-100) / 403);


	Send(0, speed);

	Delay_ms(50);
}



// ########################## SEND ##########################
void Send(int16_t uSteer, int16_t uSpeed)
{
	uint16_t buffer ;

	// Create command
	Command.start    = (uint16_t)START_FRAME;
	Command.steer    = (int16_t)uSteer;
	Command.speed    = (int16_t)uSpeed;
	Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

	// Write to Serial

	//todo check the sending sequence --> [Least byte is sent first for now]

	//Start frame send

	//CD send
	buffer = Command.start & 0x00FF;
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);
	//AB send
	buffer = (Command.start >> 8) ;
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);

	//steering send
	buffer = (Command.steer & 0x00FF);
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);
	buffer = (Command.steer >> 8);
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);

	//speed send
	buffer = (Command.speed  & 0x00FF);
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);
	buffer = (Command.speed  >> 8) ;
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);

	//checksum send
	buffer = (Command.checksum & 0x00FF);
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);
	buffer = (Command.checksum  >> 8);
	MCAL_UART_SendData(PEDAL_USART, &buffer ,enable);

}


// ########################## RECEIVE ##########################

SerialFeedback Receive(void)
{
	// Check for new data availability in the Serial buffer
	MCAL_UART_ReceiveData(PEDAL_USART,& incomingByte, enable);                 // Read the incoming byte


	bufStartFrame = ((uint16_t)(incomingByte) << 8) | incomingBytePrev;       // Construct the start frame


	// If DEBUG_RX is defined print all incoming bytes
#ifdef DEBUG_RX
	//Serial.print(incomingByte);
	return;
#endif


	// Copy received data
	if (bufStartFrame == START_FRAME) {                     // Initialize if new data is detected
		p       = (uint8_t *)&NewFeedback;
		*p++    = incomingBytePrev;
		*p++    = incomingByte;
		idx     = 2;
	} else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  // Save the new received data
		*p++    = incomingByte;
		idx++;
	}

	// Check if we reached the end of the package
	if (idx == sizeof(SerialFeedback)) {
		uint16_t checksum;
		checksum = (uint16_t)(NewFeedback.start ^ NewFeedback.cmd1 ^ NewFeedback.cmd2 ^ NewFeedback.speedR_meas ^ NewFeedback.speedL_meas
				^ NewFeedback.batVoltage ^ NewFeedback.boardTemp ^ NewFeedback.cmdLed);

		// Check validity of the new data
		if (NewFeedback.start == START_FRAME && checksum == NewFeedback.checksum) {
			// Copy the new data
			memcpy(&Feedback, &NewFeedback, sizeof(SerialFeedback));


		} else {
			//Serial.println("Non-valid data skipped");
		}

		idx = 0;   // Reset the index (it prevents to enter in this if condition in the next cycle)
	}

	// Update previous states
	incomingBytePrev = incomingByte;
}



