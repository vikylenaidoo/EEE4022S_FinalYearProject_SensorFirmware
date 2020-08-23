/*
 * uart.h
 *
 *  Created on: 15 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_UART_H_
#define TASKS_UART_H_

//-----------------------INCLUDES----------------------------------------//
#include "stm32f4xx_usart.h"
//---------------------TASKS INCLUDES-----------------------------------//

//---------------------PUBLIC DEFINES---------------------------------//

//-------------------PUBLIC VARIABLES----------------------------------//
typedef enum {
	UART_DEVICE_GNSS	= 0,
	UART_DEVICE_JETSON	= 1
} UART_DeviceSelectTypeDef;

//--------------------FUNCTION HEADERS----------------------------------//
void uart_initialise();
void uart_send(uint8_t data_out, UART_DeviceSelectTypeDef device);
uint8_t uart_receive(UART_DeviceSelectTypeDef device);

#endif /* TASKS_UART_H_ */
