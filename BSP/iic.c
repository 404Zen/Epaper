/**
  ******************************************************************************
  * @file    iic.c 
  * @author  
  * @version 1.0
  * @date    2020-04-25
  * @brief   IIC Driver (IO模拟)
  **************************************c****************************************
  **/ 



/* Includes ------------------------------------------------------------------*/
#include "iic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  IIC初始化
  * @param  None
  * @retval None
  */
void IIC_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	//使能GPIOB时钟
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    

	GPIO_InitStructure.GPIO_Pin     = SDA_PIN;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_OD;			//复用开漏输出
	//GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin     = SCL_PIN;			
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);

	GPIO_SetBits(IIC_PORT, SCL_PIN | SDA_PIN);
}


/**
  * @brief  IIC起始信号
  * @param  None
  * @retval None
  */
void IIC_Start(void)
{
	SDA_OUT();                  //SDA输出模式
	IIC_SDA = 1;
	IIC_SCL = 1;
	Delay_us(4);
	IIC_SDA = 0;
	Delay_us(4);
	IIC_SCL = 0;
}

/**
  * @brief  IIC停止信号
  * @param  None
  * @retval None
  */
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	Delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1;
	Delay_us(4);
}

/**
  * @brief  等待IIC应答信号
  * @param  None
  * @retval 0：有应答	1：无应答
  */
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	SDA_IN();
	IIC_SDA = 1;	Delay_us(1);
	IIC_SCL = 1;	Delay_us(1);
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;
	return 0;
}

/**
  * @brief  等待IIC应答信号
  * @param  None
  * @retval None
  */
uint8_t Check_Ack(void)
{
	uint8_t AckFlag;
	
	SDA_IN();
	IIC_SDA = 1;
	Delay_us(1);
	IIC_SCL = 1;
	Delay_us(3);

	if(READ_SDA)
		AckFlag = 0;
	else
		AckFlag = 1;

	return AckFlag;
}

/**
  * @brief  产生ACK应答
  * @param  None
  * @retval None
  */
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	Delay_us(2);
	IIC_SCL = 1;
	Delay_us(2);
	IIC_SCL = 0;
}

/**
  * @brief  不产生ACK应答
  * @param  None
  * @retval None
  */
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	Delay_us(2);
	IIC_SCL = 1;
	Delay_us(2);
	IIC_SDA = 0;
}

/**
  * @brief  IIC发送1字节数据
  * @param  None
  * @retval None
  */
void IIC_SendByte(uint8_t data)
{
	uint8_t i;
	SDA_OUT();
	IIC_SCL = 0;		//拉低时钟线开始传输数据
	for (i = 0; i < 8; i++)
	{
		IIC_SDA = (data & 0x80) >> 7;
		data <<= 1;
		Delay_us(2);
		IIC_SCL = 1;
		Delay_us(2);
		IIC_SCL = 0;
		Delay_us(2);
	}
}

/**
  * @brief  IIC读取1字节数据
  * @param  ack：  1：产生ACK应答， 0：不产生ACK应答
  * @retval None
  */
uint8_t IIC_ReadByte(uint8_t ack)
{
	uint8_t i, data = 0;
	SDA_IN();
	for (i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		Delay_us(2);
		IIC_SCL = 1;
		data <<= 1;
		if(READ_SDA)
			data++;
		Delay_us(1);
	}
	if(ack)
		IIC_Ack();
	else
		IIC_NAck();
	
	return data;
}


/**
  * @brief  IIC总线读一个字节
  * @param  DeviceAddr：器件地址
  * 		RegAddr：寄存器地址
  * 		Data：数据
  * @retval 
  */
uint8_t IICBusReadOneByte(uint8_t DeviceAddr, uint8_t RegAddr)
{
	uint8_t data;
	
	IIC_Start();						//发送起始信号
	IIC_SendByte(DeviceAddr);		//发送设备地址和读命令
	if(IIC_Wait_Ack())					//等待应答
		return 0;
	
	IIC_SendByte(RegAddr);		//发送寄存器地址
	if(IIC_Wait_Ack())			//等待应答
		return 0;

	IIC_Start();
	IIC_SendByte(DeviceAddr|0x01);		//发送设备地址和读命令
	if(IIC_Wait_Ack())					//等待应答
		return 0;
	
	data = IIC_ReadByte(1);				//读字节，应答
	//data = IIC_ReadByte(0);				//读字节，非应答
	if(IIC_Wait_Ack())					//等待应答
		return 0;
	IIC_Stop();
	return data;
}

/**
  * @brief  IIC总线写一个字节
  * @param  DeviceAddr：器件地址
  * 		RegAddr：寄存器地址
  * 		Data：数据
  * @retval 
  */
uint8_t IICBusWriteOneByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t Data)
{
	IIC_Start();				//发送起始信号
	IIC_SendByte(DeviceAddr);	//发送设备地址
	if(IIC_Wait_Ack())			//等待应答
		return 0;
	
	IIC_SendByte(RegAddr);		//发送寄存器地址
	if(IIC_Wait_Ack())			//等待应答
		return 0;
	
	IIC_SendByte(Data);			//发送数据
	if(IIC_Wait_Ack())			//等待应答
		return 0;

	IIC_Stop();					//发送Stop信号
	return 1;
}


/**
  * @brief  IIC总线写多个字节
  * @param  DeviceAddr：器件地址
  * 		RegAddr：寄存器地址
  * 		*Ptr：数据指针
  * 		Length：长度
  * @retval 
  */
uint8_t IICBusWriteNByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t *Ptr, uint8_t Length)
{
	uint8_t i;

	IIC_Start();						//发送起始信号
	IIC_SendByte(DeviceAddr);		//发送设备地址和读命令
	if(IIC_Wait_Ack())					//等待应答
		return 0;
	
	IIC_SendByte(RegAddr);		//发送寄存器地址
	if(IIC_Wait_Ack())			//等待应答
		return 0;

	for (i = 0; i < Length; i++)
	{
		IIC_SendByte(*Ptr);
		if(IIC_Wait_Ack())			//等待应答
		return 0;
		Ptr++;
	}
	IIC_Stop();
	return 1;	
}



/**
  * @brief  IIC总线读多个字节
  * @param  DeviceAddr：器件地址
  * 		RegAddr：寄存器地址
  * 		*Ptr：数据指针
  * 		Length：写入长度
  * @retval 
  */
uint8_t IICBusReadNByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t *Ptr, uint8_t Length)
{
	//uint8_t data;
	
	IIC_Start();						//发送起始信号
	IIC_SendByte(DeviceAddr);		//发送设备地址和读命令
	if(IIC_Wait_Ack())					//等待应答
		return 0;
	
	IIC_SendByte(RegAddr);		//发送寄存器地址
	if(IIC_Wait_Ack())			//等待应答
		return 0;

	IIC_Start();
	IIC_SendByte(DeviceAddr|0x01);		//发送设备地址和读命令
	if(IIC_Wait_Ack())					//等待应答
		return 0;
	
	for(; Length > 1; Length --)
	{
		*Ptr = IIC_ReadByte(1);				//读字节，应答
		Ptr++;
	}

	*Ptr = IIC_ReadByte(0);				//读字节，非应答
	// if(IIC_Wait_Ack())					//等待应答
	// 	return 0;

	IIC_Stop();

	return 1;
}

