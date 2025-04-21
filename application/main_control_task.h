#ifndef MAIN_CONTROL_TASK_H
#define MAIN_CONTROL_TASK_H

#include "stdint.h"
#include "pid.h"
#include "user_lib.h"
#include "TD.h"

#define DCDC_VOLTAGE1_KP 450.0f
#define DCDC_VOLTAGE1_KI 0.0f
#define DCDC_VOLTAGE1_KD 0.0f
#define DCDC_VOLTAGE1_PID_MAX_OUT 700.0f
#define DCDC_VOLTAGE1_PID_MAX_IOUT 0.0f

#define DCDC_VOLTAGE2_KP 120.0f
#define DCDC_VOLTAGE2_KI 0.0f
#define DCDC_VOLTAGE2_KD 6.0f
#define DCDC_VOLTAGE2_PID_MAX_OUT 500.0f
#define DCDC_VOLTAGE2_PID_MAX_IOUT 50.0f

#define Vref 3.3f // 假设参考电压3.3V
#define resolution 4096 // 12位ADC
#define voltage_stm_to_real 10
#define input_resistance 10.0f
#define current_a 1.25f
#define current_b_1 0.92000f
#define current_b_2 1.30304f
#define current_b_3 1.39000f

typedef struct
{
	int var_duty;//spwm占空比变化
	
	uint16_t AD_current_get[3]; //PA0 PA2 PB14
	uint16_t AD_voltage_get[3]; //PA1 PA7 PC0
	float voltage[3];
	float current[3];
	first_order_filter_type_t current_filter[3];  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
	first_order_filter_type_t voltage_filter[3];  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
	int q_dcdc[4]; //PA5 PB3 PB10 PB11 前两个第一组上管下管，后两个第二组上管下管，范围0-1000对应占空比0-1
	float voltage_middle;
	float Vin;
	
	float i0_mul_r1;
	
	pid_type_def dcdc_voltage1_pid;
	pid_type_def dcdc_voltage2_pid;
	
	TD_t voltage1_TD;
	
}control_t;

extern control_t control_converter;
extern void main_control_Task(void const * argument);

#endif
