/**
  ******************************************************************************
  * @file    Timer.c 
  * @author  Zen
  * @version 1.0
  * @date    2020-07-05
  * @brief   Timer C Source File
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/
#include "timer.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Timer3 Interrupt Init
  * @param  arr:自动重装载值
  *         psc：预分频系数
  * @retval None
  * @note	在RCC_Configuration()已经将APB1时钟配置2分频,即为36MHz,当APB1分频系数为1时，TIM2~TIM7时钟为
  * 		APB1时钟,当APB1时钟分频系数不为1时,TIM2~TIM7时钟为APB1的2倍
  */
void TIM3_Interrupt_Init(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//TIM结构体
	NVIC_InitTypeDef	NVIC_InitStrcut;					//中断向量结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//时钟使能，TIM3在APB1上

	TIM_TimeBaseInitStruct.TIM_Period		= arr;			//自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler	= psc;			//预分频系数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子 TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;	//向上计数模式，计数到arr会溢出
	// TIM_TimeBaseInitStruct.TIM_RepetitionCounter	= ;		//该寄存器对高级定时器起作用
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);		//TIM3初始化
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				//使能TIM3更新中断,计数器溢出会产生更新中断

	NVIC_InitStrcut.NVIC_IRQChannel		= TIM3_IRQn;		//TIM3中断
	NVIC_InitStrcut.NVIC_IRQChannelPreemptionPriority = 0;	//抢先优先级
	NVIC_InitStrcut.NVIC_IRQChannelSubPriority	= 3;		//子优先级
	NVIC_InitStrcut.NVIC_IRQChannelCmd	= ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStrcut);							//NVIC初始化

	TIM_Cmd(TIM3, ENABLE);									//使能定时器3
}

/**
  * @brief  TIM3_IRQHandler
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	if(ENABLE == TIM_GetITStatus(TIM3, TIM_IT_Update))		//判断是否产生了更新中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			//清除中断标志位
		LED_Toggle();										//执行相应函数
	}
}

/**
  * @brief  TIM1_PWM_Init
  * @param  arr:自动重装载值
  *         psc：预分频系数
  * @retval None
  * @note	
  */
void TIM1_PWM_Init(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef	GPIO_InitStruct;					//需要重新初始化一下PA8，所以要用到GPIO初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//TIM初始化结构体
	TIM_OCInitTypeDef	TIM_OCInitStruct;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);				

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	TIM_TimeBaseInitStruct.TIM_Period		= arr;			//自动重装载值		ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler	= psc;			//预分频系数		PSC
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子 TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;	//向上计数模式，计数到arr会溢出
	// TIM_TimeBaseInitStruct.TIM_RepetitionCounter	= 10;		//重复次数寄存器，该寄存器对高级定时器起作用
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);		//TIM1初始化


	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;							//PWM模式/输出比较模式	PWM模式2
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;				//使能PWM输出到端口/
	TIM_OCInitStruct.TIM_Pulse = 500;										//待装入捕获比较寄存器的值		CCR寄存器，决定占空比
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;					//极性
	// TIM_OCInitStruct.TIM_OCIdleState = ;
	// TIM_OCInitStruct.TIM_OCNPolarity = ;
	// TIM_OCInitStruct.TIM_OutputNState = ;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);	//TIM1 OC1初始化

	TIM_CtrlPWMOutputs(TIM1, ENABLE);	//MOE主输出使能
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);	//TIM1,CH1预装载使能
	TIM_ARRPreloadConfig(TIM1, ENABLE);	//TIM1 arr预装载寄存器使能

	TIM_Cmd(TIM1, ENABLE);				//使能定时器1
}


void TIM1_OC_Init(uint16_t arr, uint16_t psc)      //TIM2 比较模式
{
	GPIO_InitTypeDef	GPIO_InitStruct;					//需要重新初始化一下PA8，所以要用到GPIO初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//TIM初始化结构体
	TIM_OCInitTypeDef	TIM_OCInitStruct;	
	NVIC_InitTypeDef 			NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);				

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//中断优先级配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;				//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 

	TIM_TimeBaseInitStruct.TIM_Period		= arr;			//自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler	= psc;			//预分频系数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子 TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;	//向上计数模式，计数到arr会溢出
	// TIM_TimeBaseInitStruct.TIM_RepetitionCounter	= 10;		//重复次数寄存器，该寄存器对高级定时器起作用
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);		//TIM1初始化

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle;						//输出比较模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;				//使能PWM输出到端口/
	TIM_OCInitStruct.TIM_Pulse = 100;										//待装入捕获比较寄存器的值		CCR寄存器，决定占空比
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;		//
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;		//
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);						//TIM1_CH1初始化

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}

void TIM1_CC_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET)
  {
    TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
    TIM_SetCounter(TIM1,0);
    TIM_SetCompare1(TIM1,100);
  }
}

#if 0
void TIM1_OPM_Init(uint16_t arr, uint16_t psc)     //TIM1  单脉冲模式
{
	GPIO_InitTypeDef	GPIO_InitStruct;					//需要重新初始化一下PA8，所以要用到GPIO初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//TIM初始化结构体
	TIM_OCInitTypeDef	TIM_OCInitStruct;	
	NVIC_InitTypeDef 			NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);				

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//中断优先级配置
	// NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;				//
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	// NVIC_Init(&NVIC_InitStructure); 

	TIM_TimeBaseInitStruct.TIM_Period		= arr;			//自动重装载值	ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler	= psc;			//预分频系数	PSC
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子 TDTS = Tck_tim 	CKD
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;	//向上计数模式，计数到arr会溢出	
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter	= 10;		//重复次数寄存器，该寄存器对高级定时器起作用
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);		//TIM1初始化

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;						//PWM模式2
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;				//使能PWM输出到端口/
	TIM_OCInitStruct.TIM_Pulse = 500;										//待装入捕获比较寄存器的值		CCR寄存器，决定占空比
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		//
	// TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;		//
	// TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);						//TIM1_CH1初始化

	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);			//单脉冲模式
	// TIM_SelectInputTrigger();

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
#endif

/**********************************END OF FILE**********************************/
