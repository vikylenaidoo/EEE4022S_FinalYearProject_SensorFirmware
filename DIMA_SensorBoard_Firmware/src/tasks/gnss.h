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

//---------------------TASKS INCLUDES-----------------------------------//

//---------------------PUBLIC DEFINES---------------------------------//
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


//-------------------PUBLIC VARIABLES----------------------------------//

//--------------------FUNCTION HEADERS----------------------------------//
/*read new data from the gnss_log buffer and out it in the struct*/
void gnss_read_new_data();



#endif /* TASKS_GNSS_H_ */
