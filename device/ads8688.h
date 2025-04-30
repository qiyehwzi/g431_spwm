//#ifndef __ADS8688_H__
//#define __ADS8688_H__

//#include "stm32g4xx_hal.h"

//#define NO_OP 0X0000 
//#define STDBY 0X8200 
//#define PWR_DN 0X8300 
//#define RST 0X8500 
//#define AUTO_RST 0XA000 
//#define MAN_Ch_1 0XC000 
//#define MAN_Ch_2 0XC400 
//#define MAN_Ch_3 0XC800 
//#define MAN_Ch_4 0XCC00 
//#define MAN_Ch_5 0XD000 
//#define MAN_Ch_6 0XD400 
//#define MAN_Ch_7 0XD800 
//#define MAN_Ch_8 0XDC00 
//#define MAN_AUX 0XE000 
//#define CH1 0x05 
//#define CH2 0x06 
//#define CH3 0x07 
//#define CH4 0x08 
//#define CH5 0x09 
//#define CH6 0x0a 
//#define CH7 0x0b 
//#define CH8 0x0c 

//// 引脚定义
//#define ADS8688_CS_PORT    GPIOB
//#define ADS8688_CS_PIN     GPIO_PIN_4
//#define ADS8688_SCK_PORT   GPIOB
//#define ADS8688_SCK_PIN    GPIO_PIN_5
//#define ADS8688_MOSI_PORT  GPIOB
//#define ADS8688_MOSI_PIN   GPIO_PIN_6
//#define ADS8688_MISO_PORT  GPIOB
//#define ADS8688_MISO_PIN   GPIO_PIN_7
//#define ADS_8688_DAISY_IN_Pin GPIO_PIN_9
//#define ADS_8688_DAISY_IN_GPIO_PORT GPIOC
//#define ADS_8688_RST_PD_Pin GPIO_PIN_8
//#define ADS_8688_RST_PD_GPIO_PORT GPIOC

//extern uint16_t My_Ad[8];
//extern float ad_real[2];

//void ADS8688_Init_Single(void); 
//uint16_t get_ADS_ch1(void); 
//void ADS8688_Init_Mult(void); 
//double get_realdata(uint16_t x,uint8_t t);
//void gat_real_ad(float *real_ad);
//void get_ADS_1and2(uint16_t *My_Ad,float *real_ad);
//void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum); 
//void AUTO_RST_Mode(void); 
//void ADS8688_SPI_WB(uint8_t com) ;
//uint8_t ADS8688_SPI_RB(void) ;
//void ADS8688_WriteCommandReg(uint16_t command);//写ADS8688命令寄存器 
//void ADS8688_Write_Program_Register(uint8_t Addr,uint8_t data);
//uint8_t ADS8688_READ_Program_Register(uint8_t Addr);
//void Enter_RESET_MODE(void);//软件复位模式，复位 program registers
//void Set_CH_Range_Select(uint8_t ch,uint8_t range); //设置各个通道的范围 
//uint16_t Get_MAN_Ch_n_Mode_Data(void);//直接获取一次通道值

//#endif
