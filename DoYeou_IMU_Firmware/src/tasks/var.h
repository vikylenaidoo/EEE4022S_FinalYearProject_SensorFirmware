/*
 * var.h
 *
 *  Created on: 29 Jul 2019
 *      Author: doyeo
 */

#ifndef TASKS_VAR_H_
#define TASKS_VAR_H_

/* Includes ------------------------------------------------------------------*/

/*/tasks Includes ------------------------------------------------------------*/

/* Public defines ------------------------------------------------------------*/
#define STM32F4DISCOVERY_DEBUGGING

#define TIMER_STAMP_Priority			8
#define TIMER_STAMP_Sub_Priority		0

#define EXTI_BARO_Priority				15
#define EXTI_BARO_Sub_Priority			0

#define EXTI_PPS_Priority				7
#define EXTI_PPS_Sub_Priority			0

#define USART_IDLE_Priority				9
#define USART_IDLE_Sub_Priority			0

#define EXTI_ACCEL_Priority				10
#define EXTI_ACCEL_Sub_Priority			0

#define EXTI_GYRO_Priority				11
#define EXTI_GYRO_Sub_Priority			0

#define EXTI_MAG_Priority				12
#define EXTI_MAG_Sub_Priority			0

#define USART_RX_DMA_Priority			13
#define USART_RX_DMA_Sub_Priority		0

#define MEM2MEM_DMA_Priority			14
#define MEM2MEM_DMA_Sub_Priority		0

#define MAG_COUNT_1000HZ 	50
#define MAG_COUNT_125HZ 	7

/* Typedefs ------------------------------------------------------------------*/

typedef struct {
	unsigned int log_enabled;
	unsigned int file_opened;
	unsigned int filename_ok;
}File_ConfigTypeDef;

typedef struct {
	volatile uint32_t clk_1Hz;		/* PPS update : 1 Hz   */
	volatile uint16_t clk_10kHz;	/* ARR update : 10 KHz */
}Time_ConfigTypeDef;

typedef struct{
	uint8_t accel_rate;
	uint8_t accel_range;
	uint8_t gyro_rate;
	uint8_t gyro_range;
	uint8_t mag_rate;
	uint8_t mag_range_xy;
	uint8_t mag_range_z;
}Sensor_ConfigTypeDef;

typedef enum {
	SENS_OK = 0,		/* 0: Successful */
	SENS_ERROR,			/* 1: R/W Error */
	SENS_NOTRDY,		/* 2: Not Ready */
	SENS_PARERR,		/* 3: Invalid Parameter */
	SENS_IDERR,			/* 4: Invalid ID */
	SENS_CONFERR,		/* 5: Configuration Error */
	SENS_INTCONFERR,	/* 6: Interrupt Configuration Error */
}Sensor_StatusTypeDef;

typedef enum {
	Accelerometer = 0,
	Gyroscope = 1,
	Magnetometer = 2,
	Barometer = 3,
}Sensor_TypeDef;

/* Public variables ----------------------------------------------------------*/
File_ConfigTypeDef file_flag;
FlagStatus division_line;
int bmx055_delay;		// overflow every 10 ms
int log_delay;			// start logging after SYSTEM_LOG_DELAY_COUNT GPS data

/* Function prototypes -------------------------------------------------------*/

#endif /* TASKS_VAR_H_ */
