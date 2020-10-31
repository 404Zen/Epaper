/**
  ******************************************************************************
  * @file    systembase.c 
  * @author  
  * @version 1.0
  * @date    2020-04-16
  * @brief   
  ******************************************************************************
  */ 
 
 /* Includes ------------------------------------------------------------------*/
#include "systembase.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  RCC时钟设置
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();                       //RCC寄存器复位
	RCC_HSEConfig(RCC_HSE_ON);          //HSE ON
	
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     //等待外部高速时钟启动完成
	if(SUCCESS == HSEStartUpStatus)
	{
		//PLL时钟为HSE 1分频，9倍频	72MHz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		//HCLK时钟为RCC时钟1分频  72MHz
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//PCLK1/AHB(APB1)低速时钟设置为RCC_HCLK_Div2	36MHz
		RCC_PCLK1Config(RCC_HCLK_Div2);
		//PCLK2/AHB(APB2)高速时钟设置为RCC_HCLK_Div1	72MHz
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//FLASH存储器延时时钟周期
		FLASH_SetLatency(FLASH_Latency_2);
		//FLASH预取指缓存使能
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		//使能PLL
		RCC_PLLCmd(ENABLE);
		//等待PLL输出稳定
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		//选择SYSCLK为PLL
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//等待PLL成为SYSCLK的时钟源
		while(RCC_GetSYSCLKSource() != 0x08);
	}
}



