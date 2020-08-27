/*
 * var.h
 *
 *  Created on: 20 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_VAR_H_
#define TASKS_VAR_H_


//-------------------PUBLIC DEFINES----------------------//
#define GNSS_RX_DMA_Priority			13
#define GNSS_RX_DMA_Sub_Priority		0

#define USART_RXNE_Priority				1
#define USART_RXNE_Sub_Priority			0

#define USART_IDLE_Priority				9
#define USART_IDLE_Sub_Priority			0

#define USART_RX_DMA_Priority			13
#define USART_RX_DMA_Sub_Priority		0

#define MEM2MEM_DMA_Priority			14
#define MEM2MEM_DMA_Sub_Priority		0




//----------------------TYPEDEFS-------------------------//

typedef enum {
	SENS_OK = 0,		/* 0: Successful */
	SENS_ERROR,			/* 1: R/W Error */
	SENS_NOTRDY,		/* 2: Not Ready */
	SENS_PARERR,		/* 3: Invalid Parameter */
	SENS_IDERR,			/* 4: Invalid ID */
	SENS_CONFERR,		/* 5: Configuration Error */
	SENS_INTCONFERR,	/* 6: Interrupt Configuration Error */
}Sensor_StatusTypeDef;


typedef struct{
	uint8_t accel_rate;
	uint8_t accel_range;
	uint8_t gyro_rate;
	uint8_t gyro_range;
	uint8_t mag_rate;
	uint8_t mag_range_xy;
	uint8_t mag_range_z;
}Sensor_ConfigTypeDef;

#endif /* TASKS_VAR_H_ */
