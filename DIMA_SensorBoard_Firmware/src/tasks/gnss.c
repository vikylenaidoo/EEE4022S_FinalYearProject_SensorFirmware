/*
 * gnss.c
 *
 *  Created on: 26 Aug 2020
 *      Author: vikylenaidoo
 */

//----------------INCLUDES---------------------------///
#include "gnss.h"
#include "stdbool.h"



//--------------------STATIC FUNCTIONS-------------------//
static bool gnss_ubx_checksum(){
	uint8_t CK_A = 0;
	uint8_t CK_B = 0;

	uint8_t ck_a = GNSS_RX_BUFFER[98];
	uint8_t ck_b = GNSS_RX_BUFFER[99];

	for(int i=2; i<GNSS_BUFFER_SIZE-2; i++){
		CK_A = CK_A + GNSS_RX_BUFFER[i];
		CK_B = CK_B + CK_A;
	}

	if(CK_A != ck_a)
		return false;

	if(CK_B != ck_b)
		return false;

	return true;

}




//----------------PUBLIC FUNCTIONS--------------------//
GNSS_StatusTypeDef gnss_read_new_data(){
	if(!gnss_ubx_checksum())
		return GNSS_READ_ERROR;


	//copy full ubx message to global data struct
	for(uint8_t i=0; i<GNSS_BUFFER_SIZE; i++){
		GlobalDataUnion.GlobalDataStruct.GNSS_UBX_MESSAGE[i] = GNSS_RX_BUFFER[i];
	}


	/*
	//copy essential UBX data from rx buffer to global struct
	int index = 0;
	for(uint8_t i=0; i<GNSS_BUFFER_SIZE; i++){
		if( (i>=20&&i<=21) || (i>=23&&i<=39) || (i>=48&&i<=67) || (i==78)){
			GlobalDataUnion.GlobalDataStruct.GNSS_UBX_MESSAGE[index] = GNSS_RX_BUFFER[i];
			index++;
		}

	}
	*/

	return GNSS_OK;

}



