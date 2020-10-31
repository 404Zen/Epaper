/**
  ******************************************************************************
  * @file    timer.h 
  * @author  
  * @version 1.0
  * @date    2020-07-05
  * @brief   Timer Driver
  ******************************************************************************
  **/ 

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

   


/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "../SYSTEM/systembase.h"
#include "../BSP/led.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */

void TIM3_Interrupt_Init(uint16_t arr, uint16_t psc);		//TIM3中断初始化函数
// void TIM3_IRQHandler(void);									//TIM3中断服务函数
void TIM1_PWM_Init(uint16_t arr, uint16_t psc);				//TIM1 PWM模式2输出初始化
void TIM1_PWM_Test(void);									//TIM1 PWM测试函数

void TIM1_OC_Init(uint16_t arr, uint16_t psc);      //TIM1 比较模式
void TIM1_OPM_Init(uint16_t arr, uint16_t psc);     //TIM1  单脉冲模式

#endif  /*__TIMER_H*/

