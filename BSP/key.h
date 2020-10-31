/**
	******************************************************************************
	* @file    key.h
	* @author  Zen
	* @version V1.0
	* @date    2020-10-11
	* @brief   Key Driver Headfile
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "../SYSTEM/systembase.h"
//#include "delay.h"
#include "SysTick.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define	KEY0_PIN			GPIO_Pin_0
#define	KEY0_PORT			GPIOB
#define	KEY0_PERIPH			RCC_APB2Periph_GPIOA

#define	KEY1_PIN			GPIO_Pin_1
#define	KEY1_PORT			GPIOB
#define	KEY1_PERIPH			RCC_APB2Periph_GPIOA

#define	KEY2_PIN			GPIO_Pin_2
#define	KEY2_PORT			GPIOB
#define	KEY2_PERIPH			RCC_APB2Periph_GPIOA

#define	KEY3_PIN			GPIO_Pin_3
#define	KEY3_PORT			GPIOB
#define	KEY3_PERIPH			RCC_APB2Periph_GPIOA

#define	KEY4_PIN			GPIO_Pin_4
#define	KEY4_PORT			GPIOB
#define	KEY4_PERIPH			RCC_APB2Periph_GPIOA

#define	KEY5_PIN			GPIO_Pin_5
#define	KEY5_PORT			GPIOB
#define	KEY5_PERIPH			RCC_APB2Periph_GPIOA

#define	KEY6_PIN			GPIO_Pin_6
#define	KEY6_PORT			GPIOB
#define	KEY6_PERIPH			RCC_APB2Periph_GPIOA


#define	KEY0_PRESS			0x30
#define	KEY1_PRESS			0x31
#define	KEY2_PRESS			0x32
#define	KEY3_PRESS			0x33
#define KEY4_PRESS			0x34
#define	KEY5_PRESS			0x35
#define	KEY6_PRESS			0x36

#define	ESC_PRESS			KEY0_PRESS
#define	LEFT_PRESS			KEY1_PRESS
#define	UP_PRESS			KEY2_PRESS
#define	DOWN_PRESS			KEY3_PRESS
#define RIGHT_PRESS			KEY4_PRESS
#define	ENTER_PRESS			KEY5_PRESS
#define	ENABLE_PRESS		KEY6_PRESS

void KeyIO_Init(void);
void KeyScan(void);
void KeyTest(void);

void PushKey(uint8_t KeyValue);			//按键入栈
uint8_t PopKey(void);					//按键出栈
uint8_t ReadKey(void);



#endif
