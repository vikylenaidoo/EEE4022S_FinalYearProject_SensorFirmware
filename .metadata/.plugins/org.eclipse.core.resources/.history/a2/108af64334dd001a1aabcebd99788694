/*
 * sensor.c
 *
 *  Created on	:	Mar 06, 2019
 *      Author	:	Do Yeou Ku
 *  Description	:	A source file for sensor.h
 *  				The "seror.h/c" is the low-level implementation of Bosch sensors and include:
 *  				- initialization of multi-slave spi communication interface:
 *  					- 2 line full duplex
 *  					- 4.5 MHz frequency
 *  					- high CPOL and 2 edge CPHA (mode 11)
 *  					- first bit MSB
 *  					- 8-bit data size
 *  					- software set of chip select
 *  				- static function(s) to write/send a single/multiple byte-block
 *  				- static function(s) to check the device id
 *  				- public function(s) to initialize:
 *  					- communication interface
 *  					- data buffers
 *  					- data current buffer pointer
 *  					- data current pointer position in the buffer
 *  				- public function(s) to de-initialize
 *  				- public function(s) to configure the device according to the SCONFIG struct
 *  				- public function(s) to retrieve measurements from the sensor(s)
 */

#include "sensorio.h"

/* Private defines -----------------------------------------------------------*/
#define BMX055_ACC_DATA_READY
#define BMX055_GYRO_DATA_READY
//#define BMX055_MAG_DATA_READY

#define DELAY100MS			1000
#define DELAY500MS			5000

#define RCC_AHBPeriphClockCmd_SPI_BOSCH_CS		RCC_AHB1PeriphClockCmd
#define RCC_AHBPeriph_GPIO_BOSCH_CS   			RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE
#define GPIO_BOSCH_ACC_CS              			GPIOE
#define GPIO_Pin_ACC_CS              			GPIO_Pin_7
#define GPIO_BOSCH_GYRO_CS              		GPIOE
#define GPIO_Pin_GYRO_CS              			GPIO_Pin_8
#define GPIO_BOSCH_MAG_CS              			GPIOE
#define GPIO_Pin_MAG_CS              			GPIO_Pin_9
#define GPIO_BOSCH_BARO_CS              		GPIOE
#define GPIO_Pin_BARO_CS              			GPIO_Pin_10

#define RCC_APBPeriphClockCmd_SPI_BOSCH			RCC_APB2PeriphClockCmd
#define RCC_APBPeriph_SPI_BOSCH					RCC_APB2Periph_SPI1
#define SPI_BOSCH                   			SPI1
#define GPIO_SPI_BOSCH              			GPIOA
#define GPIO_Pin_SPI_BOSCH_SCK      			GPIO_Pin_5
#define GPIO_Pin_SPI_BOSCH_MISO     			GPIO_Pin_6
#define GPIO_Pin_SPI_BOSCH_MOSI     			GPIO_Pin_7

#define GPIO_Pin_SPI_BOSCH_SCK_Src				GPIO_PinSource5
#define GPIO_Pin_SPI_BOSCH_MISO_Src				GPIO_PinSource6
#define GPIO_Pin_SPI_BOSCH_MOSI_Src				GPIO_PinSource7

#define GPIO_AF_SPI_BOSCH						GPIO_AF_SPI1

#define SPI_BaudRatePrescaler_SPI_BOSCH			SPI_BaudRatePrescaler_16

#define ACC_SELECT()        GPIO_ResetBits(GPIO_BOSCH_ACC_CS, GPIO_Pin_ACC_CS)		/* ACC CS = L */
#define ACC_DESELECT()      GPIO_SetBits(GPIO_BOSCH_ACC_CS, GPIO_Pin_ACC_CS)		/* ACC CS = H */
#define GYRO_SELECT()		GPIO_ResetBits(GPIO_BOSCH_GYRO_CS, GPIO_Pin_GYRO_CS)	/* GYRO CS = L */
#define GYRO_DESELECT()		GPIO_SetBits(GPIO_BOSCH_GYRO_CS, GPIO_Pin_GYRO_CS)		/* GYRO CS = H */
#define MAG_SELECT()		GPIO_ResetBits(GPIO_BOSCH_MAG_CS, GPIO_Pin_MAG_CS)		/* MAG CS = L */
#define MAG_DESELECT()		GPIO_SetBits(GPIO_BOSCH_MAG_CS, GPIO_Pin_MAG_CS)		/* MAG CS = H */
#define BARO_SELECT()		GPIO_ResetBits(GPIO_BOSCH_BARO_CS, GPIO_Pin_BARO_CS)	/* BARO CS = L */
#define BARO_DESELECT()		GPIO_SetBits(GPIO_BOSCH_BARO_CS, GPIO_Pin_BARO_CS)

/* Static functions ----------------------------------------------------------*/
static
void sensor_on (void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t dummyread;

	/* Enable GPIO clock for CSs */
	RCC_AHBPeriphClockCmd_SPI_BOSCH_CS(RCC_AHBPeriph_GPIO_BOSCH_CS, ENABLE);
	/* Enable SPI clock, SPI1: APB2, SPI2: APB1 */
	RCC_APBPeriphClockCmd_SPI_BOSCH(RCC_APBPeriph_SPI_BOSCH, ENABLE);

	/* SPI GPIO Configuration --------------------------------------------------*/

	/* Configure SPI Chip Select pin */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	/* SPI Accelerometer CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_ACC_CS;
	GPIO_Init(GPIO_BOSCH_ACC_CS, &GPIO_InitStructure);

	/* SPI Gyroscope CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_GYRO_CS;
	GPIO_Init(GPIO_BOSCH_GYRO_CS, &GPIO_InitStructure);

	/* SPI Magnetometer CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_MAG_CS;
	GPIO_Init(GPIO_BOSCH_MAG_CS, &GPIO_InitStructure);

	/* SPI Barometer CS pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_BARO_CS;
	GPIO_Init(GPIO_BOSCH_BARO_CS, &GPIO_InitStructure);

	/* De-select the sensor: Chip Select high */
	ACC_DESELECT();
	GYRO_DESELECT();
	MAG_DESELECT();
	BARO_DESELECT();

	/* Configure SPI pins: SCK and MOSI with default alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_BOSCH_SCK;
	GPIO_Init(GPIO_SPI_BOSCH, &GPIO_InitStructure);

	/* SPI MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_BOSCH_MOSI;
	GPIO_Init(GPIO_SPI_BOSCH, &GPIO_InitStructure);

	/* Configure MISO as Input with internal pull-up */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_BOSCH_MISO;
	GPIO_Init(GPIO_SPI_BOSCH, &GPIO_InitStructure);

	/* Connect SPI pins to AF5 */
	GPIO_PinAFConfig(GPIO_SPI_BOSCH, GPIO_Pin_SPI_BOSCH_SCK_Src, GPIO_AF_SPI_BOSCH );	// SCK
	GPIO_PinAFConfig(GPIO_SPI_BOSCH, GPIO_Pin_SPI_BOSCH_MOSI_Src, GPIO_AF_SPI_BOSCH );	// MOSI
	GPIO_PinAFConfig(GPIO_SPI_BOSCH, GPIO_Pin_SPI_BOSCH_MISO_Src, GPIO_AF_SPI_BOSCH );	// MISO

	/* SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_SPI_BOSCH; // 72000kHz/16 = 4500kHz < 10000kHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI_BOSCH, &SPI_InitStructure);
	SPI_CalculateCRC(SPI_BOSCH, DISABLE);
	SPI_Cmd(SPI_BOSCH, ENABLE);

	/* drain SPI */
	while (SPI_I2S_GetFlagStatus(SPI_BOSCH, SPI_I2S_FLAG_TXE) == RESET) { ; }
	dummyread = SPI_I2S_ReceiveData(SPI_BOSCH);
	if (dummyread) { ; }
}

static
void sensor_off(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SPI_I2S_DeInit(SPI_BOSCH);
	SPI_Cmd(SPI_BOSCH, DISABLE);
	RCC_APBPeriphClockCmd_SPI_BOSCH(RCC_APBPeriph_SPI_BOSCH, DISABLE);

	/* All SPI-Pins to input with weak internal pull-downs */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_BOSCH_SCK | GPIO_Pin_SPI_BOSCH_MISO | GPIO_Pin_SPI_BOSCH_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIO_SPI_BOSCH, &GPIO_InitStructure);
}

static
uint8_t stm32_sensor_rw( uint8_t out )
{
	/* Loop while DR register in not empty */
	//while (SPI_I2S_GetFlagStatus(SPI_BOSCH, SPI_I2S_FLAG_TXE) == RESET) { ; }

	/* Send byte through the SPI peripheral */
	SPI_I2S_SendData(SPI_BOSCH, out);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI_BOSCH, SPI_I2S_FLAG_RXNE) == RESET) { ; }

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI_BOSCH);
}

#define xmit_sensor(dat)  stm32_sensor_rw(dat)

static
uint8_t rcvr_sensor (void)
{
	return stm32_sensor_rw(0xff);
}

#define rcvr_sensor_m(dst)  *(dst)=stm32_sensor_rw(0xff)

static
uint8_t rcvr_data (
	uint8_t addr,			/* Address of the register to read */
	Sensor_TypeDef sensor	/* Sensor type */
)
{
	switch(sensor){
	case(Accelerometer)	:	ACC_SELECT();		break;
	case(Gyroscope)		:	GYRO_SELECT();		break;
	case(Magnetometer)	:	MAG_SELECT();		break;
	default				:	;
	}

	uint8_t token = 0x80 | addr;	/* Read token */
	xmit_sensor(token);
	token = rcvr_sensor();

	switch(sensor){
	case(Accelerometer)	:	ACC_DESELECT();		break;
	case(Gyroscope)		:	GYRO_DESELECT();	break;
	case(Magnetometer)	:	MAG_DESELECT();		break;
	default				:	;
	}

	return token;
}

static
Sensor_StatusTypeDef rcvr_data_m (
	uint8_t *buff,			/* Data buffer to store received data */
	uint8_t addr,			/* Address of the register to read */
	Sensor_TypeDef sensor,	/* Sensor type */
	uint8_t length			/* Length of Buffer */
)
{
	switch(sensor){
	case(Accelerometer)	:	ACC_SELECT();		break;
	case(Gyroscope)		:	GYRO_SELECT();		break;
	case(Magnetometer)	:	MAG_SELECT();		break;
	default				:	;
	}

	uint8_t token = 0x80 | addr;				/* Read token */
	xmit_sensor(token);
	for(int i = 0; i < length; i++){
		rcvr_sensor_m(buff++);
	}

	switch(sensor){
	case(Accelerometer)	:	ACC_DESELECT();		break;
	case(Gyroscope)		:	GYRO_DESELECT();	break;
	case(Magnetometer)	:	MAG_DESELECT();		break;
	default				:	;
	}

	return SENS_OK;
}

static
Sensor_StatusTypeDef xmit_data_c (
	uint8_t out,			/* Data buffer which contains data to be written */
	uint8_t addr,			/* Address of the register to write */
	Sensor_TypeDef sensor	/* Sensor type */
)
{
	switch(sensor){
	case(Accelerometer)	:	ACC_SELECT();	break;
	case(Gyroscope)		:	GYRO_SELECT();	break;
	case(Magnetometer)	:	MAG_SELECT();	break;
	default				:	;
	}

	uint8_t token = 0x7F & addr;		/* Write token */
	xmit_sensor(token);
	xmit_sensor(out);

	switch(sensor){
	case(Accelerometer)	:	ACC_DESELECT();		break;
	case(Gyroscope)		:	GYRO_DESELECT();	break;
	case(Magnetometer)	:	MAG_DESELECT();		break;
	default				:	;
	}

	bmx055_delay = 0;
	while(bmx055_delay < DELAY100MS);

	uint8_t sample = rcvr_data(addr, sensor);

	if(sample != out)		/* Check if written correctly */
		return SENS_ERROR;

	return SENS_OK;
}

static
Sensor_StatusTypeDef xmit_data (
	uint8_t out,			/* Data buffer which contains data to be written */
	uint8_t addr,			/* Address of the register to write */
	Sensor_TypeDef sensor	/* Sensor type */
)
{
	switch(sensor){
	case(Accelerometer)	:	ACC_SELECT();	break;
	case(Gyroscope)		:	GYRO_SELECT();	break;
	case(Magnetometer)	:	MAG_SELECT();	break;
	default				:	;
	}

	uint8_t token = 0x7F & addr;		/* Write token */
	xmit_sensor(token);
	xmit_sensor(out);

	switch(sensor){
	case(Accelerometer)	:	ACC_DESELECT();		break;
	case(Gyroscope)		:	GYRO_DESELECT();	break;
	case(Magnetometer)	:	MAG_DESELECT();		break;
	default				:	;
	}

	return SENS_OK;
}

static
void release_sensor (void)
{
	ACC_DESELECT();
	GYRO_DESELECT();
	MAG_DESELECT();
	BARO_DESELECT();
	rcvr_sensor();
}

/* Public functions ----------------------------------------------------------*/
void  sensor_initialize (void)
{
	sensor_on();

	sensor_log_start	=	0xff;
	sensor_log_wait 	=	0;

	sensor_crnt_buffer	=	0;
	sensor_crnt_pos		=	0;

	sensor_buffer[0]	=	S_BUFFER0;
	sensor_buffer[1]	=	S_BUFFER1;
	sensor_buffer[2]	=	S_BUFFER2;
	sensor_buffer[3]	=	S_BUFFER3;
	sensor_buffer[4]	=	S_BUFFER4;
	sensor_buffer[5]	=	S_BUFFER5;
	sensor_buffer[6]	=	S_BUFFER6;
	sensor_buffer[7]	=	S_BUFFER7;
	sensor_buffer[8]	=	S_BUFFER8;
	sensor_buffer[9]	=	S_BUFFER9;
	sensor_buffer[10]	=	S_BUFFER10;
	sensor_buffer[11]	=	S_BUFFER11;
	sensor_buffer[12]	=	S_BUFFER12;
	sensor_buffer[13]	=	S_BUFFER13;
	sensor_buffer[14]	=	S_BUFFER14;
}

void sensor_deinitialize (void)
{
	release_sensor();
	sensor_off();
}

Sensor_StatusTypeDef sensor_check_id (void)
{
	uint8_t acc, gyro, mag;

	acc = rcvr_data(BMX055_ACC_CHIP_ID, Accelerometer);
	if(acc != BMX055_ACC_ID)
		return SENS_IDERR;

	gyro = rcvr_data(BMX055_GYRO_CHIP_ID, Gyroscope);
	if(gyro != BMX055_GYRO_ID)
		return SENS_IDERR;

	xmit_data(MAG_PWR_CR1_POWER_CONTROL_BIT, BMX055_MAG_PWR_CR1, Magnetometer);
	bmx055_delay = 0;
	while(bmx055_delay < DELAY100MS);

	mag = rcvr_data(BMX055_MAG_CHIP_ID, Magnetometer);
	if(mag != BMX055_MAG_ID)
		return SENS_IDERR;

	return SENS_OK;
}

Sensor_StatusTypeDef  sensor_configure(
	volatile Sensor_ConfigTypeDef *f
	)
{
	/* Accelerometer measurement range
	 * Default	: 0b(0000)0011 	= +-2g
	 * Set		: user-defined*/
	if(xmit_data_c(f->accel_range, BMX055_ACC_PMU_RANGE, Accelerometer) != SENS_OK)
		return SENS_CONFERR;
	/* Accelerometer measurement output data rate
	 * Default	: 0b(000)01111 	= 1000 Hz
	 * Set		: user-defined*/
	if(xmit_data_c(f->accel_rate, BMX055_ACC_PMU_BW, Accelerometer) != SENS_OK)
		return SENS_CONFERR;
	/* Accelerometer power mode
	 * Default	: 0b0000000(0) 	= normal mode
	 * Set		: 0b0000000(0) 	= normal mode */
	if(xmit_data_c(ACC_PWR_NORMAL, BMX055_ACC_PMU_LPW, Accelerometer) != SENS_OK)
		return SENS_CONFERR;

	#ifdef BMX055_ACC_DATA_READY
	/* Accelerometer interrupt map 1
	 * Default	: 0b000(00)000 	= INT1 and INT2 pin, FIFO water-mark, FIFO full interrupt disabled
	 * Set		: 0b000(00)001 	= map data ready interrupt to INT1 pin enabled*/
	if(xmit_data_c(ACC_INT_MAP_1_DATA_1, BMX055_ACC_INT_MAP_1, Accelerometer) != SENS_OK)
		return SENS_INTCONFERR;
	/* Accelerometer interrupt output configuration
	 * Default	: 0b(0000)0101 	= INT2: push-pull active high, INT1: push-pull active high
	 * Set		: 0b(0000)0101	= default state*/
	if(xmit_data_c(ACC_INT_OUT_CTRL_RESET, BMX055_ACC_INT_OUT_CTRL, Accelerometer) != SENS_OK)
		return SENS_INTCONFERR;
	/* Accelerometer interrupt enable
	 * Default 	: 0b(0)0000000 	= no interrupt enabled
	 * Set		: 0b(0)0010000  = data ready interrupt enabled*/
	if(xmit_data_c(ACC_INT_EN_1_DATA, BMX055_ACC_INT_EN_1, Accelerometer) != SENS_OK)
		return SENS_INTCONFERR;
	#endif

	/* Gyroscope measurement range
	 * Default	: 0b(00000)000 	= +-2000d/s
	 * Set		: user-defined*/
	if(xmit_data_c(f->gyro_range, BMX055_GYRO_RANGE, Gyroscope) != SENS_OK)
		return SENS_CONFERR;
	/* Gyroscope measurement output data rate
	 * Default	: 0b(1000)0000 	= 2000 Hz with no filter
	 * Set		: user-defined*/
	if(xmit_data_c((f->gyro_rate | GYRO_BW_RESET), BMX055_GYRO_BW, Gyroscope) != SENS_OK)
		return SENS_CONFERR;
	/* Gyroscope power mode
	 * Default	: 0b0(0)0(0)000(0)	= normal mode
	 * Set		: 0b0(0)0(0)000(0)	= normal mode*/
	if(xmit_data_c(GYRO_LPM1_NORMAL_MODE, BMX055_GYRO_LPM1, Gyroscope) != SENS_OK)
		return SENS_CONFERR;

	#ifdef BMX055_GYRO_DATA_READY
	/* Gyroscope interrupt map 1
	 * Default	: 0b00000000 	= INT3 and INT4 interrupt pins disabled
	 * Set		: 0b00000001 	= map new data interrupt to INT3 pin enabled*/
	if(xmit_data_c(GYRO_INT_MAP_1_INT1_DATA, BMX055_GYRO_INT_MAP_1, Gyroscope) != SENS_OK)
		return SENS_INTCONFERR;
	/* Gyroscope interrupt output configuration
	 * Default	: 0b(0000)1111 	= INT2: open-drive active high, INT1: open-drive active high
	 * Set		: 0b(0000)1101	= INT2: open-drive active high, INT1: push-pull active high*/
	if(xmit_data_c(((~GYRO_INT_EN_1_INT1_OD) & BMX055_GYRO_INT_EN_1_RESET), BMX055_GYRO_INT_EN_1, Gyroscope) != SENS_OK)
		return SENS_INTCONFERR;
	/* Gyroscope interrupt enable
	 * Default 	: 0b00(0000)00 	= no interrupt enabled
	 * Set		: 0b10(0000)00  = new data interrupt enabled*/
	if(xmit_data_c(GYRO_INT_EN_0_DATA_EN, BMX055_GYRO_INT_EN_0, Gyroscope) != SENS_OK)
		return SENS_INTCONFERR;
	#endif

	/* Magnetometer power mode 1
	 * Default	: 0b0(0000)001	= not in suspend mode
	 * Set		: 0b1(0000)010	= put in suspend mode*/
	xmit_data(MAG_PWR_CR1_SOFT_RESET_0|MAG_PWR_CR1_SOFT_RESET_1 , BMX055_MAG_PWR_CR1, Magnetometer);  // Soft-reset MAG, ends up in sleep mode
	bmx055_delay = 0;	while(bmx055_delay < DELAY500MS);

	/* Magnetometer power mode 1
	 * Current	: 0b1(0000)010	= in suspend mode
	 * Set		: 0b0(0000)001	= up from suspend mode to sleep mode */
	xmit_data(MAG_PWR_CR1_POWER_CONTROL_BIT, BMX055_MAG_PWR_CR1, Magnetometer);
	bmx055_delay = 0;	while(bmx055_delay < DELAY500MS);

	/* Magnetometer power mode 2
	 * Default	: 0b00000110	= in sleep mode
	 * Set		: 0b00000000	= in normal mode */
	xmit_data(MAG_PWR_CR2_NORMAL_MODE, BMX055_MAG_PWR_CR2, Magnetometer);
	bmx055_delay = 0;	while(bmx055_delay < DELAY500MS);

	/* Magnetometer power mode 2
	 * Current	: 0b00000000	= in normal mode
	 * Set		: user-defined output data rate */
	if(xmit_data_c(f->mag_rate, BMX055_MAG_PWR_CR2, Magnetometer) != SENS_OK)
		return SENS_CONFERR;

	/* Magnetometer number of repetition XY axes
	 * Current	: 0b00000000	= 0
	 * Set		: user-defined number of repetition */
	if(xmit_data_c(f->mag_range_xy, BMX055_MAG_REP_XY, Magnetometer) != SENS_OK)
		return SENS_CONFERR;

	/* Magnetometer number of repetition Z axis
	 * Current	: 0b00000000	= 0
	 * Set		: user-defined number of repetition */
	if(xmit_data_c(f->mag_range_z, BMX055_MAG_REP_Z, Magnetometer) != SENS_OK)
		return SENS_CONFERR;

	// TODO: fix magnetometer data ready line
	/* Magnetometer interrupt configuration
	 * Default	: 0b00000111	= Data ready pin active high, latched active high interrupt
	 * Set		: 0b10000111	= Data ready on DRDY pin enabled with default settings */
	#ifdef BMX055_MAG_DATA_READY
	if(xmit_data_c(MAG_INT_CR2_DATA_READY_PIN_EN | MAG_INT_CR2_RESET, BMX055_MAG_INT_CR_2, Magnetometer) != SENS_OK)
		return SENS_INTCONFERR;
	bmx055_delay = 0;	while(bmx055_delay < DELAY100MS);
	#endif

	return SENS_OK;
}

Sensor_StatusTypeDef accel_read (
	uint8_t *buff,
	uint8_t data_length)
{
	if(rcvr_data_m(buff, BMX055_ACC_D_X_LSB, Accelerometer, data_length) != SENS_OK)
		return SENS_ERROR;
	return SENS_OK;
}

Sensor_StatusTypeDef gyro_read (
	uint8_t *buff,
	uint8_t data_length)
{
	if(rcvr_data_m(buff, BMX055_GYRO_RATE_X_LSB, Gyroscope, data_length) != SENS_OK)
		return SENS_ERROR;
	return SENS_OK;
}

Sensor_StatusTypeDef mag_read (
	uint8_t *buff,
	uint8_t data_length)
{
	if(rcvr_data_m(buff, BMX055_MAG_XOUT_LSB, Magnetometer, data_length) != SENS_OK)
		return SENS_ERROR;
	return SENS_OK;
}

/* Interrupt Service Routine (ISR) -------------------------------------------*/

