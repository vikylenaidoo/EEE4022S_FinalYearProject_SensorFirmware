/*
 * uart.c
 *
 *  Created on: 	15 Aug 2020
 *      Author: 	vikylenaidoo
 *     Details:		low level UART driver
 */

//-----------------------------INLCUDES---------------------------//
#include "uart.h"

//------------------------------DEFINES--------------------------//

/*----------------------------GNSS UART-------------------------*/
#define RCC_APBPeriph_USART_GNSS	   			RCC_APB2Periph_USART1
#define RCC_AHBPeriph_USART_GNSS	   			RCC_AHB1Periph_GPIOB
#define USART_GNSS								USART1
#define USART_BaudRate_USART_GNSS				115200

#define GPIO_USART_GNSS							GPIOB
#define GPIO_Pin_USART_GNSS_TX					GPIO_Pin_6
#define GPIO_Pin_USART_GNSS_RX					GPIO_Pin_7

#define GPIO_Pin_USART_GNSS_TX_Src				GPIO_PinSource6
#define GPIO_Pin_USART_GNSS_RX_Src				GPIO_PinSource7

#define GPIO_AF_USART_GNSS						GPIO_AF_USART1

#define USART_GNSS_IRQn							USART1_IRQn
#define USART_GNSS_IRQHandler					USART1_IRQHandler

//DMA GNSS RX TO MEM
#define RCC_AHBPeriph_GNSS_DMA					RCC_AHB1Periph_DMA2
#define DMA_Channel_USART_GNSS_RX				DMA_Channel_4
#define DMA_Channel_USART_GNSS_TX				DMA_Channel_4
#define DMA_Stream_USART_GNSS_RX				DMA2_Stream2
#define DMA_Stream_USART_GNSS_TX				DMA2_Stream7

#define DMA_GNSS_RX_IRQn						DMA2_Stream2_IRQn
#define DMA_GNSS_TX_IRQn						DMA2_Stream7_IRQn
#define DMA_GNSS_RX_IRQHandler					DMA2_Stream2_IRQHandler
#define DMA_GNSS_TX_IRQHandler					DMA2_Stream7_IRQHandler

#define DMA_IT_TC_GNSS_RX						DMA_IT_TCIF2
#define DMA_FLAG_GNSS_RX_TC						DMA_FLAG_TCIF2

// DMA MEM TO MEM
#define DMA_Channel_USART_MEM					DMA_Channel_0
#define DMA_Stream_USART_MEM					DMA2_Stream0

#define DMA_USART_MEM_IT						DMA_IT_TC
#define DMA_Stream_USART_MEM_IT					DMA_IT_TCIF0
#define DMA_FLAG_USART_MEM_TC					DMA_FLAG_TCIF0

#define DMA_USART_MEM_IRQn						DMA2_Stream0_IRQn
#define DMA_USART_MEM_IRQHandler				DMA2_Stream0_IRQHandler


/*---------------------JETSON UART-----------------------------------------*/
#define RCC_APBPeriph_USART_JETSON	   			RCC_APB1Periph_USART3
#define RCC_AHBPeriph_USART_JETSON   			RCC_AHB1Periph_GPIOD
#define USART_JETSON							USART3
#define USART_BaudRate_JETSON					115200

#define GPIO_USART_JETSON						GPIOD
#define GPIO_Pin_USART_JETSON_TX				GPIO_Pin_8
#define GPIO_Pin_USART_JETSON_RX				GPIO_Pin_9
#define GPIO_Pin_USART_JETSON_TX_Src			GPIO_PinSource8
#define GPIO_Pin_USART_JETSON_RX_Src			GPIO_PinSource9

#define GPIO_AF_USART_JETSON					GPIO_AF_USART3

#define USART_JETSON_IRQn						USART3_IRQn
#define USART_JETSON_IRQHandler					USART3_IRQHandler

//DMA JETSON MEM TO TX
#define RCC_AHBPeriph_JETSON_DMA				RCC_AHB1Periph_DMA1
#define DMA_Channel_USART_JETSON_RX				DMA_Channel_4
#define DMA_Channel_USART_JETSON_TX				DMA_Channel_4
#define DMA_Stream_USART_JETSON_RX				DMA1_Stream1
#define DMA_Stream_USART_JETSON_TX				DMA1_Stream3

#define DMA_JETSON_RX_IRQn						DMA1_Stream1_IRQn
#define DMA_JETSON_TX_IRQn						DMA1_Stream3_IRQn
#define DMA_JETSON_RX_IRQHandler				DMA1_Stream1_IRQHandler
#define DMA_JETSON_TX_IRQHandler				DMA1_Stream3_IRQHandler

#define DMA_IT_TC_JETSON_TX						DMA_IT_TCIF3
#define DMA_FLAG_TC_JETSON_TX					DMA_FLAG_TCIF3


//------------------------STATIC FUNCTIONS-------------------//
static void reset_gnss_rx_buffer(){
	for(int i=0; i<GNSS_BUFFER_SIZE; i++){
		GNSS_RX_BUFFER[i] = 0;
	}
}


/* Initialise DMA for sending to JETSON
 * source: 		GlobalDataUSART_JESTON (USART3)
 * destination: USART_JESTON (USART3)
 * mode			memory to peripheral
 */
static void initialise_dma_jetson(void){
	//todo: for transmitting data packet from memory to uart_jetson
	//DMA CONFIG
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DMACmd(USART_JETSON, USART_DMAReq_Tx, ENABLE);	//enable dma interface for this uart
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph_JETSON_DMA,ENABLE);

	// De-initialize DMA RX & TX Stream
	DMA_DeInit(DMA_Stream_USART_JETSON_RX);
	while (DMA_GetCmdStatus(DMA_Stream_USART_JETSON_RX ) != DISABLE) { ; }
	DMA_DeInit(DMA_Stream_USART_JETSON_TX);
	while (DMA_GetCmdStatus(DMA_Stream_USART_JETSON_TX ) != DISABLE) { ; }

	//---------------------------------Memory to Peripherral Mode---------------------//
	//shared DMA configuration values:
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_JETSON->DR));	//data destination
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)GlobalDataUnion.GlobalDataArray;	//data source
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	DMA_InitStructure.DMA_Channel = DMA_Channel_USART_JETSON_TX;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_BufferSize = GLOBAL_DATA_BUFFER_SIZE;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;

	//initialise dma
	DMA_Init(DMA_Stream_USART_JETSON_TX, &DMA_InitStructure);

	// enable the TE interrupt in the NVIC
	NVIC_InitStructure.NVIC_IRQChannel = DMA_JETSON_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_TX_DMA_TE_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_TX_DMA_TE_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA_Stream_USART_JETSON_TX, DMA_IT_TE, ENABLE);

	//enable the DMA TC interrupt
	/*NVIC_InitStructure.NVIC_IRQChannel = DMA_JETSON_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_RX_DMA_TC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_RX_DMA_TC_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA_Stream_USART_JETSON_TX, DMA_IT_TC, ENABLE);
	 */

	//------------------ START DMA -----------------------------//
	DMA_Cmd(DMA_Stream_USART_JETSON_TX, ENABLE);
	while (DMA_GetCmdStatus(DMA_Stream_USART_JETSON_TX) != ENABLE) { ; }


}


/* Initialise DMA for receiving from GNSS
 * source: 		USART_GNSS (USART1)
 * destination: GNSS_RX_BUFFER
 * mode: 		Peripheral to memmory
 */
static void initialise_dma_gnss(void){
	//DMA CONFIG
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DMACmd(USART_GNSS, USART_DMAReq_Rx, ENABLE);	//enable dma interface for this uart
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph_GNSS_DMA,ENABLE);

	// De-initialize DMA RX & TX Stream
	DMA_DeInit(DMA_Stream_USART_GNSS_RX);
	while (DMA_GetCmdStatus(DMA_Stream_USART_GNSS_RX ) != DISABLE) { ; }
	DMA_DeInit(DMA_Stream_USART_GNSS_TX);
	while (DMA_GetCmdStatus(DMA_Stream_USART_GNSS_TX ) != DISABLE) { ; }

	//---------------------------------Perpiheral to Memory Mode---------------------//
	//shared DMA configuration values:
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_GNSS->DR));	//data source
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)GNSS_RX_BUFFER;	//data destination
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	DMA_InitStructure.DMA_Channel = DMA_Channel_USART_GNSS_RX;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_BufferSize = GNSS_BUFFER_SIZE;

	#ifdef GNSS_NMEA
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	#else
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	#endif

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;

	DMA_Init(DMA_Stream_USART_GNSS_RX, &DMA_InitStructure);


	// enable the TE interrupt in the NVIC
	NVIC_InitStructure.NVIC_IRQChannel = DMA_GNSS_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_RX_DMA_TE_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_RX_DMA_TE_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA_Stream_USART_GNSS_RX, DMA_IT_TE, ENABLE);

#ifndef GNSS_NMEA
	//enable the DMA TC interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA_GNSS_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_RX_DMA_TC_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_RX_DMA_TC_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA_Stream_USART_GNSS_RX, DMA_IT_TC, ENABLE);

#endif

	//------------------ START DMA -----------------------------//

	DMA_Cmd(DMA_Stream_USART_GNSS_RX, ENABLE);
	while (DMA_GetCmdStatus(DMA_Stream_USART_GNSS_RX ) != ENABLE) { ; }
}

static void initialise_uart_jetson(){
	//@todo: decide which uart line for jetson
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable USART_JETSON clock */
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_USART_JETSON, ENABLE);
	/* Enable GPIO clock for USART_JETSON pins */
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph_USART_JETSON, ENABLE);

	/*----------USART3 GPIO Configuration -------------------------------*/

	/* Configure USART3 pins: TX & RX */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	// USART TX pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART_JETSON_TX;	//pin 8
	GPIO_Init(GPIO_USART_JETSON, &GPIO_InitStructure);

	// USART RX pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART_JETSON_RX; 	//pin 9
	GPIO_Init(GPIO_USART_JETSON, &GPIO_InitStructure);

	// Connect USART pins to AF
	GPIO_PinAFConfig(GPIO_USART_JETSON, GPIO_Pin_USART_JETSON_TX_Src, GPIO_AF_USART_JETSON); //
	GPIO_PinAFConfig(GPIO_USART_JETSON, GPIO_Pin_USART_JETSON_RX_Src, GPIO_AF_USART_JETSON);

	/* -------------USART3 configuration-------------------- */
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_BaudRate = USART_BaudRate_JETSON;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART_JETSON, &USART_InitStructure);

	//Enable UART 1
	USART_Cmd(USART_JETSON, ENABLE);

	initialise_dma_jetson();

}

static void initialise_uart_gnss(){
	reset_gnss_rx_buffer();

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APBPeriph_USART_GNSS, ENABLE);
	/* Enable GPIO clock for USART pins */
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph_USART_GNSS, ENABLE);

	/*----------USART 1 GPIO Configuration -------------------------------*/

	/* Configure USART pins: TX & RX */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	// USART TX pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART_GNSS_TX;	//pin 6
	GPIO_Init(GPIO_USART_GNSS, &GPIO_InitStructure);

	// USART RX pin configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_USART_GNSS_RX; 	//pin7
	GPIO_Init(GPIO_USART_GNSS, &GPIO_InitStructure);

	// Connect USART pins to AF
	GPIO_PinAFConfig(GPIO_USART_GNSS, GPIO_Pin_USART_GNSS_TX_Src, GPIO_AF_USART_GNSS); //
	GPIO_PinAFConfig(GPIO_USART_GNSS, GPIO_Pin_USART_GNSS_RX_Src, GPIO_AF_USART_GNSS);

	/* -------------USART 1 configuration-------------------- */
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_BaudRate = USART_BaudRate_USART_GNSS;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART_GNSS, &USART_InitStructure);

	/*
	//------------- USART 1 RXNE interrupt configuration ----------------//
	NVIC_InitStructure.NVIC_IRQChannel = USART_GNSS_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_RXNE_Priority; //USART_IDLE_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_RXNE_Sub_Priority; //USART_IDLE_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART_GNSS, USART_IT_RXNE, ENABLE);
	*/

#ifdef GNSS_NMEA
	//------------- USART 1 IDLE interrupt configuration ----------------//
	NVIC_InitStructure.NVIC_IRQChannel = USART_GNSS_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_IDLE_Priority; //USART_IDLE_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_IDLE_Sub_Priority; //USART_IDLE_Sub_Priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART_GNSS, USART_IT_IDLE, ENABLE);

#endif

	//Enable UART 1
	USART_Cmd(USART_GNSS, ENABLE);

	initialise_dma_gnss();
}

static void initialise_crc(){
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
	CRC_ResetDR();
}

//--------------------------PUBLIC FUNCTIONS---------------------------------//

void uart_initialise(){
	initialise_uart_gnss();
	initialise_uart_jetson();
	initialise_crc();
}



/*send globa data packet to jetson over uart*/
UART_StatusTypeDef uart_send_to_jetson(){
	//GlobalData.start_token = '$';

	//populate global data union
	GlobalDataUnion.GlobalDataStruct.start_token1 = '$';
	GlobalDataUnion.GlobalDataStruct.start_token2 = '$';
	if(sensor_read_all() != SENS_OK) //add data from sensors
		return UART_SEND_ERROR;

	if(gnss_read_new_data() != GNSS_OK) ///add data from gnss
		return UART_SEND_ERROR;

	//calculate checksum
	uint8_t length = GLOBAL_DATA_BUFFER_SIZE - sizeof(uint32_t);
	uint32_t crc = CRC_calculate_8bit_datablock(GlobalDataUnion.GlobalDataArray, length);
	GlobalDataUnion.GlobalDataStruct.CRC32_CHECKSUM = crc;

	//send data

	uint16_t counter = DMA_GetCurrDataCounter(DMA_Stream_USART_JETSON_TX);
	FlagStatus flag = DMA_GetFlagStatus(DMA_Stream_USART_JETSON_TX, DMA_FLAG_TC_JETSON_TX);
	FunctionalState status = DMA_GetCmdStatus(DMA_Stream_USART_JETSON_TX);

	if(DMA_GetFlagStatus(DMA_Stream_USART_JETSON_TX, DMA_FLAG_TC_JETSON_TX)){

		//disable the dma
		DMA_Cmd(DMA_Stream_USART_JETSON_TX, DISABLE);
		while(DMA_GetCmdStatus(DMA_Stream_USART_JETSON_TX) != DISABLE){;}

		//clear dma tc flag
		DMA_ClearFlag(DMA_Stream_USART_JETSON_TX, DMA_FLAG_TC_JETSON_TX);
		//DMA_ClearITPendingBit(DMA_Stream_USART_JETSON_TX, DMA_IT_TC_JETSON_TX);

		//reset the dma & enable
		DMA_SetCurrDataCounter(DMA_Stream_USART_JETSON_TX, GLOBAL_DATA_BUFFER_SIZE);
		DMA_Cmd(DMA_Stream_USART_JETSON_TX, ENABLE);
		while(DMA_GetCmdStatus(DMA_Stream_USART_JETSON_TX) != ENABLE){;}


	}
	else{
		return UART_SEND_ERROR;
	}

	return UART_OK;

}

/*send data to selected device*/
void uart_send(uint8_t data_out, UART_DeviceSelectTypeDef device){
	//todo
}


/*receive data from the selected uart device*/
uint8_t uart_receive(UART_DeviceSelectTypeDef device){
	uint8_t data;
	switch (device) {
		case UART_DEVICE_GNSS:
			//while(USART_GetFlagStatus(USART_GNSS, USART_FLAG_RXNE) != SET){ ; } //wait for rx buffer to fill
			//read rx buffer
			data = (uint8_t)USART_ReceiveData(USART_GNSS); //@TOD0: 16 bit or 8 bit?
			break;
		case UART_DEVICE_JETSON:
			//code here
			break;
		default:
			data=0xff;
			break;
	}

	return data;
}

uint32_t CRC_calculate_8bit_datablock(uint8_t data[], uint8_t length){
	CRC_ResetDR();
	uint8_t index = 0;

	for(index = 0; index < length; index++){
		CRC->DR = data[index];
	}
	return (CRC->DR);
}

//---------------------------------------INTERRUPT HANDLERS-----------------------------------///

void DMA_JETSON_TX_IRQHandler(void){

	/*transfer complete*/
	if(DMA_GetITStatus(DMA_Stream_USART_JETSON_TX, DMA_IT_TC_JETSON_TX)){
		DMA_ClearITPendingBit(DMA_Stream_USART_JETSON_TX, DMA_IT_TC_JETSON_TX);
	}

	/*transfer error*/
	if(DMA_GetITStatus(DMA_Stream_USART_JETSON_TX, DMA_IT_TE)){
		DMA_ClearITPendingBit(DMA_Stream_USART_JETSON_TX, DMA_IT_TE);
	}

}


//DMA UART TO MEM INTERRUPTS
void DMA_GNSS_RX_IRQHandler(void){

#ifndef GNSS_NMEA
	/*transfer complete*/
	if(DMA_GetITStatus(DMA_Stream_USART_GNSS_RX, DMA_IT_TC_GNSS_RX)){

		//reset DMA stream
		DMA_Cmd(DMA_Stream_USART_GNSS_RX, ENABLE);
		while(DMA_GetCmdStatus(DMA_Stream_USART_GNSS_RX) != ENABLE){;}

		/* Clear DMA Stream Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA_Stream_USART_GNSS_RX, DMA_IT_TC_GNSS_RX);
	}
#endif
	/*transfer error*/
	if(DMA_GetITStatus(DMA_Stream_USART_GNSS_RX, DMA_IT_TE)){
		//int a = 1;
	}


}

/*
//DMA MEM OT MEM INTERRUPTS
void DMA_USART_MEM_IRQHandler (void){
	//Test on DMA Stream Transfer Complete interrupt
	if (DMA_GetFlagStatus(DMA_Stream_USART_MEM, DMA_FLAG_USART_MEM_TC) != RESET){
		// Clear DMA Stream Transfer Complete interrupt pending bit
		DMA_ClearITPendingBit(DMA_Stream_USART_MEM, DMA_Stream_USART_MEM_IT);

		// Enable DMA transfer from uart rx
		DMA_Cmd(DMA_Stream_USART_GNSS_RX, ENABLE);
		while (DMA_GetCmdStatus(DMA_Stream_USART_GNSS_RX ) != ENABLE) { ; }
	}
}
*/

//UART INTERRUPTS
#ifdef GNSS_NMEA
void USART_GNSS_IRQHandler(){
	/*if(USART_GetFlagStatus(USART_GNSS, USART_FLAG_RXNE)){
	//if(USART_GetITStatus(USART_GNSS, USART_IT_RXNE)){
		uint8_t temp = uart_receive(UART_DEVICE_GNSS);
		if(temp == '$')
			index = 0;

		GNSS_RX_BUFFER[index] = temp;
		index++;
		//USART_ClearITPendingBit(USART_GNSS, USART_IT_RXNE);
		//USART_ClearFlag(USART_GNSS, USART_FLAG_RXNE);
		return;
	}*/
	if(USART_GetITStatus(USART_GNSS, USART_IT_IDLE)){
	//if(USART_GetFlagStatus(USART_GNSS, USART_FLAG_IDLE)){

		// Disable DMA RX Stream
		DMA_Cmd(DMA_Stream_USART_GNSS_RX, DISABLE);
		while (DMA_GetCmdStatus(DMA_Stream_USART_GNSS_RX ) != DISABLE) { ; }

		//clear DMA Flag
		DMA_ClearFlag(DMA_Stream_USART_GNSS_RX,DMA_FLAG_GNSS_RX_TC);
		USART_ClearFlag(USART_GNSS, USART_FLAG_RXNE);

		//find the lenght of data
		volatile uint32_t temp = DMA_GetCurrDataCounter(DMA_Stream_USART_GNSS_RX);
		GNSS_DATA_LENGTH = GNSS_BUFFER_SIZE - temp;

		//reset the data counter
		DMA_SetCurrDataCounter(DMA_Stream_USART_GNSS_RX, GNSS_BUFFER_SIZE);
		//temp = DMA_GetCurrDataCounter(DMA_Stream_USART_GNSS_RX);

		//gnss_read_new_data();
//		reset_gnss_rx_buffer();


		/* Enable DMA transfer */
		DMA_Cmd(DMA_Stream_USART_GNSS_RX, ENABLE);
		while (DMA_GetCmdStatus(DMA_Stream_USART_GNSS_RX) != ENABLE) { ; }

		//clear usart IDLE interrupt
		USART_ReceiveData(USART_GNSS);
	}

}
#endif













