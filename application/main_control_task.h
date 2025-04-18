#ifndef MAIN_CONTROL_TASK_H
#define MAIN_CONTROL_TASK_H

#include "stdint.h"
#include "pid.h"
#include "user_lib.h"

#define DCDC_VOLTAGE_KP 0.0f
#define DCDC_VOLTAGE_KI 0.0f
#define DCDC_VOLTAGE_KD 0.0f
#define DCDC_VOLTAGE_PID_MAX_OUT 0.0f
#define DCDC_VOLTAGE_PID_MAX_IOUT 0.0f

#define Vref 3.3f // 假设参考电压3.3V
#define resolution 4096 // 12位ADC
#define voltage_stm_to_real 10
#define current_a 1.25f
#define current_b_1 1.28000f
#define current_b_2 1.30304f
#define current_b_3 1.39000f

typedef struct
{
	first_order_filter_type_t current_filter[3];  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
	first_order_filter_type_t voltage_filter[3];  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
	
	int var_duty;//spwm占空比变化
	
	uint16_t AD_current_get[3]; //PA0 PA2 PB14
	uint16_t AD_voltage_get[3]; //PA1 PA7 PC0
	float voltage[3];
	float current[3];
	int q_dcdc[4]; //PA5 PB3 PB10 PB11 前两个第一组正反，后两个第二组正反，范围0-1000对应占空比0-1
	float Vin;
	
	float voltage_set;//30V的第一级dcdc输出
	
	pid_type_def dcdc_voltage_pid;
	
}control_t;

extern control_t control_converter;
extern void main_control_Task(void const * argument);

#endif
