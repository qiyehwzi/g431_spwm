#include "ad7606b.h"
#include "bsp_spi.h"
#include "bsp_dwt.h"

/**
 * @brief       AD7606B初始化
 * @param       无
 * @retval      无
 */
void AD7606B_Init(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	__HAL_RCC_GPIOF_CLK_ENABLE();					  	/* 开启GPIOF时钟使能 */
	
	gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;      	/* 推挽输出 */
	gpio_init_struct.Pull = GPIO_PULLUP;              	/* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    	/* 高速 */
	HAL_GPIO_Init(GPIOC, &gpio_init_struct);       	  	/* 初始化引脚 */
	
	gpio_init_struct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;      		/* 推挽输入 */
	gpio_init_struct.Pull = GPIO_PULLUP;              	/* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    	/* 高速 */
	HAL_GPIO_Init(GPIOC, &gpio_init_struct);       	  	/* 初始化引脚 */

	spi1_init();
	AD7606B_RST_L;
	AD7606B_SER_H;
	AD7606B_CNT_H;
	AD7606B_CS_H;
	AD7606B_WR_H;
	DWT_Delay(0.010);
	
	// 复位
	AD7606B_RST_H;
	DWT_Delay(0.001);
	AD7606B_RST_L;
	DWT_Delay(0.100);
	
	AD7606B_Read_Reg(0x02);	// 对寄存器写入操作前需要对任意寄存器执行一次读取操作，使得AD7606B进入寄存器操作模式
	AD7606B_Write_Reg(0x02,0x00);//配置为单线Doutx模式
	
	AD7606B_Write_Reg(0x03,AD7606B_Range_10V);	// CH1-2输入范围设置寄存器，±10V
	AD7606B_Write_Reg(0x04,AD7606B_Range_10V);	// CH3-4输入范围设置寄存器，±10V
	AD7606B_Write_Reg(0x05,AD7606B_Range_10V);	// CH5-6输入范围设置寄存器，±10V
	AD7606B_Write_Reg(0x06,AD7606B_Range_10V);	// CH7-8输入范围设置寄存器，±10V
	AD7606B_Write_Reg(0x08,0x00);				// 无过采样
	
	AD7606B_Write_Reg(0x00,0x00);			// SDI连续输入16个低电平，AD7606B进入ADC模式
}


/**
 * @brief       AD7606B启动转换
 * @param       无
 * @retval      无
 */
void AD7606B_Start_Convst(void)
{  
	AD7606B_CNT_L;
	AD7606B_CNT_H;
}

/**
 * @brief       AD7606B读寄存器
 * @param       Addr：寄存器地址
 * @retval      寄存器中的数据
 */
uint8_t AD7606B_Read_Reg(uint8_t Addr)
{
	uint16_t RxData = 0;
	uint16_t TxData = 0;
	TxData = 0x40 + Addr;
	TxData = TxData << 8;
	
	AD7606B_CS_L;
	RxData = spi1_read_write_16bit(TxData);
	AD7606B_CS_H;
	
	return (uint8_t)RxData;
}

/**
 * @brief       AD7606B写寄存器
 * @param       Addr：寄存器地址，Data：寄存器数据
 * @retval      无
 */
void AD7606B_Write_Reg(uint8_t Addr, uint8_t Data)
{
	uint16_t TxData = 0;
	TxData = ((uint16_t)Addr << 8) | ((uint16_t)Data);
	
	AD7606B_CS_L;
	spi1_read_write_16bit(TxData);
	AD7606B_CS_H;
}



/**
 * @brief       AD7606B读采集数据
 * @param       data：读取到的8个通道的ADC数据
 * @retval      无
 */
void AD7606B_Read_AD_Data(int16_t * data)
{
	uint8_t i;
	uint16_t TxData[8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
   
	AD7606B_CS_L;
	for(i=0; i<8; i++)
	{
		data[i] = spi1_read_write_16bit(TxData[i]);
	}
	AD7606B_CS_H;
}

