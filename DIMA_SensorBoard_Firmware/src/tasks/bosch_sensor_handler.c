/*
 * bosch_sensor_handler.c
 *
 *  Created on: 19 Aug 2020
 *      Author: vikylenaidoo
 */

//-------------------------------INCLUDES-------------------------------//
#include "bosch_sensor_handler.h"
//-------------------------------DEFINES----------------------------------//

//------------------------------STATIC FUNCTIONS--------------------------------//
/* @brief change magneto mode between sleep / suspend
 * cmd==ENABLE: put the magneto to sleep mode
 * cmd==Disable: put magneto to suspend mode
 */
static void sensor_mag_power_control(FunctionalState cmd){
	if(cmd == ENABLE){
		spi_write_single(SPI_CS_Mag, MAG_PWR_CR1_POWER_CONTROL_BIT, BMX055_MAG_PWR_CR1);
	}
	else{
		spi_write_single(SPI_CS_Mag, 0x00, BMX055_MAG_PWR_CR1);
	}
}

/**
 * @brief	initialise bmp280 barometer
 *
 * register			setting		value	bits	comment
 * ----------------------------------------------------
 * BMP_CTRL_MEAS	osrs_t		001		7,6,5	x1 sampling
 * 					osrs_p		001		4,3,2	x1 sampling
 * 					mode		00		1,0		initial=sleep mode. (force mode per measurement)
 *
 */
static Sensor_StatusTypeDef bmp280_config(){
	//write 001000100 to BMP_CTRL_MEAS
	uint8_t calib_data = CTRL_MEAS_OSRS_P_1|CTRL_MEAS_OSRS_T_1;
	return spi_write_single(SPI_CS_Baro, calib_data,BMP280_CTRL_MEAS);

}

/**
 * @brief	configure bmx055 imu
 *
 * register			setting		value	bits	comment
 * ----------------------------------------------------
 *
 */
static Sensor_StatusTypeDef bmx055_config(){


}


//-----------------------------PUBLIC FUNCTIONS-------------------------------//

/**
 * @brief used to check sensors are present
 */
Sensor_StatusTypeDef sensor_check_id(){

	uint8_t acc_id = spi_read_single(SPI_CS_Acc, BMX055_ACC_CHIP_ID);
	if(acc_id != BMX055_ACC_ID){
		return SENS_IDERR;
	}

	uint8_t gyro_id = spi_read_single(SPI_CS_Gyro, BMX055_GYRO_CHIP_ID);
	if(gyro_id != BMX055_GYRO_ID){
		return SENS_IDERR;
	}

	sensor_mag_power_control(ENABLE);
	uint8_t mag_id = spi_read_single(SPI_CS_Mag, BMX055_MAG_CHIP_ID);
	if(mag_id != BMX055_MAG_ID){
		return SENS_IDERR;
	}

	uint8_t baro_id = spi_read_single(SPI_CS_Baro, BMP280_CHIP_ID);
	if(baro_id != BMP280_ID){
		return SENS_IDERR;
	}

	return SENS_OK;

}




