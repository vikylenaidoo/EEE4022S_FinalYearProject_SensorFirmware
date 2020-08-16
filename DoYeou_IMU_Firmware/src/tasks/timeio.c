/*
 * timeio.c
 *
 *  Created on: Mar 20, 2018
 *      Author: Do Yeou Ku
 */

#include "tasks/timeio.h"

/* Private defines -----------------------------------------------------------*/
#define RCC_APBPeriphClockCmd_STAMP				RCC_APB1PeriphClockCmd
#define RCC_APBPeriph_STAMP						RCC_APB1Periph_TIM3

// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1)
#define TIMER_STAMP								TIM3
#define TIMER_STAMP_PSC							41
#define TIMER_STAMP_ARR							199

#define TIMER_STAMP_IRQn						TIM3_IRQn
#define TIMER_STAMP_IRQHandler					TIM3_IRQHandler


/* Static functions ----------------------------------------------------------*/
static
void timestamp_on (void)
{
	/* Timer Configuration */
	RCC_APBPeriphClockCmd_STAMP(RCC_APBPeriph_STAMP, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_Prescaler = TIMER_STAMP_PSC;
	TIM_InitStructure.TIM_Period = TIMER_STAMP_ARR;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_STAMP, &TIM_InitStructure);

	/* Interrupt Enable */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIMER_STAMP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER_STAMP_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER_STAMP_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIMER_STAMP, TIM_IT_Update, ENABLE);
}

/* Public functions ----------------------------------------------------------*/
void timestamp_initialize (void)
{
	time_stamp.clk_1Hz		= 0;
	time_stamp.clk_10kHz	= 0;
	timestamp_on();
}

void timestamp_enable (void)
{
	/* Timer Enable */
	TIM_Cmd(TIMER_STAMP, ENABLE);
}

void timestamp_disable (void)
{
	/* Timer Enable */
	TIM_Cmd(TIMER_STAMP, DISABLE);
}

/* Interrupt Service Routine (ISR) -------------------------------------------*/
void TIMER_STAMP_IRQHandler (void)
{
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIMER_STAMP, TIM_IT_Update))
    {

    	time_stamp.clk_10kHz++;
    	bmx055_delay++;

        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIMER_STAMP, TIM_IT_Update);
    }
}
