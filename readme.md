***IO Definitions***
	EPaper(IL3895)	<-->		STM32F103C8T6
	DIN				<-->		PA7			SPI MOSI
	CLK				<-->		PA5			SPI Clock Pin
	CS				<-->		PA4			CS Enable Low
	DC()			<-->		PA2			Data/Cmd Control, H=Data L=Command
	RST				<-->		PA1			RST is active low
	BUSY			<-->		PA3			Busy state output pin, When BUSY is low, the chip is busy


***中断优先级配置***
	中断优先级分组2

    NVIC_IRQChannel	|PreemptionPriority	|SubPriority
	
***进度表***

    1.驱动屏幕完成
    2.
    

***已知问题***
    
    1.
