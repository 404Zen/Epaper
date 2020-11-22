/**
  ******************************************************************************
  * @file    main.c 
  * @author  
  * @version 
  * @date    
  * @brief   Main program body
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* IO Definitions ---------------------------------------------------------*/
/*	EPaper(IL3895)	<-->		STM32F103C8T6
	DIN				<-->		PA7			SPI MOSI
	CLK				<-->		PA5			SPI Clock Pin
	CS				<-->		PA4			CS Enable Low
	DC()			<-->		PA2			Data/Cmd Control, H=Data L=Command
	RST				<-->		PA1			RST is active low
	BUSY			<-->		PA3			Busy state output pin, When BUSY is low, the chip is busy
*/


/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	static uint32_t MainLoopTimer = 0;
	uint8_t i = 0;

	RCC_Configuration();
	SysTickInit();
	LED_Init();
	KeyIO_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init(115200);						//115200 8N1
	printf("\r\nUSART1 Init Finished!\r\n");

	EPD2IN13SPIInit();					//初始化对应的SPI
	EPD2IN13IOInit();					//初始化相应的IO

	while (0 == RTC_Init())
	{
		i++;		
		if(i >= 250)
		{
			//RTC Initial Fail!
			printf("RTC Initial Fail!...\r\n");
			break;
		}
	}

	TimerReload(&MainLoopTimer);
	
//	StartUpShow();
	StandbyInit();
	eClockInit();

	while (1)
	{

		KeyScan();
		KeyTest();
		AlarmInit();

		// eClcokService();

		//运行指示灯
		if(TRUE == TimerDelay(&MainLoopTimer, 500))
		{
			TimerReload(&MainLoopTimer);
			LED_Toggle();
		}
	}
}



