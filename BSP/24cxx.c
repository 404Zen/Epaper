#include "24cxx.h" 
#include "../SYSTEM/SysTick.h"


/**
  * @brief  24CXX读取1字节数据
  * @param  ReadAddr:读取地址
  * @retval None
  */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_SendByte(0XA0);			//发送器件地址
		IIC_Wait_Ack();
		IIC_SendByte(ReadAddr>>8);	//发送寄存器地址
		IIC_Wait_Ack();		 
	}else IIC_SendByte(0XA0+((ReadAddr/256)<<1));		//

	IIC_Wait_Ack(); 
    IIC_SendByte(ReadAddr%256);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_SendByte(0XA1);           //½øÈë½ÓÊÕÄ£Ê½			   
	IIC_Wait_Ack();	 
    temp=IIC_ReadByte(0);		   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ	    
	return temp;
}
/**
  * @brief  24CXX写入1字节数据
  * @param  WriteAddr:写入地址
  * 		DataToWrite：写入数据
  * @retval None
  */
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_SendByte(0XA0);	    //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		IIC_SendByte(WriteAddr>>8);//·¢ËÍ¸ßµØÖ·
 	}else
	{
		IIC_SendByte(0XA0+((WriteAddr/256)<<1));   //·¢ËÍÆ÷¼þµØÖ·0XA0,Ð´Êý¾Ý 
	}	 
	IIC_Wait_Ack();	   
    IIC_SendByte(WriteAddr%256);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack(); 	 										  		   
	IIC_SendByte(DataToWrite);     //·¢ËÍ×Ö½Ú							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ 
	Delay_ms(10);	 
}

/**
  * @brief  24CXX写入多字节数据
  * @param  WriteAddr:写入地址
  * 		DataToWrite：写入数据
  * 		Len：写入长度
  * @retval None
  */
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

/**
  * @brief  24CXX读取多字节数据
  * @param  ReadAddr:写入地址
  * 		Len：读出长度
  * @retval None
  */
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
/**
  * @brief  24CXX自测试函数
  * @param  None
  * @retval None
  */
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//±ÜÃâÃ¿´Î¿ª»ú¶¼Ð´AT24CXX			   
	if(temp==0X55)return 0;		   
	else//ÅÅ³ýµÚÒ»´Î³õÊ¼»¯µÄÇé¿ö
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

/**
  * @brief  24CXX读取函数
  * @param  None
  * @retval None
  */
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
/**
  * @brief  24CXX写入函数
  * @param  None
  * @retval None
  */
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 








