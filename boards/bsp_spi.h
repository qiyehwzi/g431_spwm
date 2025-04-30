/**
 ****************************************************************************************************
 * @file        spi.c
 * @author      ��һ����
 * @version     V1.0
 * @date        2020-04-21
 * @brief       spi��������
 ****************************************************************************************************
 */
#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"
#include <stdio.h>
#include <stdint.h>
#include "gpio.h"

/*********************************��������*********************************************/ 
#define AD7606B_SCLK_L      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)
#define AD7606B_SCLK_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)
#define AD7606B_SDI_L      	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET)
#define AD7606B_SDI_H     	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)

#define AD7606B_SDO 		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)

/*******************************�ⲿ��������*******************************************/  													  
void spi1_init(void);
void spi1_set_speed(uint8_t speed);
uint16_t spi1_read_write_16bit(uint16_t data);

#endif
