/**
	******************************************************************************
	* @file    iic.h 
	* @author  
	* @version 1.0
	* @date    2020-04-25
	* @brief   IIC Driver (IO模拟)
	******************************************************************************
	**/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IIC_H
#define __IIC_H

/* Includes ------------------------------------------------------------------*/
#include "../CMSIS/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "../SYSTEM/systembase.h"
#include "SysTick.h"

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define IIC_PORT            GPIOB
#define SCL_PIN             GPIO_Pin_6
#define SDA_PIN             GPIO_Pin_7

#define IIC_SCL             PBout(6)
#define IIC_SDA             PBout(7)
#define READ_SDA            PBin(7)

#define SDA_IN()            {GPIOB->CRL &= 0x0FFFFFFF; GPIOB->CRL |= (uint32_t) 8 << 28;}
#define SDA_OUT()           {GPIOB->CRL &= 0x0FFFFFFF; GPIOB->CRL |= (uint32_t) 3 << 28;}


/* Exported functions ------------------------------------------------------- */
void IIC_Init(void);              //IIC初始化
void IIC_Start(void);             //IIC起始信号
void IIC_Stop(void);              //IIC停止信号
uint8_t IIC_Wait_Ack(void);       //等待IIC应答信号
uint8_t Check_Ack(void);          //
void IIC_Ack(void);               //
void IIC_NAck(void);              //
void IIC_SendByte(uint8_t data);
uint8_t IIC_ReadByte(uint8_t ack);



uint8_t IICBusReadOneByte(uint8_t DeviceAddr, uint8_t RegAddr);
uint8_t IICBusWriteOneByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t Data);


uint8_t IICBusWriteNByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t *Ptr, uint8_t Length);
// uint8_t IICBusReadByte(uint8_t device_addr, uint8_t reg_addr);
uint8_t IICBusReadNByte(uint8_t Device_addr, uint8_t Reg_addr, uint8_t *Ptr, uint8_t Length);
#endif  /*__IIC_H*/
