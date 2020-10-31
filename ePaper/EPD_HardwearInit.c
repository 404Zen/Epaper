/**
	******************************************************************************
	* @file    ePaper213.c 
	* @author  
	* @version 1.0
	* @date    2020-10-18
	* @brief   ePaper2.13inch Driver (4Wire SPI)
	******************************************************************************
	**/ 

/* Includes ------------------------------------------------------------------*/
#include "EPD_HardwearInit.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ePaperIOInit
  * @param  None
  * @retval None
  */
void EPD2IN13IOInit(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(EPD_RCC_Periph, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//复位拉高		DC 和片选都拉低
	EPD_RST = 1;	EPD_DC = 0; 	EPD_CS = 0;
}

/**
  * @brief  ePaparSPIInit
  * @param  None
  * @retval None
  */
void EPD2IN13SPIInit(void)
{
	SPI1IOInit();
	SPI1Init();
}












