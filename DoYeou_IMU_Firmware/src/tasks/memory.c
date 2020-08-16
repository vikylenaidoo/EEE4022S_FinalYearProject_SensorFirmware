/*
 * memory.c
 *
 *  Created on: Feb 26, 2018
 *      Author: Do Yeou Ku
 */


#include "memory.h"

static
void timer_initialize (void){
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// TIM2 initialization overflow every 10 ms
	// TIM2 by default has clock of 84MHz
	// Here, we must set value of prescaler and period,
	// so update event is 100 Hz or 10 ms
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1)
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_Prescaler = Timer_TIM2_PSC;
	TIM_InitStructure.TIM_Period = Timer_TIM2_ARR;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// TIM2 initialize
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	// Enable TIM2 interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// Start TIM2
	TIM_Cmd(TIM2, ENABLE);

	// Nested vectored interrupt settings
	// TIM2 interrupt is most important (PreemptionPriority and SubPriority = 0)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler (void)
{
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
    	disk_timerproc();
        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

SD_FlagStatus check_memory (
	FATFS *fs,
	FIL *fp,
	FRESULT err
	) {

	timer_initialize();
	/* TODO : create equivalent */
	//xLoggingTaskQueue = xQueueCreate(6, sizeof(CommsTask_TransmitPacketStruct));
	//xLoggingTaskFilenameQueue = xQueueCreate (2, 12); // Filename size is 23 bytes
	SD_FlagStatus memory_status = 0;//
	memory_status &= ~LOGGING_ON; // Logging is off at startup

	f_mount(0,fs); //never fails

	// Check if the memory is okay
	err = f_open(fp, (char*)"testfile.dat", FA_CREATE_ALWAYS | FA_READ);
	if (err == FR_OK) {
		memory_status |= SD_CARD_OK; // SD card OK
		f_close(fp);
	} else {
		memory_status &= ~SD_CARD_OK; // SD card error
	}

	return memory_status;
}


SD_FlagStatus write_to_memory	(
	FATFS *fs,
	FIL *fp,
	FRESULT err,
	const TCHAR *path,
	UINT *bw,
	const TCHAR *data,
	UINT data_length,
	File_ConfigTypeDef *f
	){

	SD_FlagStatus memory_status = 0;//

	// If logging is enabled, try to open a file and write to a file
	if (f->log_enabled == 1) {
		// If a file is currently not open, open it
		if (f->file_opened == 0) {	/* TODO: file open variable to main.c */
			/* TODO: write a code to check the filename */
			// Check that the filename is valid
			if (f->filename_ok == FAIL) {
				memory_status &= ~SD_CARD_OK;
				memory_status &= ~LOGGING_ON;
			} // A filename is not valid.
			else {
				if ((err = f_open(fp, (char*) path, FA_CREATE_ALWAYS | FA_WRITE))) {
					memory_status &= ~SD_CARD_OK;
					memory_status &= ~LOGGING_ON;
					f->file_opened = 0;
				} // A file cannot be open.
				else {
					err = f_lseek(fp, fp->fsize);
					f->file_opened = 1;
					memory_status |= SD_CARD_OK;
					memory_status |= LOGGING_ON;
				} // A file is open and a pointer to the file object is found.
			}
		}
		// If a file is currently open, write to a file
		if (f->file_opened == 1) {
			/* TODO: write a access to buffer + extract length */
			//Block on queue. Copy from queue to buffer
			//xQueueReceive(xLoggingTaskQueue, packet_to_write, portMAX_DELAY);

			//Extract length. This is LITTLE ENDIAN, so the MSB is the last byte of the buffer, and the LSB is the 2nd last
			//length_of_data_to_tx = packet_to_write[COMMS_TX_BUFFER_SIZE - 1] << 8 | packet_to_write[COMMS_TX_BUFFER_SIZE - 2];
			// If a file is open, proceed to write
			if (err == FR_OK) {
				err = f_write(fp, data, data_length, bw);
				memory_status |= SD_CARD_OK;
				memory_status |= LOGGING_ON;
			} // A entry written successfully.
			else {
				memory_status &= ~SD_CARD_OK;
				memory_status &= ~LOGGING_ON;
			} // A entry failed to be written successfully.
		}
	}
	// If logging is not enabled
	else {
		memory_status &= ~LOGGING_ON;
		// If logging is not enabled but a file is open
		if (f->file_opened == 1) {
			// TODO: Code to empty the buffer
			f_close(fp);
			f->file_opened = 0;
		} // A file is closed and buffer is reset
	}

	return memory_status;
}
