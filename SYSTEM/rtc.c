/**
  ******************************************************************************
  * @file    rtc.c 
  * @author  Zen
  * @version 1.0
  * @date    2020-10-13
  * @brief   STM32 Real-Time-Clock Driver
  ******************************************************************************
  */ 
 
 /* Includes ------------------------------------------------------------------*/
#include "rtc.h"

CalendarStruct		Calendar;


static uint8_t	TimeRefreshFlag = 0;			//手动修改了时间 需要刷新所有的值

/* Private typedef -----------------------------------------------------------*/
const uint8_t	NonleapYaerMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
const uint8_t	WeekFixTable[12] = {0,3,3,6,1,4,6,2,5,0,3,5};					
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  RTC_Init
  * @param  None
  * @retval 1：Success 0:Fail
  */
uint8_t RTC_Init(void)
{
	uint8_t Temp = 0;

	NVIC_InitTypeDef	NVIC_InitStructure;

	//使能PWR和BKP外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	//使能后备区域寄存器访问
	PWR_BackupAccessCmd(ENABLE);
	
	//向后备寄存器写入指定的数据判断是否第一次配置时钟，一般情况下，只有完全掉电后后备区域寄存器的值才会复位
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)			//从后备区域寄存器读出非写入的指定数据
	{
		BKP_DeInit();                   //复位备份区域
		RCC_LSEConfig(RCC_LSE_ON);      //外部低速时钟开启
		//等待外部低速时钟就绪
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET)
		{
			Temp++;
			Delay_ms(10);

			//外部低速时钟初始化失败
			if(Temp > 200)
			{
				return 0;
			}
		}
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);             //RTC时钟源选择
		RCC_RTCCLKCmd(ENABLE);                              //RTC使能
		RTC_WaitForLastTask();								//等待最后一次RTC寄存器操作完成
		RTC_WaitForSynchro();								//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);					//使能RTC秒中断
		RTC_WaitForLastTask();								//等待最后一次RTC寄存器操作完成
		RTC_EnterConfigMode();								//RTC允许配置
		RTC_SetPrescaler(32767);							//设置RTC时钟分频系数
		RTC_WaitForLastTask();								//等待最后一次RTC寄存器操作完成
		RTC_SetCounter(0);									//RTC计数器的值设为0 UNIX时间起始点
		RTC_WaitForLastTask();								//等待最后一次RTC寄存器操作完成
		RTC_ExitConfigMode();								//退出RTC配置模式

		//向后备寄存器写入指定数据 除了侵入时间外 只有完全掉电后备寄存器才会复位
		BKP_WriteBackupRegister(BKP_DR1, 0x5050);			
	}
	//不是首次上电配置RTC,继续计时
	else		
	{
		RTC_WaitForSynchro();								//等待最近一次对RTC寄存器的操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);					//使能RTC秒中断
		RTC_WaitForLastTask();								//等待最后一次RTC寄存器操作完成
	}

	//RTC中断配置
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;				//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//中断使能
	NVIC_Init(&NVIC_InitStructure);								//中断初始化

	//刷新时间

	return 1;
}

/**
  * @brief  RTC_IRQHandler
  * @param  None
  * @retval RTC中断复位函数
  */
void RTC_IRQHandler(void)
{	
	//秒中断，刷新时间
	if(RESET != RTC_GetITStatus(RTC_IT_SEC))
	{
		RTC_TimeReFresh();
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
	//闹钟中断，
	if(RESET != RTC_GetITStatus(RTC_IT_ALR))
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);					//清除中断

	}
	
	RTC_ClearITPendingBit(RTC_IT_OW);
	
	RTC_WaitForLastTask();									//等待最后一次RTC寄存器操作完成
}

/**
  * @brief  RTC_TimeReFresh
  * @param  None
  * @retval 将UNIX转换成标准时间，RTC时间刷新函数，每秒只对时间结构体中的秒进行+1操作，
  * 		每小时转换一次RTC_CNT的值；避免每秒都对RTC_CNT进行转换，减小计算量，提高程序运行效率。
  */
uint8_t RTC_TimeReFresh(void)
{
	static uint16_t	HourFlag = 65535;
	static uint16_t DayCounter = 65535;		//最长记录天数为49710天
	uint32_t	RTCCounter = 0;
	uint32_t	TempU32 = 0;
	uint16_t	i = 0;

	if((HourFlag >= 3600 ) || 1 == TimeRefreshFlag)
	{
		TimeRefreshFlag = 0;
		RTCCounter = RTC_GetCounter();		//获取RTC时钟计数器的值
		TempU32 = RTCCounter/86400;			//计算天数
		//与保存的值不等，则重新计算天数
		if(DayCounter != TempU32)			
		{
			DayCounter = TempU32;
			i = 1970;						//UNIX时间从1970年1月1日开始
			
			while (TempU32 >= 365)			//超过1年
			{	
				//闰年判断
				if(LeapYearCheck(i))
				{
					if(TempU32 >= 366)
					{
						TempU32 -= 366;
						i++;
					}
					else
					{
						i++;
						break;
					}
				}
				else
				{
					TempU32 -= 365;
					i++;
				}
			}
			Calendar.Year = i;				//年份计算完成

			i = 0;
			while (TempU32 >= 28)					//大于1个月
			{
				//闰年且为2月
				if(LeapYearCheck(Calendar.Year) && i == 1)
				{
					if(TempU32 >= 29)
					{
						TempU32 -= 29;

					}
					else
					{
						break;
					}
				}
				
				else
				{
					if(TempU32 >= NonleapYaerMonth[i])
					{
						TempU32 -= NonleapYaerMonth[i];
					}	
					else
					{
						break;
					}		
				}
				i++;
			}
			Calendar.Month = i + 1;			//月份计算完成
			Calendar.Day = TempU32 + 1;		//日期
		}

		TempU32 = RTCCounter%86400;								//求出剩余的秒钟数
		Calendar.Hour = TempU32 / 3600;							//小时
		Calendar.Minute = (TempU32 % 3600) / 60;				//分钟
		Calendar.Second = (TempU32 % 3600) % 60;				//秒钟
		HourFlag = TempU32%3600;

		Calendar.Week = DayOfWeek(Calendar.Year, Calendar.Month, Calendar.Day);

		return 1;
	}
	else
	{
		HourFlag++;

		Calendar.Minute = HourFlag / 60;				//分钟
		if(Calendar.Minute > 59)
			Calendar.Minute = 0;
		Calendar.Second = (HourFlag % 3600) % 60;				//秒钟
	}
	return	1;
}

/**
  * @brief  uint8_t	LeapYearCheck(uint16_t Year)
  * @param  Year:年份
  * @retval 1：Leap Year 0：Nonleap Year
  */
uint8_t	LeapYearCheck(uint16_t Year)
{
	if((Year % 4) == 0)					//闰年可以被4整除
	{
		if((Year % 100) == 0)				//年份00结尾的，还需要加入能被400整除才是闰年
		{
			if((Year % 400) == 0)
				return 1;
			else
				return 0;
		}
		else
			return 1;
	}
	else
		return 0;
}

/**
  * @brief  uint8_t	DayOfWeek(uint16_t Year, uint8_t Month, uint8_t Day)
  * @param  Year:年份
  * 		Month：月份
  * 		Day:日期
  * @retval Weekn
  */
uint8_t	DayOfWeek(uint16_t Year, uint8_t Month, uint8_t Day)
{
	uint8_t Century, Years;
	uint16_t i = 0;

	Century = Year / 100;		//世纪
	Years	= Year % 100;		//年份

	if(Century > 19)			//超过2000年 年份+100
		Years += 100;

	i = Years + Years/4;
	i %= 7;
	i = i + Day + WeekFixTable[Month - 1];
	
	if(Years %4 == 0  && Month < 3)
		i--;
	
	return (i%7);
}

/**
  * @brief  根据年月日时分秒设置RTC_CNT
  * @param  CalendarSet:需要设置的年月日的结构体
  * @retval 1:Success	0:Fail
  */
uint8_t	SetRTCCNTReg(CalendarStruct	CalendarSet)
{
	uint16_t i = 0;
	uint32_t RTCCNTValue = 0;

	if(CalendarSet.Year < 1970 || CalendarSet.Year > 2099)
		return 0;

	//年份的秒钟数
	for (i = 1970; i < CalendarSet.Year; i++)
	{
		if(LeapYearCheck(i))
		{
			RTCCNTValue += 31622400;	//闰年秒数
		}
		else
		{
			RTCCNTValue += 31536000;	//平年秒数
		}
	}


	//月份的秒钟数
	CalendarSet.Month -= 1;
	for(i = 0; i < CalendarSet.Month; i++)
	{
		RTCCNTValue += (uint32_t)NonleapYaerMonth[i] * 86400;
		//该年是闰年，2月份
		if((LeapYearCheck(CalendarSet.Year)) && i == 1)
		{
			RTCCNTValue += 86400;
		}
	}

	//日的秒钟数
	RTCCNTValue += (uint32_t)(CalendarSet.Day - 1) * 86400;
	//时的秒钟数
	RTCCNTValue += (uint32_t)CalendarSet.Hour * 3600;
	//分钟的秒钟数
	RTCCNTValue += (uint32_t)CalendarSet.Minute * 60;
	//秒钟数
	RTCCNTValue += CalendarSet.Second;

	//使能PWR和BKP外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	//使能后备寄存器访问
	PWR_BackupAccessCmd(ENABLE);
	//设置RTC_CNT的值
	RTC_SetCounter(RTCCNTValue);

	//等待操作完成
	RTC_WaitForLastTask();

	//手动设置了时间 置位标志
	TimeRefreshFlag = 1;

	return 1;
}

