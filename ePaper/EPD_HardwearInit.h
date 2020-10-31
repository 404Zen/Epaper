/**
	******************************************************************************
	* @file    ePaper213.h 
	* @author  
	* @version 1.0
	* @date    2020-10-18
	* @brief   ePaper2.13inch Driver (4Wire SPI)
	******************************************************************************
	**/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EPAPER213_H__
#define __EPAPER213_H__

/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "../SYSTEM/systembase.h"
#include "spi.h"



/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
// Display resolution
#define EPD_2IN13_WIDTH       			122
#define EPD_2IN13_HEIGHT      			250

#define EPD_2IN13_FULL					0			
#define EPD_2IN13_PART					1


//相关IO定义
#define EPD_RST_PORT					GPIOA
#define EPD_RCC_Periph					RCC_APB2Periph_GPIOA

// #define EPD_RST_PIN             		GPIO_Pin_1
// #define EPD_DC_PIN             			GPIO_Pin_2
// #define EPD_BUSY_PIN             		GPIO_Pin_3
// #define EPD_CS_PIN             			GPIO_Pin_4

#define EPD_RST							PAout(1)		//低电平复位
#define	EPD_DC							PAout(2)		//1=Data 0=Command
#define EPD_CS							PAout(3)		//低电平选中
#define EPD_BUSY						PAin(3)




/* Exported functions ------------------------------------------------------- */
void EPD2IN13IOInit(void);				//ePaper相关IO初始化
void EPD2IN13SPIInit(void);				//ePaparSPIInit


#endif  /*__EPAPER213_H__*/
