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
#include "var.h"
#include "gnss.h"
#include "stm32f4xx_crc.h"
//---------------------TASKS INCLUDES-----------------------------------//

//---------------------PUBLIC DEFINES---------------------------------//
//#define GNSS_NMEA

#ifdef GNSS_NMEA
	#define GNSS_BUFFER_SIZE		1024	//total bytes received from UBX-NAV-PVT message = 100

#else
#define	GNSS_BUFFER_SIZE			UBX_MESSAGE_SIZE
									//header=2 class=1 ID=1 length=2 payload=92 chksum=2
#endif


//-------------------PUBLIC VARIABLES----------------------------------//
uint8_t GNSS_RX_BUFFER[GNSS_BUFFER_SIZE];	//To store bytes recieved from uart_gnss over dma2
volatile uint16_t GNSS_DATA_LENGTH;
//uint8_t GNSS_LOG_BUFFER[GNSS_BUFFER_SIZE];

typedef enum {
	UART_DEVICE_GNSS	= 0,
	UART_DEVICE_JETSON	= 1
} UART_DeviceSelectTypeDef;

//--------------------FUNCTION HEADERS----------------------------------//

void uart_initialise();

void uart_send(uint8_t data_out, UART_DeviceSelectTypeDef device);

uint8_t uart_receive(UART_DeviceSelectTypeDef device);

UART_StatusTypeDef uart_send_to_jetson();

uint32_t CRC_calculate_8bit_datablock(uint8_t data[], uint8_t length);













#endif /* TASKS_UART_H_ */
