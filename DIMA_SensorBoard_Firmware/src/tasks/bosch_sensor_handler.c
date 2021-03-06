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
 * 					mode		11		1,0		(set to normal mode)
 *
 */
static Sensor_StatusTypeDef bmp280_config(){
	//write 001000100 to BMP_CTRL_MEAS
	//uint8_t calib_data = CTRL_MEAS_OSRS_P_1|CTRL_MEAS_OSRS_T_1;
	uint8_t calib_data = CTRL_MEAS_OSRS_P_1|CTRL_MEAS_OSRS_T_1|CTRL_MEAS_MODE_NORMAL;

	if( spi_write_single(SPI_CS_Baro, calib_data, BMP280_CTRL_MEAS) != SENS_OK)
		return SENS_CONFERR;

	//if (spi_write_single(SPI_CS_Baro, 0x04, BMP280_CONFIG) != SENS_OK)
		//return SENS_CONFERR;

	return SENS_OK;
}

/**
 * @brief	configure bmx055 imu
 *
 * use Sensor_ConfigTypeDef to set
 * rates and ranges
 *
 */
static Sensor_StatusTypeDef bmx055_config(Sensor_ConfigTypeDef *f){

	//----------------ACCELEROMETER-------------------------//
	/* Accelerometer measurement range
	 * Default	: 0b(0000)0011 	= +-2g
	 * Set		: user-defined*/
	if(spi_write_single(SPI_CS_Acc, f->accel_range, BMX055_ACC_PMU_RANGE) != SENS_OK){
		return SENS_CONFERR;
	}
	/* Accelerometer measurement output data rate
	 * Default	: 0b(000)01111 	= 1000 Hz
	 * Set		: user-defined*/
	if(spi_write_single(SPI_CS_Acc, f->accel_rate, BMX055_ACC_PMU_BW) != SENS_OK){
		return SENS_CONFERR;
	}
	/* Accelerometer power mode
	 * Default	: 0b0000000(0) 	= normal mode
	 * Set		: 0b0000000(0) 	= normal mode */
	if(spi_write_single(SPI_CS_Acc, ACC_PWR_NORMAL, BMX055_ACC_PMU_LPW) != SENS_OK){
		return SENS_CONFERR;
	}

	//-----------------------GYROSCOPE-----------------------//
	/* Gyroscope measurement range
	 * Default	: 0b(00000)000 	= +-2000d/s
	 * Set		: user-defined*/
	if(spi_write_single(SPI_CS_Gyro, f->gyro_range, BMX055_GYRO_RANGE) != SENS_OK){
		return SENS_CONFERR;
	}
	/* Gyroscope measurement output data rate (bandwidth)
	 * Default	: 0b(1000)0000 	= 2000 Hz with no filter
	 * Set		: user-defined*/
	if(spi_write_single(SPI_CS_Gyro, f->gyro_rate|GYRO_BW_RESET, BMX055_GYRO_BW) != SENS_OK){
		return SENS_CONFERR;
	}
	/* Gyroscope power mode
	 * Default	: 0b0(0)0(0)000(0)	= normal mode
	 * Set		: 0b0(0)0(0)000(0)	= normal mode*/
	if(spi_write_single(SPI_CS_Gyro, GYRO_LPM1_NORMAL_MODE, BMX055_GYRO_LPM1) != SENS_OK){
		return SENS_CONFERR;
	}

	//----------------------MAGNETOMETER-------------------------//
	//currently in sleep mode, force mode when reading data

	/*Magneto meter xy repetitions
	 *choose nXY=3 ==> REPXY=1 (see pg122 table37 and pg138 of datasheet)
	 *default	: 0b00000000
	 *set to	: 0b00000001 */
	if(spi_write_single(SPI_CS_Mag, MAG_REP_XY_LOW_POWER_PRESET_REPXY, BMX055_MAG_REP_XY) != SENS_OK){
		return SENS_CONFERR;
	}

	/*Magneto meter z repetitions
	 *choose nZ=3 ==> REPZ=2 (see pg122 table37 and pg138 of datasheet)
	 *default	: 0b00000000
	 *set to	: 0b00000010 */
	if(spi_write_single(SPI_CS_Mag, MAG_REP_Z_LOW_POWER_PRESET_REPZ, BMX055_MAG_REP_Z) != SENS_OK ){
		return SENS_CONFERR;
	}



	return SENS_OK;
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

Sensor_StatusTypeDef sensor_config(){
	if(bmp280_config() != SENS_OK){
		return SENS_CONFERR;
	}

	//@TODO: decide rate and range
	Sensor_ConfigTypeDef *f;
	f->accel_range = ACC_PMU_RANGE_02;
	f->accel_rate = ACC_PMU_BW_1000;
	f->gyro_rate = GYRO_ODR_2000;
	f->gyro_range = GYRO_RANGE_RANGE_2000;
	if(bmx055_config(f) != SENS_OK){
		return SENS_CONFERR;
	}
	return SENS_OK;
}

/**
 * @brief	read x,y,z data from acc, LSB first
 * @param	buff: variable to store read values
 * @param	lenght: how many bytes to read. recommended 6
 * */
Sensor_StatusTypeDef sensor_read_acc(uint8_t *buff, uint8_t length){
	return spi_read_burst(SPI_CS_Acc, BMX055_ACC_D_X_LSB, buff, length);
}

/**
 * @brief	read x,y,z data from gyro, LSB first
 * @param	buff: variable to store read values
 * @param	lenght: how many bytes to read. recommended 6
 * */
Sensor_StatusTypeDef sensor_read_gyro(uint8_t *buff, uint8_t length){
	return spi_read_burst(SPI_CS_Gyro, BMX055_GYRO_RATE_X_LSB, buff, length);
}

/**
 * @brief	read x,y,z data from magneto, LSB first
 * @param	buff: variable to store read values
 * @param	lenght: how many bytes to read. recommended 8
 * */
Sensor_StatusTypeDef sensor_read_mag(uint8_t *buff, uint8_t length){
	//select forced mode
	Sensor_StatusTypeDef status = spi_write_single(SPI_CS_Mag, MAG_PWR_CR2_FORCED_MODE, BMX055_MAG_PWR_CR2);
	//read data
	status= spi_read_burst(SPI_CS_Mag, BMX055_MAG_XOUT_LSB, buff, length);
	return status;
}


/**
 * @brief	read pressure and temperature data from baro, MSB first
 * @param	buff: variable to store read values
 * @param	lenght: how many bytes to read. recommended 6
 * */
Sensor_StatusTypeDef sensor_read_baro(uint8_t *buff, uint8_t length){
	//select forced mode
	//Sensor_StatusTypeDef status = spi_write_single(SPI_CS_Baro, CTRL_MEAS_MODE_FORCED, BMP280_CTRL_MEAS);
	//read data
	//uint8_t mode1 = spi_read_single(SPI_CS_Baro, BMP280_CTRL_MEAS);
	Sensor_StatusTypeDef status = spi_read_burst(SPI_CS_Baro, BMP280_PRESS_MSB, buff, length);
	//uint8_t mode2 = spi_read_single(SPI_CS_Baro, BMP280_CTRL_MEAS);
	return status;
}

/*@brief	reall data from all sensors and pack into SensorData Struct
 * */
Sensor_StatusTypeDef sensor_read_all(){

	uint8_t acc_buffer [ACC_READ_LENGTH];
	uint8_t gyro_buffer [GYRO_READ_LENGTH];
	uint8_t mag_buffer [MAG_READ_LENGTH];
	uint8_t baro_buffer [BARO_READ_LENGTH];

	//read acc data
	if(sensor_read_acc(acc_buffer, ACC_READ_LENGTH) != SENS_OK)
		return SENS_ERROR;

	GlobalDataUnion.GlobalDataStruct.AccX_LSB = (acc_buffer[0])&ACC_MASK_LSB;
	GlobalDataUnion.GlobalDataStruct.AccX_MSB = acc_buffer[1];
	GlobalDataUnion.GlobalDataStruct.AccY_LSB = (acc_buffer[2])&ACC_MASK_LSB;
	GlobalDataUnion.GlobalDataStruct.AccY_MSB = acc_buffer[3];
	GlobalDataUnion.GlobalDataStruct.AccZ_LSB = (acc_buffer[4])&ACC_MASK_LSB;
	GlobalDataUnion.GlobalDataStruct.AccZ_MSB = acc_buffer[5];

	//read gyro data
	if(sensor_read_gyro(gyro_buffer, GYRO_READ_LENGTH))
		return SENS_ERROR;

	GlobalDataUnion.GlobalDataStruct.GyroX_LSB = gyro_buffer[0];
	GlobalDataUnion.GlobalDataStruct.GyroX_MSB = gyro_buffer[1];
	GlobalDataUnion.GlobalDataStruct.GyroY_LSB = gyro_buffer[2];
	GlobalDataUnion.GlobalDataStruct.GyroY_MSB = gyro_buffer[3];
	GlobalDataUnion.GlobalDataStruct.GyroZ_LSB = gyro_buffer[4];
	GlobalDataUnion.GlobalDataStruct.GyroZ_MSB = gyro_buffer[5];

	//read mag data
	if(sensor_read_mag(mag_buffer, MAG_READ_LENGTH)!= SENS_OK)
		return SENS_ERROR;

	GlobalDataUnion.GlobalDataStruct.MagX_LSB = mag_buffer[0]&MAG_MASK_XY_LSB;
	GlobalDataUnion.GlobalDataStruct.MagX_MSB = mag_buffer[1];
	GlobalDataUnion.GlobalDataStruct.MagY_LSB = mag_buffer[2]&MAG_MASK_XY_LSB;
	GlobalDataUnion.GlobalDataStruct.MagY_MSB = mag_buffer[3];
	GlobalDataUnion.GlobalDataStruct.MagZ_LSB = mag_buffer[4]&MAG_MASK_Z_LSB;
	GlobalDataUnion.GlobalDataStruct.MagZ_MSB = mag_buffer[5];
	GlobalDataUnion.GlobalDataStruct.MagHall_LSB = mag_buffer[6]&MAG_MASK_HALL_LSB;
	GlobalDataUnion.GlobalDataStruct.MagHall_MSB = mag_buffer[7];

	//read baro data
	if(sensor_read_baro(baro_buffer, ACC_READ_LENGTH)!= SENS_OK)
		return SENS_ERROR;

	GlobalDataUnion.GlobalDataStruct.BaroPress_MSB = baro_buffer[0];
	GlobalDataUnion.GlobalDataStruct.BaroPress_LSB = baro_buffer[1];
	GlobalDataUnion.GlobalDataStruct.BaroPress_XLSB = baro_buffer[2]&BARO_MASK_XLSB;
	GlobalDataUnion.GlobalDataStruct.BaroTemp_MSB = baro_buffer[3];
	GlobalDataUnion.GlobalDataStruct.BaroTemp_LSB = baro_buffer[4];
	GlobalDataUnion.GlobalDataStruct.BaroTemp_XLSB = baro_buffer[5]&BARO_MASK_XLSB;



	return SENS_OK;
}
















