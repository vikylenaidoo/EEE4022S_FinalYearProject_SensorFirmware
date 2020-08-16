/*
 * timeio.h
 *
 *  Created on: Mar 20, 2018
 *      Author: Do Yeou Ku
 */

#ifndef TASKS_TIMEIO_H_
#define TASKS_TIMEIO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*/tasks Includes ------------------------------------------------------------*/
#include "tasks/var.h"

#include "tasks/led.h"

/* Public defines ------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
Time_ConfigTypeDef time_stamp;

/* Function prototypes -------------------------------------------------------*/
void timestamp_initialize (void);

void timestamp_enable (void);
void timestamp_disable (void);

/* Interrupt Service Routine (ISR) -------------------------------------------*/
void TIMER_STAMP_IRQHandler (void);

#endif /* TASKS_TIMEIO_H_ */
