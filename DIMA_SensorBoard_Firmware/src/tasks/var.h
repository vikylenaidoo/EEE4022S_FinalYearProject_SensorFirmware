/*
 * var.h
 *
 *  Created on: 20 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_VAR_H_
#define TASKS_VAR_H_



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
