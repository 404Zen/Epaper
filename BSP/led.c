/**
	******************************************************************************
	* @file    led.c 
	* @author  
	* @version 1.0
	* @date    2020-04-16
	* @brief   LEDs Driver
	******************************************************************************
	*/ 
 
 /* Includes ------------------------------------------------------------------*/
#include "led.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
	* @brief  This function Initial LEDs GPIO.
	* @param  None
	* @retval None
	*/
void LED_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_RCC_Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	LED0 = 1;			//关闭LED

#if	MY_MINI_STM32
	RCC_APB2PeriphClockCmd(LED1_RCC_Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	LED1 = 1;			//关闭LED

	RCC_APB2PeriphClockCmd(LED2_RCC_Periph, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
	LED2 = 1;			//关闭LED

#endif
}

void LED_Toggle(void)
{
	LED0 = ~LED0;

#if	MY_MINI_STM32
	LED1 = ~LED1;
	LED2 = ~LED2;
#endif
}









