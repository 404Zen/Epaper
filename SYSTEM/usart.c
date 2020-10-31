/**
  ******************************************************************************
  * @file    usart.c
  * @author  
  * @version 1.0
  * @date    2020-04-16
  * @brief   usart Driver
  ******************************************************************************
  **/ 

 /* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/
uint8_t  USART1_RX_BUF[USART1_REC_LEN];   //接收缓存
uint16_t USART1_RX_STA = 0;                   //接收状态标记
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


//重定向fputc函数
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE 
{ 
	int handle;
}; 

FILE __stdout; 

_sys_exit(int x) 
{ 
	x = x; 
}


_ttywrch(int ch)
{
    ch = ch;
}

int fputc(int ch, FILE *f)
{
	while((USART_n->SR&0x40) == 0);             //等待发送完毕
			USART_n->DR = (uint8_t)ch;
	return ch;
}
#endif

/**
  * @brief  .
  * @param  None
  * @retval None
  */
void USART1_Init(uint32_t bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;

	//使能串口时钟与GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	//PA9:USART1_TX
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;              //复用推挽输出
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PA10:USART1_RX
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate      = bound;
	USART_InitStructure.USART_WordLength    = USART_WordLength_8b;                  //字长      8bit
	USART_InitStructure.USART_StopBits      = USART_StopBits_1;                     //停止位    1
	USART_InitStructure.USART_Parity        = USART_Parity_No;                      //校验      无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
	USART_InitStructure.USART_Mode          = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(USART1, &USART_InitStructure);                                       //
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                  //接收缓冲区非空中断使能 
	USART_Cmd(USART1, ENABLE);                                                      //使能串口1
}

/**
  * @brief  USART1 printf函数.
  * @param  None
  * @retval None
  */
void USART1_printf(char *fmt, ...)
{
	char buffer[USART1_REC_LEN+1];
	uint8_t i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, USART1_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART1_REC_LEN) && (i < strlen(buffer)))
	{
		USART_SendData(USART1, (uint8_t)buffer[i++]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
	va_end(arg_ptr);
}

/**
  * @brief  USART1中断服务函数.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     //接收中断
	{
		Res = USART_ReceiveData(USART1);                        //读取收到的数据
		if((USART1_RX_STA&0x8000) == 0)                         //接收未完成
		{
			if((USART1_RX_STA&0x4000) == 0)
			{
				if(Res != 0x0A)
					USART1_RX_STA = 0;              //接收未完成
				else            
					USART1_RX_STA |= 0x8000;        //接收完成
			}
			else
			{
				if(Res == 0x0D)
					USART1_RX_STA |= 0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0x3FFF] = Res;
					USART1_RX_STA++;
					if (USART1_RX_STA > (USART1_REC_LEN - 1))
						USART1_RX_STA = 0;
				}
			}
		}
	}
}


