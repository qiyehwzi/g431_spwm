#include "ad7606b.h"
#include "bsp_spi.h"
#include "bsp_dwt.h"

/**
 * @brief       AD7606B��ʼ��
 * @param       ��
 * @retval      ��
 */
void AD7606B_Init(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	__HAL_RCC_GPIOF_CLK_ENABLE();					  	/* ����GPIOFʱ��ʹ�� */
	
	gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;      	/* ������� */
	gpio_init_struct.Pull = GPIO_PULLUP;              	/* ���� */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    	/* ���� */
	HAL_GPIO_Init(GPIOC, &gpio_init_struct);       	  	/* ��ʼ������ */
	
	gpio_init_struct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;      		/* �������� */
	gpio_init_struct.Pull = GPIO_PULLUP;              	/* ���� */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    	/* ���� */
	HAL_GPIO_Init(GPIOC, &gpio_init_struct);       	  	/* ��ʼ������ */

	spi1_init();
	AD7606B_RST_L;
	AD7606B_SER_H;
	AD7606B_CNT_H;
	AD7606B_CS_H;
	AD7606B_WR_H;
	DWT_Delay(0.010);
	
	// ��λ
	AD7606B_RST_H;
	DWT_Delay(0.001);
	AD7606B_RST_L;
	DWT_Delay(0.100);
	
	AD7606B_Read_Reg(0x02);	// �ԼĴ���д�����ǰ��Ҫ������Ĵ���ִ��һ�ζ�ȡ������ʹ��AD7606B����Ĵ�������ģʽ
	AD7606B_Write_Reg(0x02,0x00);//����Ϊ����Doutxģʽ
	
	AD7606B_Write_Reg(0x03,AD7606B_Range_10V);	// CH1-2���뷶Χ���üĴ�������10V
	AD7606B_Write_Reg(0x04,AD7606B_Range_10V);	// CH3-4���뷶Χ���üĴ�������10V
	AD7606B_Write_Reg(0x05,AD7606B_Range_10V);	// CH5-6���뷶Χ���üĴ�������10V
	AD7606B_Write_Reg(0x06,AD7606B_Range_10V);	// CH7-8���뷶Χ���üĴ�������10V
	AD7606B_Write_Reg(0x08,0x00);				// �޹�����
	
	AD7606B_Write_Reg(0x00,0x00);			// SDI��������16���͵�ƽ��AD7606B����ADCģʽ
}


/**
 * @brief       AD7606B����ת��
 * @param       ��
 * @retval      ��
 */
void AD7606B_Start_Convst(void)
{  
	AD7606B_CNT_L;
	AD7606B_CNT_H;
}

/**
 * @brief       AD7606B���Ĵ���
 * @param       Addr���Ĵ�����ַ
 * @retval      �Ĵ����е�����
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
 * @brief       AD7606Bд�Ĵ���
 * @param       Addr���Ĵ�����ַ��Data���Ĵ�������
 * @retval      ��
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
 * @brief       AD7606B���ɼ�����
 * @param       data����ȡ����8��ͨ����ADC����
 * @retval      ��
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

