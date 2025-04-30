/**
 ****************************************************************************************************
 * @file        ad7606b.h
 * @author      科一电子
 * @version     V1.0
 * @date        2020-04-21
 * @brief       AD7606B驱动程序
 ****************************************************************************************************
 */
#ifndef _AD7606B_H_
#define _AD7606B_H_
#include "stdint.h"
#include "gpio.h"
//#include "sys.h"

/* AD7616输入电压范围 */
#define AD7606B_Range    	10000.0f	// AD7606B量程±10V

#define AD7606B_Range_2_5V		0x00	// ±2.5V量程
#define AD7606B_Range_5V		0x11	// ±5V量程
#define AD7606B_Range_10V		0x22	// ±10V量程

/*********************************引脚声明*********************************************/
#define AD7606B_SER_L      	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)
#define AD7606B_SER_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)
#define AD7606B_CNT_L      	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET)
#define AD7606B_CNT_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET)
#define AD7606B_WR_L      	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET)
#define AD7606B_WR_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET)
#define AD7606B_RST_L      	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)
#define AD7606B_RST_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)
#define AD7606B_CS_L      	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET)
#define AD7606B_CS_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET)

#define AD7606B_BUSY        HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)
#define AD7606B_FDA        	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)

/*******************************外部函数声明*******************************************/
extern void AD7606B_Init(void);
extern void AD7606B_Start_Convst(void);
extern uint8_t AD7606B_Read_Reg(uint8_t Addr);
extern void AD7606B_Write_Reg(uint8_t Addr, uint8_t Data);
extern void AD7606B_Read_AD_Data(int16_t * data);

#endif

