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
#define ACC_READ_LENGTH			6
#define GYRO_READ_LENGTH		6
#define MAG_READ_LENGTH			8
#define BARO_READ_LENGTH		6

#define	ACC_MASK_LSB			0b11110000
#define MAG_MASK_XY_LSB			0b11111000
#define MAG_MASK_Z_LSB			0b11111110
#define MAG_MASK_HALL_LSB		0b11111100
#define BARO_MASK_XLSB			0b11110000



/*struct to hold all data read from bosch sensors. will be updated every request sensor is read*/
struct SensorData{

	/*	Acc Data		width		structure					format
	 * ---------------------------------------------------------------------------
	 *	XYZ Axis: 		12 bits	 	8bits MSB + 4bits LSB		2s complement
	 *total=6 bytes	 */
	uint8_t AccX_LSB;
	uint8_t AccX_MSB;
	uint8_t AccY_LSB;
	uint8_t AccY_MSB;
	uint8_t AccZ_LSB;
	uint8_t AccZ_MSB;

	/*	Gyro Data		width		structure					format
	 * ---------------------------------------------------------------------------
	 *	XYZ Axis: 		16 bits	 	8bits MSB + 8bits LSB		2s complement
	 *total = 6 bytes*/
	uint8_t GyroX_LSB;
	uint8_t GyroX_MSB;
	uint8_t GyroY_LSB;
	uint8_t GyroY_MSB;
	uint8_t GyroZ_LSB;
	uint8_t GyroZ_MSB;

	/*	Magneto Data		width		structure					format
	 * ----------------------------------------------------------------------------
	 * 	XY Axis: 		13 bits	 	8bits MSB + 5bits LSB		2s complement
	 *   Z Axis:		15 bits		8bits MSB + 7bits LSB		2s complement
	 *    RHALL:		14 bits		8bits MSB + 6bits LSB		unsigned
	 * total = 8 bytes*/
	uint8_t MagX_LSB;
	uint8_t MagX_MSB;
	uint8_t MagY_LSB;
	uint8_t MagY_MSB;
	uint8_t MagZ_LSB;
	uint8_t MagZ_MSB;
	uint8_t MagHall_LSB;
	uint8_t MagHall_MSB;


	/*	Baro Data			width		structure								format
	 * ---------------------------------------------------------------------------------
	 *	   Pressure: 		20 bits	 	8bits MSB + 8bits LSB + 4bits XLSB		unsigned
	 *  Temperature:		20 bits		8bits MSB + 8bits LSB + 4bits XLSB		unsigned
	 * total = 6 bytes*/
	uint8_t BaroPress_MSB;
	uint8_t BaroPress_LSB;
	uint8_t BaroPress_XLSB;
	uint8_t BaroTemp_MSB;
	uint8_t BaroTemp_LSB;
	uint8_t BaroTemp_XLSB;

};

//struct SensorData SensorDataGlobal;


//------------------------FUNCTION HEADERS--------------------------//
Sensor_StatusTypeDef sensor_check_id();
Sensor_StatusTypeDef sensor_config();

Sensor_StatusTypeDef sensor_read_baro(uint8_t *buff, uint8_t length);
Sensor_StatusTypeDef sensor_read_acc(uint8_t *buff, uint8_t length);
Sensor_StatusTypeDef sensor_read_gyro(uint8_t *buff, uint8_t length);
Sensor_StatusTypeDef sensor_read_mag(uint8_t *buff, uint8_t length);

Sensor_StatusTypeDef sensor_read_all();










#endif /* TASKS_BOSCH_SENSOR_HANDLER_H_ */
