/*
 * spi.c
 *
 *  Created on	:	15 Aug 2020
 *      Author	:	vikylenaidoo
 * Description	:	source file for spi.h
 *					spi.c/h is the low level spi driver to provide api for communicatig with bosch sensors bmp280 and bmx055.
 *					-initialization of multi-slave spi communication interface:
*  						- 2 line full duplex
 *  					- 4.5 MHz frequency
 *  					- high CPOL and 2 edge CPHA (mode 11)
 *  					- first bit MSB
 *  					- 8-bit data size
 *  					- software set of chip select
*  					-static functions
*  						-cs_select
*  						-cs_deselect
*  					-public functions
*						-spi_initialise
*						-spi_read
*						-spi_write
 */
//---------------------------------INCLUDES-----------------------------------------//
#include "spi.h"

//-------------------------------PRIVATE DEFINES---------------------------------//
#define GPIO_SPI_Port_CS			GPIOE
#define GPIO_SPI_Pin_CS_Acc			GPIO_Pin_7
#define GPIO_SPI_Pin_CS_Gyro		GPIO_Pin_8
#define GPIO_SPI_Pin_CS_Mag			GPIO_Pin_9
#define GPIO_SPI_Pin_CS_Baro		GPIO_Pin_10

#define GPIO_SPI_Port				GPIOA
#define GPIO_SPI_Pin_SCK			GPIO_Pin_5
#define GPIO_SPI_Pin_MISO			GPIO_Pin_6
#define GPIO_SPI_Pin_MOSI			GPIO_Pin_7

#define GPIO_SPI_Pinsource_SCK		GPIO_PinSource5
#define GPIO_SPI_Pinsource_MISO		GPIO_PinSource6
#define GPIO_SPI_Pinsource_MOSI		GPIO_PinSource7

#define SPI_BaudRatePrescaler_SPI	SPI_BaudRatePrescaler_16
//------------------------------STATIC FUNCTIONS--------------------------------//

/**
 * @brief deselect cs pin means set high
 */
static void cs_deselect(SPI_ChipSelectTypeDef cs){
	switch(cs){
	case SPI_CS_Acc	 : 	GPIO_SetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Acc);	break;
	case SPI_CS_Gyro : 	GPIO_SetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Gyro);	break;
	case SPI_CS_Mag	 : 	GPIO_SetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Mag);	break;
	case SPI_CS_Baro : 	GPIO_SetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Baro);	break;
	default 		 :	;
	}
}

/**
 * @brief select cs pin means set low
 */
static void cs_select(SPI_ChipSelectTypeDef cs){
	switch(cs){
	case SPI_CS_Acc : 	GPIO_ResetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Acc);	break;
	case SPI_CS_Gyro: 	GPIO_ResetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Gyro);	break;
	case SPI_CS_Mag	: 	GPIO_ResetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Mag);	break;
	case SPI_CS_Baro: 	GPIO_ResetBits(GPIO_SPI_Port_CS, GPIO_SPI_Pin_CS_Baro);	break;
	default			:	;
	}
}

static uint8_t stm32_spi_rw(uint8_t data_out){

	/* Send byte through the SPI peripheral */
	SPI_I2S_SendData(SPI1, data_out);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) { ; }

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);

}
static void spi_tx(uint8_t data){
	stm32_spi_rw(data);
}

static uint8_t spi_rx(void){
	return stm32_spi_rw(0xff);
}
//------------------------------PUBLIC FUNCTIONS-----------------------------------//

/**
 * @brief intialise all clock and registers needed for spi. assign and initialise all gpio pins needed
 * @return void
 */
void spi_initialise(void){

	//enable SPI clock: APB2 for SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//enable GPIO clock for SCK, MOSI, MISO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//enable GPIO clock for CS pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//SPI GPIO configurations:
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure SPI Chip Select pins */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	/* SPI Accelerometer CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_CS_Acc;
	GPIO_Init(GPIO_SPI_Port_CS, &GPIO_InitStructure);

	/* SPI Gyroscope CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_CS_Gyro;
	GPIO_Init(GPIO_SPI_Port_CS, &GPIO_InitStructure);

	/* SPI Magnetometer CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_CS_Mag;
	GPIO_Init(GPIO_SPI_Port_CS, &GPIO_InitStructure);

	/* SPI Barometer CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_CS_Baro;
	GPIO_Init(GPIO_SPI_Port_CS, &GPIO_InitStructure);

	//deselect all cs pins
	cs_deselect(SPI_CS_Acc);
	cs_deselect(SPI_CS_Gyro);
	cs_deselect(SPI_CS_Mag);
	cs_deselect(SPI_CS_Baro);

	/* Configure SPI pins: SCK and MOSI with default alternate function push-pull output*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_SCK;
	GPIO_Init(GPIO_SPI_Port, &GPIO_InitStructure);

	/* SPI MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_MOSI;
	GPIO_Init(GPIO_SPI_Port, &GPIO_InitStructure);

	/* Configure MISO as Input with internal pull-up */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_SPI_Pin_MISO;
	GPIO_Init(GPIO_SPI_Port, &GPIO_InitStructure);

	/* Connect SPI pins to AF5 */
	GPIO_PinAFConfig(GPIO_SPI_Port, GPIO_SPI_Pinsource_SCK, GPIO_AF_SPI1);	// SCK
	GPIO_PinAFConfig(GPIO_SPI_Port, GPIO_SPI_Pinsource_MOSI, GPIO_AF_SPI1);	// MOSI
	GPIO_PinAFConfig(GPIO_SPI_Port, GPIO_SPI_Pinsource_MISO, GPIO_AF_SPI1);	// MISO

	/* SPI configuration */
	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_SPI; // 72000kHz/16 = 4500kHz < 10000kHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_CalculateCRC(SPI1, DISABLE);
	SPI_Cmd(SPI1, ENABLE);

	/* drain SPI */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) { ; } //wait for TX buffer to empty
	uint8_t dummyread = SPI_I2S_ReceiveData(SPI1);
	if (dummyread) { ; }
}

/**
 * @brief	read single byte of data from spi
 * @param 	cs: chip select
 * @param	addr: start address to read from
 */
uint8_t spi_read_single(SPI_ChipSelectTypeDef cs, uint8_t addr){
	cs_select(cs);

	uint8_t token = 0x80 | addr;	//read control byte = address but with MSB=1
	spi_tx(token);			//send control byte

	token = spi_rx(); 		// recieve data

	cs_deselect(cs);
	return token;
}

/**
 * @brief	send single byte of data to spi
 * @param 	cs: chip select
 * @param	addr: start address to read from
 * @param	data_out: //data to be written
 */
Sensor_StatusTypeDef spi_write_single(SPI_ChipSelectTypeDef cs, uint8_t data_out, uint8_t addr){
	cs_select(cs);

	uint8_t token = 0x7F & addr;	//write control byte = address but with MSB=0

	spi_tx(token);
	spi_tx(data_out);

	cs_deselect(cs);

	token = spi_read_single(cs, addr);

	if(token!=data_out){
		return SENS_ERROR;
	}

	return SENS_OK;
}

/**
 * @brief	read multiple bytes of data from spi
 * @param 	cs: chip select
 * @param	addr: start address to read from
 * @param	buffer: //buffer to store read result
 * @param	length // length of the buffer (how many bytes to read)
 */

Sensor_StatusTypeDef spi_read_burst(SPI_ChipSelectTypeDef cs, uint8_t addr, uint8_t* buffer, uint8_t length){
	cs_select(cs);

	uint8_t token = 0x80 | addr;	//read control byte = address but with MSB=1
	spi_tx(token);			//send control byte

	for(int i=0; i<length; i++){
		token = spi_rx();
		*(buffer+i) = token; // recieve data
	}

	cs_deselect(cs);

	return SENS_OK;

}
















