/**
	******************************************************************************
	* @file    SysTick.C
	* @author  Zen
	* @version V1.0
	* @date    2020-10-11
	* @brief   SysTick Driver Source file
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/

 /* Includes ------------------------------------------------------------------*/
#include "SysTick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t  us_tick = 0;
static uint16_t ms_tick = 0;		//每毫秒的tick数	
static uint32_t SysMSCounter = 0;	//毫秒计数器
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  SysTickInit
  * @param  None
  * @retval None
  */
void SysTickInit(void)
{
	//重装载值为 0xFFFFFF,	8分频， 则一个周期的时间为1.86s	
	us_tick = SystemCoreClock / 8000000;									//9个Tick 1us
	ms_tick = us_tick * 1000;
	//SysTick是一个24位的递减计数器
	//要先配置SysTick_Config 再配置SysTick_CLKSourceConfig
	//tick 不能超过24bit
	if(SysTick_Config(ms_tick))	//重装载值为9000 1ms中断一次
	{
		while (1)
		{
			//SysTick配置错误
			;
		}
	}

	//SysTick时钟源配置 不进行配置则不进行分频					
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);           //HCLK 8分频  	 

}

void SysTick_Handler(void)
{
	//1ms中断一次
	SysMSCounter++;
}

uint32_t GetSysTickValue(void)
{
	return SysTick->VAL;
}


void TimerReload(uint32_t *Timer)
{
	*Timer = SysMSCounter;
}

uint8_t TimerDelay(uint32_t *Timer, uint32_t nMS)
{
	if((SysMSCounter - *Timer) >= nMS)
		return TRUE;
	else
		return FALSE;
}


void Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*us_tick; 							//需要的节拍数	  		 
	tcnt=0;
	// delay_osschedlock();						//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};
	// delay_osschedunlock();						//恢复OS调度									    
}


void Delay_ms(uint16_t nms)
{	
	// if(delay_osrunning&&delay_osintnesting==0)	//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
	// {		 
	// 	if(nms>=fac_ms)							//延时的时间大于OS的最少时间周期 
	// 	{ 
   	// 		delay_ostimedly(nms/fac_ms);		//OS延时
	// 	}
	// 	nms%=fac_ms;							//OS已经无法提供这么小的延时了,采用普通方式延时    
	// }
	Delay_us((uint32_t)(nms*1000));					//普通方式延时  
}


void TimerTest(void)
{
	static uint32_t TestTimer = 0;
	static uint8_t	TestCase = 0;

	switch (TestCase)
	{
	case 0:
		TimerReload(&TestTimer);
		TestCase = 1;
		break;

	case 1:
		if(TRUE == TimerDelay(&TestTimer, 500))
		{
			TimerReload(&TestTimer);
			LED_Toggle();
		}
		break;
	
	default:
		break;
	}
}

