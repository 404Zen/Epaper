/**
  ******************************************************************************
  * @file    eClock.c 
  * @author  Zen
  * @version 1.0
  * @date    2020-10-31
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/
#include "eClock.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t 	*BGImage;					//存放背景图片的内存
uint16_t	ImgSize = 4000;				//250*122分辨率 需要 4000Byte


static uint8_t eClcokStatus = 0;		//
#define ECLOCK_INIT			0x00		
#define ECLOCK_INIT_DONE	0x01		//Show Logo

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int8_t eClockInit(void)
{
	DEV_Module_Init();

	EPD_2IN13_Init(EPD_2IN13_FULL);
	// EPD_2IN13_Clear();

	DEV_Delay_ms(500);
		
	/* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
	/* 需要在 startup_stm32fxxx.s 中设置足够大的 heap size */
	if((BGImage = (uint8_t *)malloc(ImgSize)) == NULL) 
	{
		printf("Failed to apply for black memory...\r\n");
		return -1;
	}
	Paint_NewImage(BGImage, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT, 270, WHITE);
	printf("Apply for Memory OK!...\r\n");

	//Show StartUp logo
	printf("Show start up logo!...\r\n");
	Paint_SelectImage(BGImage);
	Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_Wankel);
	EPD_2IN13_Display(BGImage);
	DEV_Delay_ms(500);

	//Page Layout
	Paint_Clear(WHITE);				
	Paint_DrawLine(180, 0, 180, 122, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(181, 41, 250, 41, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(181, 82, 250, 82, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	// Paint_DrawString_EN(20, 0, "ePaper Clock", &Font16, WHITE, BLACK);
	EPD_2IN13_Display(BGImage);

	//Config to Partial refresh
	EPD_2IN13_Init(EPD_2IN13_PART);					//配置为局部刷新
	
	eClcokStatus = ECLOCK_INIT_DONE;
	return 0;
}


void eClockRefresh(void)
{
	uint8_t Value[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	uint8_t Dx = 0;

	uint8_t XStart = 10;
	uint8_t YStart = 20;

	if(ECLOCK_INIT_DONE == eClcokStatus)
	{
		Dx = Font36.Width;
		Paint_ClearWindows(XStart, YStart, XStart+Font36.Width * 7, YStart+Font36.Height, WHITE);
		Paint_DrawChar(XStart		, YStart, Value[Calendar.Hour/10]	, &Font36, BLACK, WHITE);
		Paint_DrawChar(XStart+Dx	, YStart, Value[Calendar.Hour%10]	, &Font36, BLACK, WHITE);
		Paint_DrawChar(XStart+Dx+Dx	, YStart, ' ' 						, &Font36, BLACK, WHITE);
		Paint_DrawChar(XStart+Dx*3	, YStart, Value[Calendar.Minute/10]	, &Font36, BLACK, WHITE);
		Paint_DrawChar(XStart+Dx*4	, YStart, Value[Calendar.Minute%10]	, &Font36, BLACK, WHITE);
		Paint_DrawChar(XStart+Dx*5	, YStart, Value[Calendar.Second/10]	, &Font36, BLACK, WHITE);
		Paint_DrawChar(XStart+Dx*6	, YStart, Value[Calendar.Second%10]	, &Font36, BLACK, WHITE);

		EPD_2IN13_Display(BGImage);
	}
	else
	{
		;
	}
}

