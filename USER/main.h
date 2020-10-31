/**
	******************************************************************************
	* @file    main.c 
	* @author  
	* @version 
	* @date    
	* @brief   Main program body
	******************************************************************************
	*/  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__




#define RST_Pin				GPIO_Pin_1
#define RST_GPIO_Port		GPIOA
#define DC_Pin				GPIO_Pin_2
#define DC_GPIO_Port		GPIOA
#define BUSY_Pin			GPIO_Pin_3
#define BUSY_GPIO_Port		GPIOA
#define SPI_CS_Pin			GPIO_Pin_4
#define SPI_CS_GPIO_Port	GPIOA

/* Includes ------------------------------------------------------------------*/

//Stander C Lib
#include <stdio.h>

//STM32F10x 
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"


//system
#include "../SYSTEM/flash.h"
#include "../SYSTEM/usart.h"
#include "../SYSTEM/timer.h"
#include "../SYSTEM/SysTick.h"
#include "../SYSTEM/rtc.h"

//BSP
#include "../BSP/led.h"
#include "../BSP/key.h"

//ePaper
#include "../ePaper/ePaper213.h"



#endif

