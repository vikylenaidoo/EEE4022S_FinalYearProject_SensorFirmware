/*
 * spi.h
 *
 *  Created on: 15 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_SPI_H_
#define TASKS_SPI_H_

//-----------------------INCLUDES----------------------------------------//
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "var.h"
//---------------------TASKS INCLUDES-----------------------------------//

//---------------------PUBLIC DEFINES---------------------------------//

//-------------------PUBLIC VARIABLES----------------------------------//
typedef enum {
	SPI_CS_Acc 	= 0,
	SPI_CS_Gyro = 1,
	SPI_CS_Mag 	= 2,
	SPI_CS_Baro = 3,

} SPI_ChipSelectTypeDef;


//--------------------FUNCTION HEADERS----------------------------------//

void spi_initialise(void);

uint8_t spi_read_single(
		SPI_ChipSelectTypeDef cs, //chip select
		uint8_t addr //start address to read from
		);

Sensor_StatusTypeDef spi_read_burst(
		SPI_ChipSelectTypeDef cs, //chip select
		uint8_t addr, //start address to read from
		uint8_t* buffer, //buffer to store read result
		uint8_t length // length of the buffer
		);

Sensor_StatusTypeDef spi_write_single(
		SPI_ChipSelectTypeDef cs, //chip select
		uint8_t data_out, //data to be written
		uint8_t addr // address to write to
		);


#endif /* TASKS_SPI_H_ */
