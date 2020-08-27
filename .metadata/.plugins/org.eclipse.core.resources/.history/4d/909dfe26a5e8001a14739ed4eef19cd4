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
#include "var.h"

//------------------------ TYPE DEFINES --------------------------//



//------------------------FUNCTION HEADERS--------------------------//
Sensor_StatusTypeDef sensor_check_id();
Sensor_StatusTypeDef sensor_config();

Sensor_StatusTypeDef sensor_read_baro(uint8_t *buff, uint8_t length);
Sensor_StatusTypeDef sensor_read_acc(uint8_t *buff, uint8_t length);
Sensor_StatusTypeDef sensor_read_gyro(uint8_t *buff, uint8_t length);
Sensor_StatusTypeDef sensor_read_mag(uint8_t *buff, uint8_t length);










#endif /* TASKS_BOSCH_SENSOR_HANDLER_H_ */
