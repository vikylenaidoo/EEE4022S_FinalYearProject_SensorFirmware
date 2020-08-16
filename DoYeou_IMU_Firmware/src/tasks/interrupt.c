/*
 * interrupt.c
 *
 *  Created on: Mar 5, 2018
 *      Author: Do Yeou Ku
 */

#include "interrupt.h"
/* Private defines -----------------------------------------------------------*/
#define RCC_AHBPeriphClockCmd_EXTI_GPIO			RCC_AHB1PeriphClockCmd
#define RCC_AHBPeriph_EXTI_GPIO		   			RCC_AHB1Periph_GPIOA
#define GPIO_EXTI		              			GPIOA
#define GPIO_Pin_EXTI_ACCEL	           			GPIO_Pin_0
#define GPIO_Pin_EXTI_GYRO             			GPIO_Pin_1
#define GPIO_Pin_EXTI_MAG	           			GPIO_Pin_2
#define GPIO_Pin_EXTI_BARO             			GPIO_Pin_3
#define GPIO_Pin_EXTI_PPS						GPIO_Pin_4

#define RCC_APBPeriphClockCmd_EXTI				RCC_APB2PeriphClockCmd
#define RCC_APBPeriph_EXTI						RCC_APB2Periph_SYSCFG

#define GPIO_EXTILine_Config					SYSCFG_EXTILineConfig
#define GPIO_Pin_EXTI_Src						EXTI_PortSourceGPIOA
#define GPIO_Pin_EXTI_ACCEL_Src					EXTI_PinSource0
#define GPIO_Pin_EXTI_GYRO_Src					EXTI_PinSource1
#define GPIO_Pin_EXTI_MAG_Src					EXTI_PinSource2
#define GPIO_Pin_EXTI_BARO_Src					EXTI_PinSource3
#define GPIO_Pin_EXTI_PPS_Src					EXTI_PinSource4

#define EXTILine_ACCEL							EXTI_Line0
#define EXTILine_GYRO							EXTI_Line1
#define EXTILine_MAG							EXTI_Line2
#define EXTILine_BARO							EXTI_Line3
#define EXTILine_PPS							EXTI_Line4

#define EXTILine_ACCEL_IRQn						EXTI0_IRQn
#define EXTILine_GYRO_IRQn						EXTI1_IRQn
#define EXTILine_MAG_IRQn						EXTI2_IRQn
#define EXTILine_BARO_IRQn						EXTI3_IRQn
#define EXTILine_PPS_IRQn						EXTI4_IRQn

#define EXTILine_ACCEL_IRQHandler				EXTI0_IRQHandler
#define EXTILine_GYRO_IRQHandler				EXTI1_IRQHandler
#define EXTILine_MAG_IRQHandler					EXTI2_IRQHandler
#define EXTILine_BARO_IRQHandler				EXTI3_IRQHandler
#define EXTILine_PPS_IRQHandler					EXTI4_IRQHandler

/* Private variables ---------------------------------------------------------*/
int MagDataReady;

/* Static functions ----------------------------------------------------------*/
static
void exti_on (void)
{
	/* EXTI GPIO Configuration --------------------------------------------------*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable clock for SYSCFG */
    RCC_APBPeriphClockCmd_EXTI(RCC_APBPeriph_EXTI, ENABLE);
    /* Enable clock for GPIOx */
    RCC_AHBPeriphClockCmd_EXTI_GPIO(RCC_AHBPeriph_EXTI_GPIO, ENABLE);

    /* Set pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

    /* EXTI Accelerometer pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI_ACCEL;
    GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);

    /* EXTI Gyroscope pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI_GYRO;
    GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);

    /* EXTI Magnetometer pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI_MAG;
    GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);

    /* EXTI Barometer pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI_BARO;
    GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);

    /* EXTI PPS pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI_PPS;
    GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);

    /* SYSCFG EXTI_Line configuration*/
    GPIO_EXTILine_Config(GPIO_Pin_EXTI_Src, GPIO_Pin_EXTI_ACCEL_Src);
    GPIO_EXTILine_Config(GPIO_Pin_EXTI_Src, GPIO_Pin_EXTI_GYRO_Src);
    GPIO_EXTILine_Config(GPIO_Pin_EXTI_Src, GPIO_Pin_EXTI_MAG_Src);
    GPIO_EXTILine_Config(GPIO_Pin_EXTI_Src, GPIO_Pin_EXTI_BARO_Src);
    GPIO_EXTILine_Config(GPIO_Pin_EXTI_Src, GPIO_Pin_EXTI_PPS_Src);

    /* EXTI Configuration --------------------------------------------------*/
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = EXTILine_ACCEL|EXTILine_GYRO|EXTILine_MAG|EXTILine_BARO|EXTILine_PPS;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStructure);

    /* EXTI NVIC Configuration --------------------------------------------------*/
    NVIC_InitTypeDef NVIC_InitStructure;

#ifdef STM32_IMU_USE
    /* Accelerometer */
    NVIC_InitStructure.NVIC_IRQChannel = EXTILine_ACCEL_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_ACCEL_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_ACCEL_Sub_Priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Gyroscope */
    NVIC_InitStructure.NVIC_IRQChannel = EXTILine_GYRO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_GYRO_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_GYRO_Sub_Priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Magnetometer */
//    NVIC_InitStructure.NVIC_IRQChannel = EXTILine_MAG_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_MAG_Priority;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_MAG_Sub_Priority;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
#endif

    /* Barometer */
    NVIC_InitStructure.NVIC_IRQChannel = EXTILine_BARO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_BARO_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_BARO_Sub_Priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* PPS */
    NVIC_InitStructure.NVIC_IRQChannel = EXTILine_PPS_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_PPS_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_PPS_Sub_Priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*-----------------------------------------------------------------------
 Message Frame functions
 ------------------------------------------------------------------------*/
#ifdef STM32_IMU_USE
static
void add_prefix (Sensor_TypeDef sensor)
{
	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = '\n';

	switch(sensor){
	case(Accelerometer)	:	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = BMX055_ACC_ID;		break;
	case(Gyroscope)		:	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = BMX055_GYRO_ID;	break;
	case(Magnetometer)	:	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = BMX055_MAG_ID;		break;
	default				:	;}
	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = ',';

	for (int i = 0; i < 4; i++){
		(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = (uint8_t)((time_stamp.clk_1Hz & ((uint32_t)0xFF<<(24-8*i)))>>(24-8*i));
	}
	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = '.';

	for (int i = 0; i < 2; i++){
		(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = (uint8_t)((time_stamp.clk_10kHz & ((uint32_t)0xFF<<(8-8*i)))>>(8-8*i));
	}
	(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = ',';
}

static
void check_overflow (void)
{
	if ((sensor_crnt_pos + MAX_DATA_LENGTH ) > SENSOR_LOG_BUFFER_SIZE){
		(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = '\n';
		while(sensor_crnt_pos < SENSOR_LOG_BUFFER_SIZE)
			(sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos++] = '\0';

		if (sensor_log_start == 0xff)	sensor_log_start = sensor_crnt_buffer;
		else							sensor_log_wait++;

		sensor_crnt_pos			=	0;
		sensor_crnt_buffer		=	(sensor_crnt_buffer + 1)%SENSOR_BUFFER_NUMBER;
	}
}
#endif

/* Public functions ----------------------------------------------------------*/
void exti_initialize (void){
	imu_log_on = RESET;
	MagDataReady = 0;
	exti_on();
}

void exti_deinitialize (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

	NVIC_DisableIRQ(EXTI0_IRQn);
	NVIC_DisableIRQ(EXTI1_IRQn);
	NVIC_DisableIRQ(EXTI2_IRQn);
	NVIC_DisableIRQ(EXTI4_IRQn);

    EXTI_InitStructure.EXTI_Line = EXTILine_ACCEL|EXTILine_GYRO|
    		EXTILine_MAG|EXTILine_BARO|EXTILine_PPS;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

    /* EXTI Accelerometer pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_EXTI_ACCEL|GPIO_Pin_EXTI_GYRO|
    		GPIO_Pin_EXTI_MAG|GPIO_Pin_EXTI_BARO|GPIO_Pin_EXTI_PPS;
    GPIO_Init(GPIO_EXTI, &GPIO_InitStructure);
}

/* Interrupt Service Routine (ISR) -------------------------------------------*/
#ifdef STM32_IMU_USE
void EXTILine_ACCEL_IRQHandler(void){
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTILine_ACCEL) != RESET) {
    	if (imu_log_on == SET)
    	{
        	add_prefix (Accelerometer);
        	if(accel_read(&((sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos]), ACCEL_DATA_LENGTH) == SENS_OK)
        	{
        		sensor_crnt_pos += ACCEL_DATA_LENGTH;
        	}
        	check_overflow	();

//			add_prefix (Gyroscope);
//			if(gyro_read(&((sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos]), GYRO_DATA_LENGTH) == SENS_OK)
//			{
//				sensor_crnt_pos += GYRO_DATA_LENGTH;
//			}
//			check_overflow	();

        	MagDataReady++;
        	if (MagDataReady == MAG_COUNT_125HZ)
        	{
    			add_prefix (Magnetometer);
    			if(mag_read(&((sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos]), MAG_DATA_LENGTH) == SENS_OK)
    			{
    				sensor_crnt_pos += MAG_DATA_LENGTH;
    			}
    			check_overflow	();
    			MagDataReady = 0;
        	}
    	}
    	/* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTILine_ACCEL);
    }
}

void EXTILine_GYRO_IRQHandler(void){
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTILine_GYRO) != RESET) {
    	if (imu_log_on == SET)
    	{
			add_prefix (Gyroscope);
			if(gyro_read(&((sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos]), GYRO_DATA_LENGTH) == SENS_OK)
			{
				sensor_crnt_pos += GYRO_DATA_LENGTH;
			}
			check_overflow	();
    	}
    	/* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTILine_GYRO);
    }
}

//void EXTILine_MAG_IRQHandler(void){
//    /* Make sure that interrupt flag is set */
//    if (EXTI_GetITStatus(EXTILine_MAG) != RESET) {
//    	if (imu_log_on == SET)
//    	{
//			add_prefix (Magnetometer);
//			if(mag_read(&((sensor_buffer[sensor_crnt_buffer])[sensor_crnt_pos]), MAG_DATA_LENGTH) == SENS_OK)
//			{
//				sensor_crnt_pos += MAG_DATA_LENGTH;
//			}
//			check_overflow	();
//    	}
//        /* Clear interrupt flag */
//        EXTI_ClearITPendingBit(EXTILine_MAG);
//    }
//}
#endif

void EXTILine_BARO_IRQHandler(void){
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTILine_BARO) != RESET) {

    	file_flag.log_enabled = 0;

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTILine_BARO);
    }
}

#define TIMER_STAMP								TIM3

void EXTILine_PPS_IRQHandler(void){
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTILine_PPS) != RESET) {

    	time_stamp.clk_1Hz		+=	1;
    	time_stamp.clk_10kHz	=	0;
    	TIM_SetCounter(TIMER_STAMP,(uint32_t)0);
    	if(time_stamp.clk_1Hz == 5)
    	{
    		imu_log_on = SET;
    		division_line = SET;
    	}

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTILine_PPS);
    }
}


