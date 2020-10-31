/**
  ******************************************************************************
  * @file    flash.c
  * @author  
  * @version 1.0
  * @date    2020-04-16
  * @brief   Flash Driver
  ******************************************************************************
  **/ 

 /* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "flash.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  FLASH Write.
  * @param  addr: 写入的地址
  *         data：写入的数据
  * @retval None
  */
void FLASH_Write(uint32_t addr, uint16_t data)
{
    // RCC_HSICmd(ENABLE);          
    FLASH_Unlock();                         //解锁FLASH
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ErasePage(addr);                  //擦除页
    FLASH_ProgramHalfWord(addr, data);      //往指定地址写入数据
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_Lock();                           //上锁
}

/**
  * @brief  FLASH Read.
  * @param  addr: 读出的地址
  * @retval data：读出的数据
  */
uint16_t FLASH_Read(uint32_t addr)
{
    uint16_t data;

    data = *(uint16_t*)(addr);

    return data;

}


