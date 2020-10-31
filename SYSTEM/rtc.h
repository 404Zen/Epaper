/**
	******************************************************************************
	* @file    rtc.h 
	* @author  Zen
	* @version 1.0
	* @date    2020-10-13
	* @brief   STM32 Real-Time-Clock Driver Headfile
	******************************************************************************
	*/ 
 
 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define	__RTC_H__


 /* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "SysTick.h"
/* Exported types ------------------------------------------------------------*/
//时间结构体
typedef struct
{
	uint8_t		Hour;					//时
	uint8_t		Minute;					//分
	uint8_t 	Second;					//秒

	uint16_t	Year;					//年
	uint8_t		Month;					//月
	uint8_t		Week;					//周
	uint8_t		Day;					//日
}CalendarStruct;
extern CalendarStruct		Calendar;	//


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/



/* Exported functions ------------------------------------------------------- */
uint8_t RTC_Init(void);										//RTC初始化函数
uint8_t RTC_TimeReFresh(void);								//时间刷新函数
uint8_t	LeapYearCheck(uint16_t Year);						//判断是否是闰年
uint8_t	DayOfWeek(uint16_t Year, uint8_t Month, uint8_t Day);//星期几计算

//根据年月日时分秒设置RTC_CNT
uint8_t	SetRTCCNTReg(CalendarStruct	CalendarSet);

#endif		
