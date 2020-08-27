/*
******************************************************************************
File:		main.c
Author:		Vikyle Naidoo

******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "main.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void){
	spi_initialise();

	Sensor_StatusTypeDef status;
	//uint8_t chipid_gyro = spi_read_single(SPI_CS_Acc, BMX055_ACC_CHIP_ID|ACC_CHIP_ID_CHIP_ID);
	do{
		 status = sensor_check_id();
	}while (status != SENS_OK);

	status = sensor_config();

	//uart_initialise();

	/*
	uint16_t log_buffer_x[20];
	uint16_t log_buffer_y[20];
	uint16_t log_buffer_z[20];

	for(int i=0; i<20; i++){
		log_buffer_x[i] = 0;
		log_buffer_y[i] = 0;
		log_buffer_z[i] = 0;
	}

	union acc_log {
		uint16_t out;
		uint8_t in[2];
	};


	//uint8_t baro_buffer [6];
	//uint8_t acc_buffer [6];
	//uint8_t gyro_buffer [6];
	//uint8_t mag_buffer [8];
	*/

	uint8_t data = 0;
	while (1){

		//sensor_read_baro(baro_buffer, 6);
		//sensor_read_acc(acc_buffer, 6);
		//sensor_read_gyro(gyro_buffer, 6);
		//sensor_read_mag(mag_buffer, 8);

		/*union acc_log data;
		data.in[0]=acc_buffer[0];
		data.in[1]=acc_buffer[1];
		log_buffer_x[index] = (uint16_t)data.out;
		data.in[0]=acc_buffer[2];
		data.in[1]=acc_buffer[3];
		log_buffer_y[index] = (uint16_t)data.out;
		data.in[0]=acc_buffer[4];
		data.in[1]=acc_buffer[5];
		log_buffer_z[index] = (uint16_t)data.out;
		index++;
		*/

		/*if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
			data = uart_receive(UART_DEVICE_GNSS);
		 */

		//gnss_read_new_data();
		sensor_read_all();

		/*for(int i=0; i<10000; i++){
			for(int i=0; i<1000; i++){
				continue;
			}
		}*/

	}


}
