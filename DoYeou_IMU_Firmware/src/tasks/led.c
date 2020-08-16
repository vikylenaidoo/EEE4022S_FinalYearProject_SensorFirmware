/*
 * led.c
 *
 *  Created on: 18 Jul 2019
 *      Author: doyeo
 */

#include "led.h"

/* Private defines -----------------------------------------------------------*/

/* Static functions ----------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
void led_initialize (void)
{

#ifdef STM32F4DISCOVERY_DEBUGGING
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIODEN, ENABLE);

	/* Set output values LOW */
	Led3_Off();
	Led4_Off();
	Led5_Off();
	Led6_Off();

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

#else
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOEEN, ENABLE);

	/* Set output values LOW */
	LDR_Off();
	LDO_Off();
	LDG_Off();

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

#endif

}

/* Interrupt Service Routine (ISR) -------------------------------------------*/
