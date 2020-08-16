/*
 * gnss.h
 *
 *  Created on: Mar 12, 2018
 *      Author: Do Yeou Ku
 */

#ifndef TASKS_GNSSIO_H_
#define TASKS_GNSSIO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*/tasks Includes ------------------------------------------------------------*/
#include "tasks/var.h"

/* Public defines ------------------------------------------------------------*/
#define STM32_GNSS_USE_DMA
#define GNSS_LOG_BUFFER_SIZE		2048

/* Public variables ----------------------------------------------------------*/
volatile FlagStatus gnss_log_on;
volatile int 		gnss_length;

uint8_t UART_RAW_Buffer	[GNSS_LOG_BUFFER_SIZE];
uint8_t GNSS_LOG_Buffer	[GNSS_LOG_BUFFER_SIZE];

/* Function prototypes -------------------------------------------------------*/
void gnss_initialize (void);
void gnss_deinitialize (void);

void xmit_usart (unsigned char);
unsigned char rsvr_usart (void);

/* Interrupt Service Routine (ISR) -------------------------------------------*/
void USART_GNSS_IRQHandler(void);
#ifdef STM32_GNSS_USE_DMA
	void DMA_USART_RX_IRQHandler (void);
	void DMA_USART_MEM_IRQHandler (void);
#endif

#endif /* TASKS_GNSSIO_H_ */
