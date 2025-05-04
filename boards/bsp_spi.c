#include "bsp_spi.h"
#include "bsp_dwt.h"

/**
 * @brief       SPI1初始化
 * @param       无
 * @retval      无
 */
void spi1_init(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	__HAL_RCC_GPIOA_CLK_ENABLE();					  	/* 开启GPIOF时钟使能 */
	
	gpio_init_struct.Pin = GPIO_PIN_1|GPIO_PIN_3;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;      	/* 推挽输出 */
	gpio_init_struct.Pull = GPIO_PULLUP;              	/* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    	/* 高速 */
	HAL_GPIO_Init(GPIOC, &gpio_init_struct);       	  	/* 初始化LED0引脚 */
	
	gpio_init_struct.Pin = GPIO_PIN_2;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;      	  	/* 推挽输出 */
	gpio_init_struct.Pull = GPIO_PULLUP;              	/* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    	/* 高速 */
	HAL_GPIO_Init(GPIOC, &gpio_init_struct);       	  	/* 初始化LED0引脚 */
}   

/**
 * @brief       SPI1读写一个16位数据
 * @param       txdata  : 要发送的数据(2字节)
 * @retval      接收到的数据(2字节)
 */
uint16_t spi1_read_write_16bit(uint16_t data)
{
	uint8_t i;
    uint16_t rxdata = 0;
	uint16_t txdata = data;
	
	for(i=0; i<16; i++)
	{		
		rxdata <<= 1;
		if(txdata&0x8000)
			AD7606B_SDI_H;
		else
			AD7606B_SDI_L;
		AD7606B_SCLK_L;	
		txdata <<= 1;
		
//		DWT_Delay(0.0001);
		
		if(AD7606B_SDO)
			rxdata++;
		AD7606B_SCLK_H;
		
//		DWT_Delay(0.0001);
	}
    return rxdata; /* 返回收到的数据 */
}

