/**
  ******************************************************************************
  * @file    oled.c 
  * @author  Zen
  * @version 1.0
  * @date    2020-06-14
  * @brief   OLED Driver C Source File
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/
#include "oled.h"
#include "oledfont.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  us延迟函数
  * @param  None
  * @retval None
  */
static void delay(unsigned char num)
{
	uint8_t i = 10;
	while(num--)
	{
		while(i--);
	}
}

/**
  * @brief  ms延迟函数
  * @param  None
  * @retval None
  */
// static void delay_ms(unsigned int ms)//延迟函数，MS级别
// {
// 	unsigned int x,y;
// 	for(x = ms;x>0;x--)
//  {
// 		for(y = 12000;y>0;y--);
//  }
// }

/**
  * @brief  OLED_GPIO_Init
  * @param  None
  * @retval None
  */
static void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;										//定义一个GPIO_InitTypeDef类型的结构体
	
	RCC_APB2PeriphClockCmd(OLED_SCK_PERIPH | OLED_SDA_PERIPH, ENABLE);			//打开GPIO的外设时钟
	
	GPIO_InitStructure.GPIO_Pin = OLED_SCK_PIN;				//选择控制的引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//设置为通用开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设置输出速率
	GPIO_Init(OLED_SCK_PORT,&GPIO_InitStructure);			//调用库函数初始化GPIO

	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;			//选择控制的引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//设置为通用开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置输出速率
	GPIO_Init(OLED_SDA_PORT,&GPIO_InitStructure);		//调用库函数初始化GPIO
	
	OLED_SCLK_Set();	//设PC2（SCL）为高电平
	OLED_SDIN_Set();	//设PC3（SDA）为高电平
}

/**
  * @brief  OLED_IIC_Start
  * @param  None
  * @retval None
  */
static void OLED_IIC_Start(void)
{

	OLED_SCLK_Set();	//时钟线置高
	OLED_SDIN_Set();	//信号线置高
	delay(1);	//延迟1us
	OLED_SDIN_Clr();	//信号线置低
	delay(1);	//延迟1us
	OLED_SCLK_Clr();	//时钟线置低
	delay(1);	//延迟1us
}

/**
  * @brief  OLED_IIC_Stop
  * @param  None
  * @retval None
  */
static void OLED_IIC_Stop(void)
{
	OLED_SDIN_Clr();	//信号线置低
	delay(1);	//延迟1us
	OLED_SCLK_Set();	//时钟线置高
	delay(1);	//延迟1us
	OLED_SDIN_Set();	//信号线置高
	delay(1);	//延迟1us
}

/**
  * @brief  IIC_Wait_Ack
  * @param  None
  * @retval None
  */
static unsigned char IIC_Wait_Ack(void)
{
	unsigned char ack;

	OLED_SCLK_Clr();	//时钟线置低
	delay(1);	//延迟1us
	OLED_SDIN_Set();	//信号线置高
	delay(1);	//延迟1us
	OLED_SCLK_Set();	//时钟线置高
	delay(1);	//延迟1us

	if(OLED_READ_SDIN())	//读取SDA的电平
		ack = IIC_NO_ACK;	//如果为1，则从机没有应答
	else
		ack = IIC_ACK;		//如果为0，则从机应答

	OLED_SCLK_Clr();//时钟线置低
	delay(1);	//延迟1us

	return ack;	//返回读取到的应答信息
}

/**
  * @brief  Write_IIC_Byte
  * @param  None
  * @retval None
  */
static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;  //定义变量
	for(i=0;i<8;i++) //for循环8次
	{
		OLED_SCLK_Clr();	//时钟线置低，为传输数据做准备
		delay(1);	//延迟1us

		if(IIC_Byte & 0x80)	//读取最高位
		  	OLED_SDIN_Set();//最高位为1
		else
			OLED_SDIN_Clr();	//最高位为0

		IIC_Byte <<= 1;  //数据左移1位
		delay(1);	//延迟1us
		OLED_SCLK_Set(); //时钟线置高，产生上升沿，把数据发送出去
		delay(1);	//延迟1us
	}
	OLED_SCLK_Clr();	//时钟线置低
	delay(1);	//延迟1us

	while(IIC_Wait_Ack());	//从机应答
}

/**
  * @brief  Write_IIC_Command
  * @param  None
  * @retval None
  */
static void Write_IIC_Command(unsigned char IIC_Command)
{
   OLED_IIC_Start();
   Write_IIC_Byte(0x78);//写入从机地址，SD0 = 0
   Write_IIC_Byte(0x00);//写入命令
   Write_IIC_Byte(IIC_Command);//数据
   OLED_IIC_Stop();  //发送停止信号
}

/**
  * @brief  Write_IIC_Data
  * @param  None
  * @retval None
  */
static void Write_IIC_Data(unsigned char IIC_Data)
{
   OLED_IIC_Start();
   Write_IIC_Byte(0x78);	//写入从机地址，SD0 = 0
   Write_IIC_Byte(0x40);	//写入数据
   Write_IIC_Byte(IIC_Data);//数据
   OLED_IIC_Stop();		//发送停止信号
}

/**
  * @brief  OLED_WR_Byte
  * @param  None
  * @retval None
  */
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
	if(cmd) 
	  {
       Write_IIC_Data(dat); //写入数据
	  }
	else {
       Write_IIC_Command(dat); //写入命令
	}
}

/**
  * @brief  OLED_Set_Pos
  * @param  None
  * @retval None
  */
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);//写入页地址
	OLED_WR_Byte((x&0x0f),OLED_CMD);  //写入列的地址  低半字节
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);//写入列的地址 高半字节
} 

/**
  * @brief  OLED_Display_On
  * @param  None
  * @retval None
  */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //设置OLED电荷泵
	OLED_WR_Byte(0X14,OLED_CMD);  //使能，开
	OLED_WR_Byte(0XAF,OLED_CMD);  //开显示
}


/**
  * @brief  OLED_Display_Off
  * @param  None
  * @retval None
  */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0XAE,OLED_CMD);  //关显示
	OLED_WR_Byte(0X8D,OLED_CMD);  //设置OLED电荷泵
	OLED_WR_Byte(0X10,OLED_CMD);  //失能，关
}	

/**
  * @brief  OLED_Display_Off
  * @param  None
  * @retval None
  */
void OLED_Clear(void)  
{  
	unsigned char i,n;		    //定义变量
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //从0~7页依次写入
		OLED_WR_Byte (0x00,OLED_CMD);      //列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //列高地址  
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); //写入 0 清屏
	}
}

/**
  * @brief  OLED_ShowChar
  * @param  None
  * @retval None
  */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' '; //获取字符的偏移量	
		if(x>Max_Column-1){x=0;y=y+2;} //如果列数超出了范围，就从下2页的第0列开始

		if(SIZE ==16) //字符大小如果为 16 = 8*16
			{
				OLED_Set_Pos(x,y);	//从x y 开始画点
				for(i=0;i<8;i++)  //循环8次 占8列
				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); //找出字符 c 的数组位置，先在第一页把列画完
				OLED_Set_Pos(x,y+1); //页数加1
				for(i=0;i<8;i++)  //循环8次
				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA); //把第二页的列数画完
			}
		else 	//字符大小为 6 = 6*8
			{	
				OLED_Set_Pos(x,y+1); //一页就可以画完
				for(i=0;i<6;i++) //循环6次 ，占6列
				OLED_WR_Byte(F6x8[c][i],OLED_DATA); //把字符画完
			}
}
/**
  * @brief  oled_pow
  * @param  None
  * @retval None
  */
unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
/**
  * @brief  OLED_ShowNum
  * @param  None
  * @retval None
  */
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size)
{         	
	unsigned char t,temp;  //定义变量
	unsigned char enshow=0;		//定义变量

	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;//取出输入数的每个位，由高到低
		if(enshow==0&&t<(len-1)) //enshow：是否为第一个数；t<(len-1)：判断是否为最后一个数
		{
			if(temp==0) //如果该数为0 
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');//显示 0 ；x+(size2/2)*t根据字体大小偏移的列数（8）
				continue; //跳过剩下语句，继续重复循环（避免重复显示）
			}else enshow=1; 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); //显示一个位；x+(size2/2)*t根据字体大小偏移的列数（8）
	}
} 

/**
  * @brief  OLED_ShowString
  * @param  None
  * @retval None
  */
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr)
{
	unsigned char j=0; //定义变量

	while (chr[j]!='\0') //如果不是最后一个字符
	{		
		OLED_ShowChar(x,y,chr[j]); //显示字符
		x+=8; //列数加8 ，一个字符的列数占8
		if(x>=128){x=0;y+=2;} //如果x大于等于128，切换页，从该页的第一列显示
		j++; //下一个字符
	}
}

/**
  * @brief  OLED_Init
  * @param  None
  * @retval None
  */
void OLED_Init(void)
{
	OLED_GPIO_Init();	//GPIO口初始化
 
	Delay_ms(200);	//延迟，由于单片机上电初始化比OLED快，所以必须加上延迟，等待OLED上复位完成

	OLED_WR_Byte(0xAE,OLED_CMD);	//关闭显示

	OLED_WR_Byte(0x00,OLED_CMD);	//设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);	//设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);	//设置起始行地址
	OLED_WR_Byte(0xB0,OLED_CMD);	//设置页地址

	OLED_WR_Byte(0x81,OLED_CMD); 	// 对比度设置，可设置亮度
	OLED_WR_Byte(0xFF,OLED_CMD);	//  265  

	OLED_WR_Byte(0xA1,OLED_CMD);	//设置段（SEG）的起始映射地址；column的127地址是SEG0的地址
	OLED_WR_Byte(0xA6,OLED_CMD);	//正常显示；0xa7逆显示

	OLED_WR_Byte(0xA8,OLED_CMD);	//设置驱动路数（16~64）
	OLED_WR_Byte(0x3F,OLED_CMD);	//64duty
	
	OLED_WR_Byte(0xC8,OLED_CMD);	//重映射模式，COM[N-1]~COM0扫描

	OLED_WR_Byte(0xD3,OLED_CMD);	//设置显示偏移
	OLED_WR_Byte(0x00,OLED_CMD);	//无偏移
	
	OLED_WR_Byte(0xD5,OLED_CMD);	//设置震荡器分频
	OLED_WR_Byte(0x80,OLED_CMD);	//使用默认值
	
	OLED_WR_Byte(0xD9,OLED_CMD);	//设置 Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);	//使用官方推荐值
	
	OLED_WR_Byte(0xDA,OLED_CMD);	//设置 com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);	//使用默认值
	
	OLED_WR_Byte(0xDB,OLED_CMD);	//设置 Vcomh，可调节亮度（默认）
	OLED_WR_Byte(0x40,OLED_CMD);	////使用官方推荐值
	
	OLED_WR_Byte(0x8D,OLED_CMD);	//设置OLED电荷泵
	OLED_WR_Byte(0x14,OLED_CMD);	//开显示
	
	OLED_WR_Byte(0xAF,OLED_CMD);	//开启OLED面板显示
	OLED_Clear();        //清屏
	OLED_Set_Pos(0,0); 	 //设置数据写入的起始行、列
}  

/**
  * @brief  OLED_Scroll
  * @param  None
  * @retval None
  */
void OLED_Scroll(void)
{
	OLED_WR_Byte(0x2E,OLED_CMD);	//关闭滚动
	OLED_WR_Byte(0x27,OLED_CMD);	//水平向左滚动
	OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);	//起始页 0
	OLED_WR_Byte(0x00,OLED_CMD);	//滚动时间间隔
	OLED_WR_Byte(0x01,OLED_CMD);	//终止页 1
	OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
	OLED_WR_Byte(0xFF,OLED_CMD);	//虚拟字节
	OLED_WR_Byte(0x2F,OLED_CMD);	//开启滚动
}
/**********************************END OF FILE**********************************/
