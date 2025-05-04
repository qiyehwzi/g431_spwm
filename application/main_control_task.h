#ifndef MAIN_CONTROL_TASK_H
#define MAIN_CONTROL_TASK_H

#include "stdint.h"
#include "pid.h"
#include "user_lib.h"
#include "TD.h"

//��һ������ʸ���pid
#define DCDC_VOLTAGE1_KP 0.0f//450.0f
#define DCDC_VOLTAGE1_KI 0.0f
#define DCDC_VOLTAGE1_KD 0.0f
#define DCDC_VOLTAGE1_PID_MAX_OUT 700.0f
#define DCDC_VOLTAGE1_PID_MAX_IOUT 0.0f

//dcdc 30Vpid
#define DCDC_VOLTAGE2_KP 5.0f//400.0f
#define DCDC_VOLTAGE2_KI 0.01f
#define DCDC_VOLTAGE2_KD 0.0f//10.0f
#define DCDC_VOLTAGE2_PID_MAX_OUT 500.0f
#define DCDC_VOLTAGE2_PID_MAX_IOUT 100.0f

//��Ƭ���Դ�adc
#define Vref 3.3f // ����ο���ѹ3.3V
#define resolution 4096 // 12λADC
#define voltage_stm_to_real 10
#define input_resistance 10.0f
#define current_a 1.25f
#define current_b_1 0.7727000f
#define current_b_2 1.30304f
#define current_b_3 1.39000f

typedef struct
{
	int var_duty;//spwmռ�ձȱ仯
	
	float voltage[3];
	float current[3];
	
	int16_t AD7606B_Data[8];
	float AD7606B_Cal[8];
	
	first_order_filter_type_t current_filter[3];  //use first order filter to slow set-point.ʹ��һ�׵�ͨ�˲������趨ֵ
	first_order_filter_type_t voltage_filter[3];  //use first order filter to slow set-point.ʹ��һ�׵�ͨ�˲������趨ֵ
	int q_dcdc[4]; //PA5 PB3 PB10 PB11 ǰ������һ���Ϲ��¹ܣ��������ڶ����Ϲ��¹ܣ���Χ0-1000��Ӧռ�ձ�0-1
	float voltage_30;
	float Vin;
	
	float i0_mul_r1;
	int v_batterty_div_30;
	
	pid_type_def dcdc_voltage1_pid;
	pid_type_def dcdc_voltage2_pid;
	
	TD_t voltage1_TD;
	
	float current_power;
	float mppt_last_power;  // ��һ���ڹ���
	float mppt_step;        // �Ŷ���������0.01��Ӧռ�ձȱ仯��10��
	int mppt_direction;     // �Ŷ�����1��-1��

}control_t;

extern control_t control_converter;
extern void main_control_Task(void const * argument);

#endif
