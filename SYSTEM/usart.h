/**
  ******************************************************************************
  * @file    usart.h 
  * @author  
  * @version 1.0
  * @date    2020-04-25
  * @brief   USART Driver for STM32F1
  ******************************************************************************
  **/ 

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "../SYSTEM/systembase.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define USART_n             USART1              //定义printf函数所使用的串口
#define USART1_REC_LEN      128

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];   //接收缓存
extern uint16_t USART1_RX_STA;                   //接收状态标记



/* Exported functions ------------------------------------------------------- */
void USART1_Init(uint32_t bound);                       //串口1初始化函数，只可以配置波特率，其他的配置为8N1
void USART1_printf(char *fmt, ...);




#endif  /*__USART_H*/

