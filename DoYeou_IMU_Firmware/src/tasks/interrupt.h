/*
 * interrupt.h
 *
 *  Created on: Mar 5, 2018
 *      Author: Do Yeou Ku
 */

#ifndef TASKS_INTERRUPT_H_
#define TASKS_INTERRUPT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*/tasks Includes ------------------------------------------------------------*/
#include "tasks/var.h"
#include "tasks/timeio.h"
#include "tasks/sensorio.h"

#include "tasks/led.h"

/* Public defines ------------------------------------------------------------*/
#define STM32_IMU_USE

/* Public variables ----------------------------------------------------------*/
volatile FlagStatus imu_log_on;

/* Function prototypes -------------------------------------------------------*/
void exti_initialize (void);
void exti_deinitialize (void);

/* Interrupt Service Routine (ISR) -------------------------------------------*/
void EXTILine_ACCEL_IRQHandler(void);
void EXTILine_GYRO_IRQHandler(void);
void EXTILine_MAG_IRQHandler(void);
void EXTILine_BARO_IRQHandler(void);

#endif /* TASKS_INTERRUPT_H_ */
