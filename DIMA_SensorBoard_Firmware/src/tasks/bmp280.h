/*
 * bmp280.h
 *
 *  Created on: 15 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_BMP280_H_
#define TASKS_BMP280_H_

#define BMP280_ID				0x58

//--------------------------REGISTERS------------------------------//
#define BMP280_DIGT1_LSB		0x88
#define BMP280_DIGT1_MSB		0x89
#define BMP280_DIGT2_LSB		0x8A
#define BMP280_DIGT2_MSB		0x8B
#define BMP280_DIGT3_LSB		0x8C
#define BMP280_DIGT3_MSB		0x8D
#define BMP280_DIGP1_LSB		0x8E
#define BMP280_DIGP1_MSB		0x8F
#define BMP280_DIGP2_LSB		0x90
#define BMP280_DIGP2_MSB		0x91
#define BMP280_DIGP3_LSB		0x92
#define BMP280_DIGP3_MSB		0x93
#define BMP280_DIGP4_LSB		0x94
#define BMP280_DIGP4_MSB		0x95
#define BMP280_DIGP5_LSB		0x96
#define BMP280_DIGP5_MSB		0x97
#define BMP280_DIGP6_LSB		0x98
#define BMP280_DIGP6_MSB		0x99
#define BMP280_DIGP7_LSB		0x9A
#define BMP280_DIGP7_MSB		0x9B
#define BMP280_DIGP8_LSB		0x9C
#define BMP280_DIGP8_MSB		0x9D
#define BMP280_DIGP9_LSB		0x9E
#define BMP280_DIGP9_MSB		0x9F
//#define BMP280_RESERVED		0xA0
//#define BMP280_RESERVED		0xA1
#define BMP280_CHIP_ID			0xD0 //should return 0x58
#define BMP280_RESET			0xE0
#define BMP280_STATUS			0xF3
#define BMP280_CTRL_MEAS		0xF4
#define BMP280_CONFIG			0xF5
#define BMP280_PRESS_MSB		0xF7
#define BMP280_PRESS_LSB		0xF8
#define BMP280_PRESS_XLSB		0xF9
#define BMP280_TEMP_MSB			0xFA
#define BMP280_TEMP_LSB			0xFB
#define BMP280_TEMP_XLSB		0xFC


//------------------------------BMP280 PIN MASK------------------------------------//

//for calib registers 0x88 to 0xA1
#define CALIB_DIGTx				0xFF //for reading all calibration data DIGTx and DIGPx
#define CALIB_DIGPx				0xFF //for reading all calibration data DIGTx and DIGPx

//#define BMP280_CHIP_ID		0xD0 //should return 0x58
#define CHIP_ID_ID				0xFF

//#define BMP280_RESET			0xE0
#define RESET_RESET				0xB6

//#define BMP280_STATUS			0xF3
#define STATUS_MEASURING		0b00001000
#define STATUS_IM_UPDATE		0b00000001

//#define BMP280_CTRL_MEAS		0xF4
#define CTRL_MEAS_MODE			0b00000011
#define CTRL_MEAS_MODE_SLEEP	0b00000000
#define CTRL_MEAS_MODE_FORCED	0b00000001
#define CTRL_MEAS_MODE_NORMAL	0b00000011

#define CTRL_MEAS_OSRS_P		0b00011100 //oversampling of pressure data
#define CTRL_MEAS_OSRS_P_SKIP	0b00000000
#define CTRL_MEAS_OSRS_P_1		0b00000100 //oversampling x1
#define CTRL_MEAS_OSRS_P_2		0b00001000 //oversampling x2
#define CTRL_MEAS_OSRS_P_4		0b00001100 //oversampling x4
#define CTRL_MEAS_OSRS_P_8		0b00010000 //oversampling x8
#define CTRL_MEAS_OSRS_P_16		0b00010100 //oversampling x16

#define CTRL_MEAS_OSRS_T		0b11100000 //oversampling of temperature data
#define CTRL_MEAS_OSRS_T_SKIP	0b00000000
#define CTRL_MEAS_OSRS_T_1		0b00100000 //oversampling x1
#define CTRL_MEAS_OSRS_T_2		0b01000000 //oversampling x2
#define CTRL_MEAS_OSRS_T_4		0b01100000 //oversampling x4
#define CTRL_MEAS_OSRS_T_8		0b10000000 //oversampling x8
#define CTRL_MEAS_OSRS_T_16		0b10100000 //oversampling x16

//#define BMP280_CONFIG			0xF5
#define CONFIG_T_SB				0b11100000
#define CONFIG_FILTER			0b00011100
#define CONFIG_SPI3W_EN			0b00000001

//#define BMP280_PRESS_MSB		0xF7
#define PRESS_MSB				0xFF

//#define BMP280_PRESS_LSB		0xF8
#define PRESS_LSB				0xFF

//#define BMP280_PRESS_XLSB		0xF9
#define PRESS_XLSB				0b11110000

//#define BMP280_TEMP_MSB		0xFA
#define TEMP_MSB			0xFF

//#define BMP280_TEMP_LSB		0xFB
#define TEMP_LSB			0xFF

//#define BMP280_TEMP_XLSB		0xFC
#define TEMP_XLSB		0b11110000
















#endif /* TASKS_BMP280_H_ */
