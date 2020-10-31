/**
	******************************************************************************
	* @file    led.h 
	* @author  
	* @version 1.0
	* @date    2020-04-16
	* @brief   LEDs Driver
	******************************************************************************
	**/ 

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H


#define BLUE_PILL           1
#define MY_MINI_STM32		0
#define FLY_BOARD			0


/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "../SYSTEM/systembase.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#if BLUE_PILL

#define LED_PIN             GPIO_Pin_12
#define LED_PORT            GPIOB
#define LED_RCC_Periph		RCC_APB2Periph_GPIOB
#define LED0								PBout(12)

#elif MY_MINI_STM32
#define LED_PIN				GPIO_Pin_12
#define LED_PORT			GPIOC
#define LED_RCC_Periph		RCC_APB2Periph_GPIOC
#define LED0								PCout(12)

#define LED1_PIN			GPIO_Pin_8
#define LED1_PORT			GPIOA
#define LED1_RCC_Periph		RCC_APB2Periph_GPIOA
#define LED1								PAout(8)

#define LED2_PIN			GPIO_Pin_2
#define LED2_PORT			GPIOD
#define LED2_RCC_Periph		RCC_APB2Periph_GPIOD
#define LED2								PDout(2)

#elif FLY_BOARD

#define LED_PIN             GPIO_Pin_8
#define LED_PORT            GPIOB
#define LED_RCC_Periph		RCC_APB2Periph_GPIOB
#define LED0								PBout(8)

#endif


/* Exported functions ------------------------------------------------------- */
void LED_Init(void);
void LED_Toggle(void);







#endif /* __LED_H */

