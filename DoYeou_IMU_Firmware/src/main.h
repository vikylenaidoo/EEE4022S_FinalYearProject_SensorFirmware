/*
 * main.h
 *
 *  Created on: Mar 12, 2018
 *      Author: Do Yeou Ku
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*/tasks Includes ------------------------------------------------------------*/
#include "tasks/led.h"
#include "tasks/memory.h"
#include "tasks/gnssio.h"
#include "tasks/timeio.h"
#include "tasks/interrupt.h"
#include "tasks/sensorio.h"

/* Public defines ------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){ return; }
uint16_t EVAL_AUDIO_GetSampleCallBack(void){ return -1; }


#endif /* MAIN_H_ */
