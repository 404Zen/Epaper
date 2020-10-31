/**
  ******************************************************************************
  * @file    key.c 
  * @author  Zen
  * @version 1.0
  * @date    2020-10-11
  * @brief   Key Driver C Source File
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/
#include "key.h"
#include "oled.h"

#define LONG_PRESS_SUPPORT				1					//支持长按键宏定义


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define KEY_BUF_LEN						5

#define SHORT_PRESS_FILTER				30					//短按键滤波延时
#define	LONG_PRESS_DELAY				1000				//长按键判断时间
#define	LONG_PRESS_REPEAT				200					//长按键重复时间

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t KeyValueBuf[KEY_BUF_LEN];
uint8_t	KeyBufCounter = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void KeyTest(void)
{
	uint8_t KeyValue = 0;

	KeyValue = PopKey();

	if(KeyValue)
	{
//		OLED_ShowString(0,2, "KEY VALUE:");
//		OLED_ShowNum(90, 2, KeyValue, 3, 16);
	}
}

/**
  * @brief  KeyIO_Init
  * @param  None
  * @retval None
  */
void KeyIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;										//定义一个GPIO_InitTypeDef类型的结构体

	RCC_APB2PeriphClockCmd(KEY0_PERIPH, ENABLE);								//打开GPIO的外设时钟,目前所有的按键都在同一个PORT

	GPIO_InitStructure.GPIO_Pin = KEY0_PIN | KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN | KEY5_PIN | KEY6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY0_PORT, &GPIO_InitStructure);
}

/**
  * @brief  KeyScan
  * @param  None
  * @retval None
  */
void KeyScan(void)
{
	uint8_t i = 0;
	static uint8_t Status;
	static uint8_t KeyValue;
	static uint32_t TimerForKeyScan = 0;

	switch (Status)
	{
		//初始化
		case 0:
		{
			for (i = 0; i < KEY_BUF_LEN; i++)
			{
				KeyValueBuf[i] = 0;
			}

			Status = 1;
			
		}
		break;

		//读按键
		case 1:
		{
			KeyValue = ReadKey();
			if(KeyValue)			//有按键按下
			{
				TimerReload(&TimerForKeyScan);
				Status = 2;
			}
		}
		break;

		case 2:
		{
			if(KeyValue == ReadKey())	//按键仍未松开
			{
				if(TRUE == TimerDelay(&TimerForKeyScan, SHORT_PRESS_FILTER))	//消抖延时
				{
					PushKey(KeyValue);											//按键入栈
					Status = 3;

					#if LONG_PRESS_SUPPORT
					TimerReload(&TimerForKeyScan);
					#endif
				}
			}
			else
			{
				Status = 1;
			}
		}
		break;

		case 3:
		{	
			//按键已释放
			if(0 == ReadKey())	
			{
				TimerReload(&TimerForKeyScan);
				Status = 1;
			}
			//按键非原值，原来的按键已释放
			else if(KeyValue != ReadKey())
			{
				TimerReload(&TimerForKeyScan);
				Status = 1;
			}
			else			//长按键
			{
				#if LONG_PRESS_SUPPORT
				if(TRUE == TimerDelay(&TimerForKeyScan, LONG_PRESS_DELAY))
				{
					TimerReload(&TimerForKeyScan);
					PushKey(KeyValue |= 0x80);
					Status = 4;
				}
				#endif
			}
			
		}
		break;


		case 4:
		{
			//按键已释放
			// KeyValue = ReadKey();
			if(0 == ReadKey())	
			{
				TimerReload(&TimerForKeyScan);
				Status = 1;
			}
			//按键非原值，原来的按键已释放
			else if(KeyValue != (ReadKey() | 0x80) && KeyValue != (ReadKey() | 0xC0) )
			{
				TimerReload(&TimerForKeyScan);
				Status = 1;
			}
			else			//长按键
			{
				#if LONG_PRESS_SUPPORT
				if(TRUE == TimerDelay(&TimerForKeyScan, LONG_PRESS_REPEAT))
				{
					TimerReload(&TimerForKeyScan);
					PushKey(KeyValue |= 0x40);
				}
				#endif
			}
		}
		break;
		
		default:
			Status = 0;
			break;
	}
}

/**
  * @brief  ReadKey
  * @param  None
  * @retval None
  */
void PushKey(uint8_t KeyValue)
{
	if(KeyBufCounter < KEY_BUF_LEN)
	{
		KeyValueBuf[KeyBufCounter++] = KeyValue;
	}
}

/**
  * @brief  ReadKey
  * @param  None
  * @retval None
  */
uint8_t PopKey(void)
{
	uint8_t i = 0, Temp = 0;

	if(KeyBufCounter)		//不为0 缓存中还有按键值
	{
		Temp = KeyValueBuf[0];

		for (i = 0; i < (KEY_BUF_LEN-1); i++)
		{
			KeyValueBuf[i] = KeyValueBuf[i+1];
		}
		KeyValueBuf[KEY_BUF_LEN-1] = 0;
		KeyBufCounter--;

		return Temp;
	}
	else
	{
		return 0;
	}
}
/**
  * @brief  ReadKey
  * @param  None
  * @retval None
  */
uint8_t ReadKey(void)
{
	// uint8_t res = 0;
	if(0 == GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN))
	{
		return KEY0_PRESS;
	}
	
	if (0 == GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN))
	{
		return KEY1_PRESS;
	}
	
	if (0 == GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN))
	{
		return KEY2_PRESS;
	}
	
	if (0 == GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN))
	{
		return KEY3_PRESS;
	}
	
	if (0 == GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN))
	{
		return KEY4_PRESS;
	}
	
	if (0 == GPIO_ReadInputDataBit(KEY5_PORT, KEY5_PIN))
	{
		return KEY5_PRESS;
	}
	
	if (0 == GPIO_ReadInputDataBit(KEY6_PORT, KEY6_PIN))
	{
		return KEY6_PRESS;
	}
	
	
	return 0;
	
}




