/*
 * bosch_sensor_handler.h
 *
 *  Created on: 19 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_BOSCH_SENSOR_HANDLER_H_
#define TASKS_BOSCH_SENSOR_HANDLER_H_

//--------------------------INCLUDES---------------------------//
#include "bmp280.h"
#include "bmx055.h"
#include "spi.h"


//------------------------ TYPE DEFINES --------------------------//

typedef enum {
	SENS_OK = 0,		/* 0: Successful */
	SENS_ERROR,			/* 1: R/W Error */
	SENS_NOTRDY,		/* 2: Not Ready */
	SENS_PARERR,		/* 3: Invalid Parameter */
	SENS_IDERR,			/* 4: Invalid ID */
	SENS_CONFERR,		/* 5: Configuration Error */
	SENS_INTCONFERR,	/* 6: Interrupt Configuration Error */
}Sensor_StatusTypeDef;


//------------------------FUNCTION HEADERS--------------------------//
Sensor_StatusTypeDef sensor_check_id();














#endif /* TASKS_BOSCH_SENSOR_HANDLER_H_ */
