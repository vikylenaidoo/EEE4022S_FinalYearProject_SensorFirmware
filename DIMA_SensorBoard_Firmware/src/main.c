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

	uart_initialise();

	exti_initialise();

	//GlobalData.start_token = '$';

	while (1){

		//Sensor_StatusTypeDef sensor_status = sensor_read_all();
		//GNSS_StatusTypeDef gnss_status = gnss_read_new_data();


		/*
		UART_StatusTypeDef uart_status = uart_send_to_jetson();

		for(int i=0; i<10000; i++){
			for(int i=0; i<1000; i++){
				continue;
			}
		}
		 */

	}


}
