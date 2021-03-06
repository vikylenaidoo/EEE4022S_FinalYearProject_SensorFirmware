/*
 * gnss.h
 *
 *  Created on: 15 Aug 2020
 *      Author: vikylenaidoo
 */

#ifndef TASKS_GNSS_H_
#define TASKS_GNSS_H_

//-----------------------INCLUDES----------------------------------------//
#include "uart.h"
#include "var.h"

//---------------------TASKS INCLUDES-----------------------------------//

//---------------------PUBLIC DEFINES---------------------------------//

#ifdef GNSS_NMEA
//gps data struct
struct GNSS_DataStruct{
	char time		[9];
	char status;
	char latitude	[10];
	char NS;
	char longitude	[10];
	char EW;
	char speed		[5];
	char course		[5];
};
#else

struct GNSS_UBX_Data_Struct{
	//only keep important variables to copy from ubx message


	//info flags

	/*validity flag
	 * bit3		validMessage
	 * bit2		fullyResolved
	 * bit1		validTime
	 * bit0		ValidDate */
	uint8_t 	valid;

	/*GNSS fix type
	 * 0	no fix
	 * 1	dead reckon only
	 * 2 	2d fix
	 * 3	3d fix
	 * 4	GNSS + dead reckon
	 * 5	time fix only*/
	uint8_t 	fixType;

	/*flags
	 * bit0		gnssFixOK	1=valid
	 * bit1		diffSoln	1=diff correction applied
	 * bit4:2	psmState	power save mode state
	 * 						0=aquisistion
	 * 						1=tracking
	 *						2=power optimized tracking
	 *						3=inactive
	 * bit5		headerVehValid	1 = heading of vehicle is valid, only set if the receiver is in sensor fusion mode
	 * bit7:6	carrSoln	Carrier phase range solution status:
	 * 						0: no carrier phase range solution
	 * 						1: carrier phase range solution with floating ambiguities
	 * 						2: carrier phase range solution with fixed ambiguities
	 */
	uint8_t 	flags;

	/*flags3
	 * bit0		invalid lon/lat/height/hMSL*/
	uint8_t 	flags3;


	uint8_t 	numSV;	//number of sattelites
	int32_t 	lon;	//longitude						deg
	int32_t 	lat;	//latitude						deg

	int32_t 	height;	//height above ellipsoid		mm
	int32_t 	hMSL;	//height above mean sea lvl		mm
	uint32_t 	hAcc;	//horizantal accuracy			mm
	uint32_t 	vAcc;	//vertical accuracy				mm

	int32_t 	velN;	//north velocity				mm/s
	int32_t 	velE;	//east velocity					mm/s
	int32_t 	velD;	//down velocity					mm/s

	int32_t 	gSpeed;	//ground speed 2d				mm/s
	int32_t 	headMot;//heading of motion 2d			deg
	uint32_t 	sAcc;	//speed accuracy				mm/s
	uint32_t 	headAcc;//heading accuracy				deg


};
struct GNSS_UBX_Data_Struct GNSS_UBX_Data_Global;

#endif
//-------------------PUBLIC VARIABLES----------------------------------//

//--------------------FUNCTION HEADERS----------------------------------//
/*read new data from the gnss_log buffer and out it in the struct*/
GNSS_StatusTypeDef gnss_read_new_data();



#endif /* TASKS_GNSS_H_ */
