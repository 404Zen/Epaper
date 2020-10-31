/**
  ******************************************************************************
  * @file    spi.c 
  * @author  
  * @version 1.0
  * @date    2020-10-18
  * @brief   SPI Driver
  ******************************************************************************
  **/ 



/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  uint8_t SPI1WriteReadByte(uint8_t Data)
  * @param  None
  * @retval None
  */
uint8_t SPI1WriteReadByte(uint8_t Data)
{
	uint8_t i = 0;

	//等待数据寄存器空
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)	
	{
		i++;

		if(i > 200)
			return 0;
	}

	//发送数据
	SPI_I2S_SendData(SPI1, Data);

	i = 0;
	//等待接收数据完成
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		i++;

		if(i > 200)
			return 0;
	}

	//接收数据
	return SPI_I2S_ReceiveData(SPI1);
}

void SPI1SendByte(uint8_t Data)
{
	uint8_t i = 0;
	//发送数据
	SPI_I2S_SendData(SPI1, Data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		
		if(i > 200)
			break;
	}
}
uint8_t SPI1ReadByte(uint8_t Data)
{
	uint8_t i;
	SPI1SendByte(Data);
	//等待接收数据完成
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		i++;

		if(i > 200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1);
}

/**
  * @brief  SPI1Init
  * @param  None
  * @retval None
  */
void SPI1Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);


	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								//SPI Master
	// SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;					//SPI 单工 只发不收
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//SPI全双工模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							//SPI 8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									//SPI极性 CLK空闲时的电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								//第一个边缘采样 CLK空闲时为低电平，所以是上升沿采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									//片选由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;			//64分频 APB2=72MHz 72/64 = 1.125MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							//高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 10;									//CRC多项式校验
	SPI_Init(SPI1, &SPI_InitStructure);											//初始化SPI1

	SPI1IOInit();																//初始化SPI1 IO
	SPI_Cmd(SPI1, ENABLE);
	
}

/**
  * @brief  SPI1IOInit
  * @param  None
  * @retval None
  */
void SPI1IOInit(void)
{
	//PA5 SPI_CLK   PA7:SPI_MOSI
	GPIO_InitTypeDef    GPIO_InitStructure;
	//开启GPIOA和SPI1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}












