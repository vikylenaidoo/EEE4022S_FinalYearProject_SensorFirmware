/*
 * gnss.c
 *
 *  Created on: 26 Aug 2020
 *      Author: vikylenaidoo
 */

//----------------INCLUDES---------------------------///
#include "gnss.h"



//----------------PUBLIC FUNCTIONS--------------------//
void gnss_read_new_data(){
	for(int i=0; i< GNSS_DATA_LENGTH; i++){
		uint8_t temp = GNSS_RX_BUFFER[i];
		GNSS_RX_BUFFER[i] = 0;
	}

}
