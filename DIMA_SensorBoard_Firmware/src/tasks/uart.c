/*
 * uart.c
 *
 *  Created on: 	15 Aug 2020
 *      Author: 	vikylenaidoo
 *     Details:		low level UART driver
 */

//-----------------------------INLCUDES---------------------------//
#include "uart.h"

//------------------------------DEFINES--------------------------//
/*GNSS UART*/
#define RCC_APBPeriph_USART_GNSS	   			RCC_APB2Periph_USART1
#define RCC_AHBPeriph_USART_GNSS	   			RCC_AHB1Periph_GPIOB
#define USART_GNSS								USART1
#define USART_BaudRate_USART_GNSS				115200

#define GPIO_USART_GNSS							GPIOB
#define GPIO_Pin_USART_GNSS_TX					GPIO_Pin_6
#define GPIO_Pin_USART_GNSS_RX					GPIO_Pin_7

#define GPIO_Pin_USART_GNSS_TX_Src				GPIO_PinSource6
#define GPIO_Pin_USART_GNSS_RX_Src				GPIO_PinSource7

#define GPIO_AF_USART_GNSS						GPIO_AF_USART1



/*JETSON UART*/


//------------------------STATIC FUNCTIONS-------------------//


//-----------------------PUBLIC FUNCTIONS------------------//

void uart_initialise(){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APBPeriph_USART_GNSS, ENABLE);
	/* Enable GPIO clock for USART pins */
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph_USART_GNSS, ENABLE);

	/*----------USART GPIO Configuration -------------------------------*/

	/* Configure USART pins: TX & RX */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	// USART TX pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART_GNSS_TX;	//pin 6
	GPIO_Init(GPIO_USART_GNSS, &GPIO_InitStructure);

	// USART RX pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART_GNSS_RX;
	GPIO_Init(GPIO_USART_GNSS, &GPIO_InitStructure);

	// Connect USART pins to AF
	GPIO_PinAFConfig(GPIO_USART_GNSS, GPIO_Pin_USART_GNSS_TX_Src, GPIO_AF_USART_GNSS); //
	GPIO_PinAFConfig(GPIO_USART_GNSS, GPIO_Pin_USART_GNSS_RX_Src, GPIO_AF_USART_GNSS);

	/* -------------USART configuration-------------------- */
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_BaudRate = USART_BaudRate_USART_GNSS;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART_GNSS, &USART_InitStructure);

	//Enable UART
	USART_Cmd(USART_GNSS, ENABLE);
}

void uart_send(uint8_t data_out, UART_DeviceSelectTypeDef device){

}

uint8_t uart_receive(UART_DeviceSelectTypeDef device){
	uint8_t data;
	switch (device) {
		case UART_DEVICE_GNSS:
			while(USART_GetFlagStatus(USART_GNSS, USART_FLAG_RXNE) != SET){ ; } //wait for rx buffer to fill
			//read rx buffer
			data = (uint8_t)USART_ReceiveData(USART_GNSS); //@TOD0: 16 bit or 8 bit?
			break;
		case UART_DEVICE_JETSON:
			//code here
			break;
		default:
			data=0xff;
			break;
	}

	return data;
}
















