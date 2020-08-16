/*
 * led.h
 *
 *  Created on: 18 Jul 2019
 *      Author: doyeo
 */

#ifndef TASKS_LED_H_
#define TASKS_LED_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "tasks/var.h"
/*/tasks Includes ------------------------------------------------------------*/

/* Public defines ------------------------------------------------------------*/
#ifdef STM32F4DISCOVERY_DEBUGGING
#define Led4_Off()      GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define Led4_On()      	GPIO_SetBits(GPIOD, GPIO_Pin_12)
#define Led3_Off()		GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define Led3_On()		GPIO_SetBits(GPIOD, GPIO_Pin_13)
#define Led5_Off()		GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define Led5_On()		GPIO_SetBits(GPIOD, GPIO_Pin_14)
#define Led6_Off()		GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define Led6_On()		GPIO_SetBits(GPIOD, GPIO_Pin_15)

#else
#define LDR_Off()      	GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define LDR_On()      	GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define LDO_Off()		GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LDO_On()		GPIO_SetBits(GPIOE, GPIO_Pin_5)
#define LDG_Off()		GPIO_ResetBits(GPIOE, GPIO_Pin_6)
#define LDG_On()		GPIO_SetBits(GPIOE, GPIO_Pin_6)
#endif
/* Public variables ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void led_initialize (void);

/* Interrupt Service Routine (ISR) -------------------------------------------*/

#endif /* TASKS_LED_H_ */
