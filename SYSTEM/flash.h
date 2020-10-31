/**
  ******************************************************************************
  * @file    flash.h 
  * @author  
  * @version 1.0
  * @date    2020-04-25
  * @brief   FLASH Driver
  ******************************************************************************
  **/ 

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

//定义写入的起始地址                    
//STM32F103x8T6:0x08000000~0x0800FFFF   64K
//STM32F103xBT6:0x08000000~0x0801FFFF   128K                               
#define FLASH_START_ADDR            0x0800F000        //64K-最后4K      


/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
// #include "stm32f10x_flash.h"
// #include "stm32f10x_rcc.h"
#include "../SYSTEM/systembase.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */
void FLASH_Write(uint32_t addr, uint16_t data);
uint16_t FLASH_Read(uint32_t addr);





#endif  /*__FLASH_H*/

