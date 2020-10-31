/**
	******************************************************************************
	* @file    ePaper213.c 
	* @author  
	* @version 1.0
	* @date    2020-10-18
	* @brief   ePaper2.13inch Driver (4Wire SPI)
	******************************************************************************
	**/ 

/* Includes ------------------------------------------------------------------*/
#include "ePaper213.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const unsigned char EPD2IN13_LUT_FULL_UPDATE[] = 
{
	0x22, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char EPD2IN13_LUT_PARTIAL_UPDATE[] = 
{
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int EPD2IN13Test(void)
{
	uint8_t *BlackImage;
	uint16_t ImageSize;


	EPD2IN13Init(EPD_2IN13_FULL);
	EPD2IN13Clear();

	Delay_ms(500);

	ImageSize = ((EPD_2IN13_WIDTH % 8 == 0) ? (EPD_2IN13_WIDTH / 8) : (EPD_2IN13_WIDTH / 8 + 1)) * EPD_2IN13_HEIGHT;

	if( (BlackImage = (uint8_t *)malloc(ImageSize) ) == NULL)
	{
		printf("E_EPD001:Failed to apply memory! May need to set you heap size!!!\r\n");
		return -1;
	}
	printf("Memory apply OK\r\n");
	Paint_NewImage(BlackImage, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT, 0, WHITE);


#if 0
	/*全刷*/
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);

	
	Paint_DrawBitMap(gImage_Wankel);
	EPD2IN13Display(BlackImage);
	Paint_DrawBitMap(gImage_2in13);
	EPD2IN13Display(BlackImage);
	Delay_ms(2000);
#endif

#if 1

	
	//Select Image
	Paint_Clear(WHITE);
	EPD2IN13Display(BlackImage);

	/*Draw on Image*/
	Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

	Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 10, 135, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

	Paint_DrawLine(45, 15, 45, 55, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(25, 35, 70, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawCircle(45, 35, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(110, 35, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

	Paint_DrawString_EN(50, 15, "OMG", &Font16, BLACK, WHITE);
    Paint_DrawNum(50, 40, 123456789, &Font16, BLACK, WHITE);

	EPD2IN13Display(BlackImage);
#endif

	/*Partial refresh*/

	



	
	return 0;
}

void EPD2IN13Display(uint8_t *Image)
{
	uint16_t Width, Heigth;
	uint16_t X, Y;

	Width = (EPD_2IN13_WIDTH % 8 == 0)? (EPD_2IN13_WIDTH / 8 ): (EPD_2IN13_WIDTH / 8 + 1);
	Heigth = EPD_2IN13_HEIGHT;

	EPD2IN13SetWindows(0, 0, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT);

	for ( Y = 0; Y < Heigth; Y++)
	{
		EPD2IN13SetCursor(0, Y);
		EPD2IN13SendCMD(0x24);

		for ( X = 0; X < Width; X++)
		{
			EPD2IN13SendByte(Image[X+Y*Width]);
		}
	}

	EPD2IN13TurnONDisplay();
}

void EPD2IN13Clear(void)
{
	uint16_t Width, Heigth;
	uint16_t i = 0, j = 0;

	Width = (EPD_2IN13_WIDTH % 8 == 0)? (EPD_2IN13_WIDTH / 8 ): (EPD_2IN13_WIDTH / 8 + 1);
	Heigth = EPD_2IN13_HEIGHT;

	EPD2IN13SetWindows(0, 0, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT);

	for (j = 0; j < Heigth; j++)
	{
		EPD2IN13SetCursor(0, j);
		EPD2IN13SendCMD(0x24);
		for (i = 0; i < Width; i++)
		{
			EPD2IN13SendByte(0xFF);
		}
		
	}	
}




/**
  * @brief  EPD2IN13Init
  * @param  None
  * @retval None
  */
void EPD2IN13Init(uint8_t Mode)
{
	uint8_t i = 0;

	EPD2IN13SPIInit();					//初始化对应的SPI
	EPD2IN13IOInit();					//初始化相应的IO
	
	EPD2IN13Reset();					//复位ePaper

	//设置门的数量
	EPD2IN13SendCMD(0x01);
	EPD2IN13SendByte(0xF9);				//250-1
	EPD2IN13SendByte(0x00);

	EPD2IN13SendByte(0x00);				//GD = 0; SM = 0; TB = 0;

	//BOOSTER_SOFT_START_CONTROL 助力器软启动控制
	EPD2IN13SendCMD(0x0C);
    EPD2IN13SendByte(0xD7);
    EPD2IN13SendByte(0xD6);
    EPD2IN13SendByte(0x9D);

	//WRITE_VCOM_REGISTER			VCOM寄存器
	EPD2IN13SendCMD(0x2C);	
	EPD2IN13SendByte(0xA8);     		// VCOM 7C

	//SET_DUMMY_LINE_PERIOD 虚线周期
	EPD2IN13SendCMD(0x3A);	
	EPD2IN13SendByte(0x1A);				// 4 dummy lines per gate

	//SET_GATE_TIME
	EPD2IN13SendCMD(0x3B);	
	EPD2IN13SendByte(0x08);				// 2us per line

	//BORDER_WAVEFORM_CONTROL 边界波形控制
	EPD2IN13SendCMD(0x3C);	
    EPD2IN13SendByte(0x03);

	//DATA_ENTRY_MODE_SETTING 数据输入模式设置
	EPD2IN13SendCMD(0X11);	
	EPD2IN13SendByte(0x03);				// X increment; Y increment

	//set the look-up table register
	EPD2IN13SendCMD(0x32);

	if(Mode == EPD_2IN13_FULL) 
	{
        for (i = 0; i < 30; i++) 
		{
            EPD2IN13SendByte(EPD2IN13_LUT_FULL_UPDATE[i]);
        }
    } 
	else if(Mode == EPD_2IN13_PART) 
	{
        for (i = 0; i < 30; i++) 
		{
            EPD2IN13SendByte(EPD2IN13_LUT_PARTIAL_UPDATE[i]);
        }
    } 
	else 
	{
        // Debug("error, the Mode is EPD_2IN13_FULL or EPD_2IN13_PART");
    }
}

void EPD2IN13SetWindows(int xStart, int yStart, int xEnd, int yEnd)
{
	EPD2IN13SendCMD(0x44);
	/* x point must be the multiple of 8 or the last 3 bits will be ignored */
	EPD2IN13SendByte((xStart >> 3) & 0xFF);
	EPD2IN13SendByte((xEnd >> 3) & 0xFF);
	EPD2IN13SendCMD(0x45);
	EPD2IN13SendByte(yStart & 0xFF);
	EPD2IN13SendByte((yStart >> 8) & 0xFF);
	EPD2IN13SendByte(yEnd & 0xFF);
	EPD2IN13SendByte((yEnd >> 8) & 0xFF);
}

void EPD2IN13SetCursor(int x, int y)
{
	EPD2IN13SendCMD(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    EPD2IN13SendByte((x >> 3) & 0xFF);
    EPD2IN13SendCMD(0x4F);
    EPD2IN13SendByte(y & 0xFF);
    EPD2IN13SendByte((y >> 8) & 0xFF);

}

void EPD2IN13TurnONDisplay(void)
{
	EPD2IN13SendCMD(0x22); // DISPLAY_UPDATE_CONTROL_2
    EPD2IN13SendByte(0xC4);
    EPD2IN13SendCMD(0X20);	// MASTER_ACTIVATION
    EPD2IN13SendCMD(0xFF);	// TERMINATE_FRAME_READ_WRITE

    EPD2IN13ReadBusy();
}

/**
  * @brief  ePaperIOInit
  * @param  None
  * @retval None
  */
void EPD2IN13IOInit(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(EPD_RCC_Periph, ENABLE);

	GPIO_InitStructure.GPIO_Pin = EPD_RST_PIN | EPD_DC_PIN | EPD_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EPD_RST_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = EPD_BUSY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EPD_RST_PORT, &GPIO_InitStructure);

	//复位拉高		DC 和片选都拉低
	EPD_RST = 1;	EPD_DC = 0; 	EPD_CS = 0;
}

/**
  * @brief  ePaparSPIInit
  * @param  None
  * @retval None
  */
void EPD2IN13SPIInit(void)
{
	SPI1IOInit();
	SPI1Init();
}


/**
  * @brief  EPD2IN13Reset Hardwear Reset
  * @param  None
  * @retval None
  */
void EPD2IN13Reset(void)
{
	EPD_RST = 1;
	Delay_ms(200);
	EPD_RST = 0;
	Delay_ms(10);
	EPD_RST = 1;
	Delay_ms(200);

}

void EPD2IN13ReadBusy(void)
{
	while(1 == EPD_BUSY)
	{
		Delay_ms(10);
	}
}

/**
  * @brief  void EPD2IN13SendCMD(uint8_t Reg)
  * @param  Reg:Command Register
  * @retval None
  */
void EPD2IN13SendCMD(uint8_t Reg)
{
	EPD_DC = 0;							//ePaper写命令
	EPD_CS = 0;							//选中ePaper

	SPI1WriteReadByte(Reg);

	EPD_CS = 1;							//释放ePaper
}

/**
  * @brief  void EPD2IN13SendByte(uint8_t Data)
  * @param 	Data:要写入的数据
  * @retval None
  */
void EPD2IN13SendByte(uint8_t Data)
{
	EPD_DC = 1;							//ePaper写数据
	EPD_CS = 0;							//选中ePaper

	SPI1WriteReadByte(Data);

	EPD_CS = 1;							//释放ePaper
}

/**
  * @brief  void EPD2IN13WriteByte(uint8_t Reg, uint8_t Data)
  * @param  Reg:Command Register
  * 		Data:Data to Write
  * @retval None
  */
void EPD2IN13WriteByte(uint8_t Reg, uint8_t Data)
{
	EPD2IN13SendCMD(Reg);
	EPD2IN13SendByte(Data);
}


/**
  * @brief  void EPD2IN13Write2Byte(uint8_t Reg, uint16_t Data)
  * @param  Reg:Command Register
  * 		Data:Data to Write
  * @retval None
  */
void EPD2IN13Write2Byte(uint8_t Reg, uint16_t Data)
{
	EPD2IN13SendCMD(Reg);
	EPD2IN13SendByte((Data >> 8) & 0xFF);
	EPD2IN13SendByte(Data & 0xFF);
}











