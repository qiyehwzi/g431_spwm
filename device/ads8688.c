//#include "ads8688.h"
//#include "main.h"
//#include "cmsis_os.h"
//#include "bsp_dwt.h"
//#include "gpio.h"
//#include "stdint.h"

//uint16_t My_Ad[8]={0,0,};
//float ad_real[2]={0.0,0.0};

////#define DWT_CYCCNT  *(volatile uint32_t*)0xE0001004
////#define DWT_CONTROL *(volatile uint32_t*)0xE0001000
////#define SCB_DEMCR   *(volatile uint32_t*)0xE000EDFC

//static void CS_Enable(void) {
//    HAL_GPIO_WritePin(ADS8688_CS_PORT, ADS8688_CS_PIN, GPIO_PIN_RESET);
//    DWT_Delay(0.000001);
//}

//static void CS_Disable(void) {
//    DWT_Delay(0.000001);
//    HAL_GPIO_WritePin(ADS8688_CS_PORT, ADS8688_CS_PIN, GPIO_PIN_SET);
//}

//void ADS8688_SPI_WB(uint8_t com) 
//{
//	uint8_t com_temp=com,s;
//	CS_Enable();
//	for(s=0;s<8;s++)
//	{
//		if(com_temp&0x80)
//		{
//			CS_Disable(); 
//		}
//		else 
//		{ 
//			CS_Enable();
//		}
//	CS_Disable();
//	com_temp<<=1; 
//	CS_Enable(); 
//	} 
//}

//uint8_t ADS8688_SPI_RB(void) 
//{ 
//	uint8_t Rdata=0,s; 
//	CS_Enable();
//	for(s=0;s<8;s++) 
//	{ 
//		Rdata<<=1;
//		CS_Disable();
//		if(HAL_GPIO_ReadPin(ADS8688_MISO_PORT,ADS8688_MISO_PIN)) 
//		{ 
//			Rdata|=0x01;
//		}
//		else 
//		{ 
//			Rdata&=0xFE;
//		}
//	CS_Enable(); 
//	}
//	return Rdata;
//}

//void ADS8688_WriteCommandReg(uint16_t command)//дADS8688����Ĵ��� 
//{ 
//	CS_Enable(); 
//	ADS8688_SPI_WB(command>>8 & 0XFF);
//	ADS8688_SPI_WB(command & 0XFF);
//	CS_Disable();
//}

//void ADS8688_Write_Program_Register(uint8_t Addr,uint8_t data)
//{ 
//	CS_Enable();
//	ADS8688_SPI_WB(Addr<<1| 0X01);
//	ADS8688_SPI_WB(data);
//	CS_Disable(); 
//}

//uint8_t ADS8688_READ_Program_Register(uint8_t Addr)
//{ 
//	uint8_t	data = 0;
//	CS_Enable();
//	ADS8688_SPI_WB(Addr<<1); 
//	data = ADS8688_SPI_RB();
//	data = ADS8688_SPI_RB(); 
//	CS_Disable(); 
//	return data;
//}

// void Enter_RESET_MODE(void)//�����λģʽ����λ program registers
//{
// ADS8688_WriteCommandReg(RST);
//}

//void AUTO_RST_Mode(void)//�����Զ�ɨ��ģʽ 
//{ 
//	ADS8688_WriteCommandReg(AUTO_RST);
//}

//void MAN_Ch_n_Mode(uint16_t ch)//�Զ�ģʽ 
//{
//	ADS8688_WriteCommandReg(ch);
//}

//void Set_CH_Range_Select(uint8_t ch,uint8_t range) //���ø���ͨ���ķ�Χ 
//{ 
//	ADS8688_Write_Program_Register(ch,range);
//}

//void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum) 
//{ 
//	//��ȡɨ��ͨ�����е�ADת������code������������ 
//	uint8_t i=0,datal=0,datah=0;
//	uint16_t	data=0; 
//	for (i=0; i<chnum; i++) 
//	{ 
//		CS_Enable(); ADS8688_SPI_WB(0X00); 
//		ADS8688_SPI_WB(0X00); 
//		datah = ADS8688_SPI_RB();
//		datal = ADS8688_SPI_RB();
//		CS_Disable();
//		data = datah<<8 | datal; //��λ��ǰ����λ�ں� 
//		*(outputdata+i) = data; 
//	} 
//}

////��ʼ��ͨ��1��Ĭ������10V��Χ 
//void ADS8688_Init_Single() 
//{
//	HAL_GPIO_WritePin(ADS_8688_RST_PD_GPIO_PORT,ADS_8688_RST_PD_Pin,GPIO_PIN_RESET);
//	DWT_Delay(0.000002);
//	HAL_GPIO_WritePin(ADS_8688_RST_PD_GPIO_PORT,ADS_8688_RST_PD_Pin,GPIO_PIN_SET);
//	HAL_GPIO_WritePin(ADS_8688_DAISY_IN_GPIO_PORT,ADS_8688_DAISY_IN_Pin,GPIO_PIN_RESET);
//	Enter_RESET_MODE();
//	ADS8688_Write_Program_Register(0X01,0Xff);
//	ADS8688_READ_Program_Register(0X01);
//	DWT_Delay(0.000002);
//	ADS8688_Write_Program_Register(0x02,0x00);//����ͨ���˳��͹���״̬ 
//	ADS8688_Write_Program_Register(0x01,0xff);//ʹ������ͨ�� 
//	Set_CH_Range_Select(CH1,0x00);//����ͨ��1����?��Χ��+-2.5*Vref 
//	// //0x00 -> +-2.5*ref 
//	// //0x01 -> +-1.25*ref 
//	// //0x02 -> +-0.625*ref
//	// //0x03 -> +2.5*ref 
//	// //0x04 -> +1.25*ref 
//	MAN_Ch_n_Mode(MAN_Ch_1); 
//	//HAL_Delay(300);
//}

//void ADS8688_Init_Mult(void)
//{
//	HAL_GPIO_WritePin(ADS_8688_RST_PD_GPIO_PORT,ADS_8688_RST_PD_Pin,GPIO_PIN_SET); 
//	HAL_GPIO_WritePin(ADS_8688_DAISY_IN_GPIO_PORT,ADS_8688_DAISY_IN_Pin,GPIO_PIN_RESET);
//	DWT_Delay(0.00001);
//	Enter_RESET_MODE();
//	//��������
//	ADS8688_Write_Program_Register(0X00,0X00);
//	//����ͨ��
//	ADS8688_Write_Program_Register(0X01,0X07);
//	//����
//	ADS8688_Write_Program_Register(0x02,0xf8);//����ͨ���˳��͹���״̬ 
//	//���ù���
//	ADS8688_Write_Program_Register(0x03,0x03);
//	DWT_Delay(0.00001);
//	
////	ADS8688_Write_Program_Register(0x01,0xff);//ʹ������ͨ�� 
//	Set_CH_Range_Select(CH1,0x00);//����ͨ��1�����뷶Χ��+-2.5*Vref 
//	DWT_Delay(0.00001);
//	Set_CH_Range_Select(CH2,0x00); 
//	DWT_Delay(0.00001);
//	Set_CH_Range_Select(CH3,0x00);
//	DWT_Delay(0.00001);

//	/*
//	Set_CH_Range_Select(CH2,0x00);
//	Set_CH_Range_Select(CH3,0x00);
//	Set_CH_Range_Select(CH4,0x00); 
//	Set_CH_Range_Select(CH5,0x00); 
//	Set_CH_Range_Select(CH6,0x00); 
//	Set_CH_Range_Select(CH7,0x00); 
//	
//	*/
//	// //0x00 -> +-2. 5*ref 
//	// //0x01 -> +-1.25*ref 
//	// //0x02 -> +-0.625*ref 
//	// //0x03 -> +2.5*ref 
//	// //0x04 -> +1.25*ref 
//	//AUTO_RST_Mode();//�����Զ�ɨ��ģʽ 
//	ADS8688_Write_Program_Register(0xa0,0x03);
//	DWT_Delay(0.000010);
//  AUTO_RST_Mode();
//}

//double get_realdata(uint16_t x,uint8_t t)//mv
//{
//	double y;
//	
//	if(!t)
//	{
//		y = (int16_t)(((double)(x-32762) *20.48/(double)65535)*1000);
//	}
//  else
//	{
//		y = (int16_t)(((double)(x-32725) *20.48/(double)65535)*1000);
//	}
//	return y;
//}

////��ʼ����ͨ����Ĭ��ȫ��������10V��Χ 
//void gat_real_ad(float *real_ad)
//{
//	real_ad[0]= (float)get_realdata(My_Ad[0],0);
//	real_ad[1]= (float)get_realdata(My_Ad[1],1);
//}

//void get_ADS_1and2(uint16_t *My_Ad,float *real_ad)
//{
//	Get_AUTO_RST_Mode_Data(My_Ad,2);
//	gat_real_ad(real_ad);
//}

//uint16_t Get_MAN_Ch_n_Mode_Data(void) 
//{ 
//	uint8_t datah=0,datal=0;
//	CS_Enable(); 
//	ADS8688_SPI_WB(0X00);
//	ADS8688_SPI_WB(0X00);
//	datah = ADS8688_SPI_RB();
//	datal = ADS8688_SPI_RB();
//	CS_Disable(); 
//	return (datah<<8 | datal);
//}

//uint16_t get_ADS_ch1(void) 
//{ 
//	return Get_MAN_Ch_n_Mode_Data();//��ȡͨ��1����,����ͨ�����ɺ��� MAN_Ch_n_Mode()�������� 
//}
