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


#endif /* TASKS_VAR_H_ */