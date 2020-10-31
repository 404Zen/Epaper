/**
	******************************************************************************
	* @file    spi.h 
	* @author  
	* @version 1.0
	* @date    2020-10-18
	* @brief   SPI Driver 
	******************************************************************************
	**/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "../SYSTEM/systembase.h"
//#include "delay.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */
void SPI1Init(void);
void SPI1IOInit(void);
uint8_t SPI1WriteReadByte(uint8_t Data);
void SPI1SendByte(uint8_t Data);
uint8_t SPI1ReadByte(uint8_t Data);



#endif  /*__SPI_H*/
