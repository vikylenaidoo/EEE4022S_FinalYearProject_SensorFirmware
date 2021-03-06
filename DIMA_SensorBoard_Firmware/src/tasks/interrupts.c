/*
 * interrupts.c
 *
 *  Created on: 21 Sep 2020
 *      Author: vikylenaidoo
 */

//------------------------------------------ INCLUDES ------------------------------//
#include "interrupts.h"
#include "uart.h"
//------------------------------------------ DEFINES ------------------------------//
#define	RCC_AHBPeriph_GPIO_EXTI		RCC_AHB1Periph_GPIOD
#define GPIO_EXTI					GPIOD
#define GPIO_Pin_EXTI				GPIO_Pin_10

//------------------------------------------STATIC FUNCTIONS ------------------------------//



//------------------------------------------PUBLIC FUNCTIONS ------------------------------//
void exti_initialise(){
	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Enable clock for GPIOx */
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph_GPIO_EXTI, ENABLE);

	//configure input port
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

	// EXTI input pin configuratio
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI;
	GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);

	// SYSCFG EXTI_Line configuration
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, GPIO_PinSource10);


	//EXTI config
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);

	//EXTI NVIC config
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);




}


//------------------------------------------ INTERRUPT HANDLER ------------------------------//


void EXTI15_10_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line10)){
		UART_StatusTypeDef uart_status = uart_send_to_jetson();
		EXTI_ClearITPendingBit(EXTI_Line10);

	}

}



