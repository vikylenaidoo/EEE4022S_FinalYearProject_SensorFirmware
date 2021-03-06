/*
 * var.h
 *
 *  Created on: 20 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_VAR_H_
#define TASKS_VAR_H_



//-------------------PUBLIC DEFINES----------------------//

#define UBX_MESSAGE_SIZE					100
#define UBX_DATA_SIZE						40

//--------------------PRIORITIES--------------------------//
#define USART_RX_DMA_TC_Priority			12
#define USART_RX_DMA_TC_Sub_Priority		0

#define USART_RXNE_Priority					1
#define USART_RXNE_Sub_Priority				0

#define USART_IDLE_Priority					1
#define USART_IDLE_Sub_Priority				0

#define USART_RX_DMA_TE_Priority			13
#define USART_RX_DMA_TE_Sub_Priority		0

#define USART_TX_DMA_TE_Priority			13
#define USART_TX_DMA_TE_Sub_Priority		0




//----------------------TYPEDEFS-------------------------//

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

typedef enum{
	GNSS_OK=0,
	GNSS_ERROR,
	GNSS_READ_ERROR

}GNSS_StatusTypeDef;

typedef enum{
	UART_OK=0,
	UART_ERROR,
	UART_READ_ERROR,
	UART_SEND_ERROR
}UART_StatusTypeDef;

//---------------------GLOBAL STRUCT--------------------------------//
/*global struct to hold all data read from bosch sensors. and gnss will be updated every request sensor is read*/
struct GlobalData_Struct{

	//---------------------------START TOKEN----------------------//
	uint8_t start_token1;
	uint8_t start_token2;
	//--------------------------SENSOR DATA-----------------------------//


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


	//------------------------GNSS UBX MESSAGE---------------------------------//

	//uint8_t GNSS_UBX_MESSAGE [UBX_DATA_SIZE];
	uint8_t GNSS_UBX_MESSAGE [UBX_MESSAGE_SIZE]; //length of ubx message including header + checksum

	//------------------------CRC32 CHECKSUM--------------------------------//
	uint32_t CRC32_CHECKSUM; //32bit checksum to

};

//struct GlobalData_Struct GlobalData;
#define GLOBAL_DATA_BUFFER_SIZE		sizeof(struct GlobalData_Struct)/sizeof(uint8_t)
union {
	struct GlobalData_Struct GlobalDataStruct;
	uint8_t GlobalDataArray [GLOBAL_DATA_BUFFER_SIZE];

} GlobalDataUnion;

#endif /* TASKS_VAR_H_ */
