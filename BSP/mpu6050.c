/**
  ******************************************************************************
  * @file    mpu6050.c 
  * @author  Zen
  * @version 1.0
  * @date    2020-05-14
  * @brief   MPU6050 Driver (IO模拟)
  **************************************c****************************************
  **/ 
/* Includes ------------------------------------------------------------------*/
#include "mpu6050.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
                                          

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  MPU6050初始化
  * @param  None
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_Init(void)
{
    IIC_Init();                         //IIC初始化

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x80);             //复位Sensor
    Delay_ms(100);                  

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x00);                                 //Enable Temp, Internal Oscillator
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, MPU6050_GYRO_2000DPS);                //Gyroscope Scale Range Select
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_2G);                   //Accelerometer Scale Range Select

    MPU6050_WriteReg(MPU6050_CONFIG, 0x04);                 //GYRO Fs = 1KHz
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x13);             //Sample Rate = 50Hz    Smaple Rate = GYRO Fs / (1 + SMPLRT_DIV)

    MPU6050_WriteReg(MPU6050_INT_ENABLE, 0x00);             //Disable all interrupt
    MPU6050_WriteReg(MPU6050_USER_CTRL, 0x00);              //Disable IIC master mode
    MPU6050_WriteReg(MPU6050_FIFO_EN, 0x00);                //Disable FIFO
    MPU6050_WriteReg(MPU6050_INT_PIN_CFG, 0x80);            //Interrupt Pin Low ACTIVE
    if(1 == MPU6050_ConnectTest())
    {
        MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);         //PLL with X axis gyroscope reference
        MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);         //6-Axis is work
    }
    else
    {
        return 0;
    }

    return 1;
}

/**
  * @brief  MPU6050设置陀螺仪量程范围
  * @param  Range:MPU6050_GYRO_250DPS, MPU6050_GYRO_500DPS, MPU6050_GYRO_1000DPS, MPU6050_GYRO_2000DPS
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_SetGyroRange(uint8_t Range)
{
	return MPU6050_WriteReg(MPU6050_GYRO_CONFIG, Range);
}


/**
  * @brief  MPU6050设置加速度量程范围
  * @param  Range:MPU6050_ACCEL_2G, MPU6050_ACCEL_4G, MPU6050_ACCEL_8G, MPU6050_ACCEL_16G
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_SetAccelRange(uint8_t Range)
{
	return MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, Range);
}

/**
  * @brief  MPU6050设置低通滤波器带宽
  * @param  BW:5~256KHz
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_SetDLPF(uint16_t BW)
{
	uint8_t DLPFValue;
	
	if(BW >= 256)		DLPFValue = MPU6050_DLPF_256KHz;
	else if(BW >= 188)	DLPFValue = MPU6050_DLPF_188KHz;
	else if(BW >= 98)	DLPFValue = MPU6050_DLPF_98KHz;
	else if(BW >= 42)	DLPFValue = MPU6050_DLPF_42KHz;
	else if(BW >= 20)	DLPFValue = MPU6050_DLPF_20KHz;
	else if(BW >= 10)	DLPFValue = MPU6050_DLPF_10KHz;
	else				DLPFValue = MPU6050_DLPF_5KHz;

	return MPU6050_WriteReg(MPU6050_CONFIG, DLPFValue);	
}

/**
  * @brief  MPU6050设置采样速率
  * @param  Rate:4~1000KHz
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_SetSamplingRate(uint16_t Rate)
{
	uint8_t Value = 0;
	// Rate = Rate > 1000 ? 1000 : Rate;
	if(Rate > 1000)
		Rate = 1000;
	else if (Rate < 4)
		Rate = 4;


	Value = (1000/Rate) - 1;
	
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, Value);

	return MPU6050_SetDLPF(Rate/2);
}

/**
  * @brief  MPU6050获取温度数据
  * @param  None
  * @retval 温度数据 单位为0.01摄氏度
  */
uint16_t MPU6050_GetTemperature(void)
{
	uint8_t TempBuffer[2];
	uint16_t TempValue;
	float Temperature;

	MPU6050_ReadNReg(MPU6050_TEMP_OUT_H, 2, TempBuffer);
	TempValue = ((uint16_t)(TempBuffer[0] << 8) | TempBuffer[1]);
	// TempValue |= TempBuffer[1];
	Temperature = 36.53 + ((float)TempValue / 340.0);
	
	return (uint16_t)(Temperature*100);
}

/**
  * @brief  MPU6050获取陀螺仪数据
  * @param  XYZ
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_GetGyroData(uint16_t *GyroX, uint16_t *GyroY, uint16_t *GyroZ)
{
	uint8_t TempBuffer[6];
	uint8_t Result;
	Result = MPU6050_ReadNReg(MPU6050_GYRO_XOUT_H, 6, TempBuffer);
	if(1 == Result)
	{
		*GyroX = ((uint16_t)(TempBuffer[0] << 8) | TempBuffer[1]);
		*GyroY = ((uint16_t)(TempBuffer[2] << 8) | TempBuffer[3]);
		*GyroZ = ((uint16_t)(TempBuffer[4] << 8) | TempBuffer[5]);
	}
	
	return Result;
}

/**
  * @brief  MPU6050获取加速度数据
  * @param  XYZ
  * @retval 1:Success else:Fail
  */
uint8_t MPU6050_GetAccelData(uint16_t *AccelX, uint16_t *AccelY, uint16_t *AccelZ)
{
	uint8_t TempBuffer[6];
	uint8_t Result;
	Result = MPU6050_ReadNReg(MPU6050_ACCEL_XOUT_H, 6, TempBuffer);
	if(1 == Result)
	{
		*AccelX = ((uint16_t)(TempBuffer[0] << 8) | TempBuffer[1]);
		*AccelY = ((uint16_t)(TempBuffer[2] << 8) | TempBuffer[3]);
		*AccelZ = ((uint16_t)(TempBuffer[4] << 8) | TempBuffer[5]);
	}
	
	return Result;
}


/**
  * @brief  验证MPU6050硬件连接性
  * @param  None
  * @retval 1:Success   0:Fail
  */
uint8_t MPU6050_ConnectTest(void)
{
    uint8_t RegVal = 0;
    RegVal = MPU6050_ReadReg(0x75);

	#if 1
//	printf("The Regval of 0x75 Is:%#x; MPU6050_ConnectTest()\r\n",RegVal);
    #endif

    return((0x68 == RegVal)? 1 : 0);
}



/**
  * @brief  读MPU6050单个寄存器
  * @param  RegAddr:寄存器地址
  * @retval RegVal:读到的寄存器的值
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
    uint8_t RegVal;

    RegVal = IICBusReadOneByte(MPU6050_ADDR, RegAddr);

    return RegVal;
}

/**
  * @brief  读MPU6050多个寄存器
  * @param  RegAddr:寄存器地址
  *         Length: 读写长度
  *         Ptr:    数据指针
  * @retval 1:Success 0:Fail
  */
uint8_t MPU6050_ReadNReg(uint8_t RegAddr, uint8_t Length, uint8_t *Ptr)
{
    return(IICBusReadNByte(MPU6050_ADDR, RegAddr, Ptr, Length));
}

/**
  * @brief  MPU6050写一个寄存器
  * @param  RegAddr:寄存器地址
  *         Data:    数据
  * @retval 1:Success 0:Fail
  */
uint8_t MPU6050_WriteReg(uint8_t RegAddr, uint8_t Data)
{
    return(IICBusWriteOneByte(MPU6050_ADDR, RegAddr, Data));
}

/**
  * @brief  MPU6050写一个寄存器
  * @param  RegAddr:寄存器地址
  *         Data:    数据
  * @retval None
  */
void MPU6050_WriteRegBit(uint8_t RegAddr, uint8_t BitNum, uint8_t Value)
{
    uint8_t RegVal = 0;

    RegVal = MPU6050_ReadReg(RegAddr);
    RegVal = (0 == Value) ? (RegVal & ~(1 << BitNum)) : (RegVal | (1 << BitNum));
    MPU6050_WriteReg(RegAddr, RegVal);
}


















