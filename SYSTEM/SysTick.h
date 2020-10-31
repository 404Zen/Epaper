/**
	******************************************************************************
	* @file    SysTick.h
	* @author  Zen
	* @version V1.0
	* @date    2020-10-11
	* @brief   SysTick Driver Headfile
	******************************************************************************
	* @attention
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTICK_H
#define __SYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "systembase.h"
#include "../BSP/led.h"


void SysTickInit(void);
uint32_t GetSysTickValue(void);

void TimerReload(uint32_t *Timer);
uint8_t TimerDelay(uint32_t *Timer, uint32_t nMS);
void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);

void TimerTest(void);







#endif
