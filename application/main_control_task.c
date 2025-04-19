#include "main_control_task.h"
#include "cmsis_os.h"
#include "tim.h"
#include "stdio.h"
#include "adc.h"
#include "user_lib.h"

control_t control_converter;

static void control_value_init(control_t *control_value_init);
static void control_value_set(control_t *control_value_set);
static void control_value_loop(control_t *control_value_loop);

void main_control_Task(void const * argument)
{
	osDelay(100);
	control_value_init(&control_converter);
  for(;;)
  {
		control_value_set(&control_converter);
		control_value_loop(&control_converter);
    osDelay(1);
  }
}

void control_value_init(control_t *control_value_init)
{
	const static fp32 adc_order_filter[1] = {0.166666666667};
	
	for(int i = 0; i < 3; i++)
	{
		first_order_filter_init(&control_value_init->current_filter[i], 0.001, adc_order_filter);

	}
	
	for(int i = 0; i < 3; i++)
	{
		first_order_filter_init(&control_value_init->voltage_filter[i], 0.001, adc_order_filter);
	}
	
	const static fp32 dcdc_voltage1_pid[3]={DCDC_VOLTAGE1_KP,DCDC_VOLTAGE1_KI,DCDC_VOLTAGE1_KD};
	const static fp32 dcdc_voltage2_pid[3]={DCDC_VOLTAGE2_KP,DCDC_VOLTAGE2_KI,DCDC_VOLTAGE2_KD};
	PID_init(&control_value_init->dcdc_voltage1_pid , PID_POSITION, dcdc_voltage1_pid, DCDC_VOLTAGE1_PID_MAX_OUT, DCDC_VOLTAGE1_PID_MAX_IOUT);
	PID_init(&control_value_init->dcdc_voltage2_pid , PID_POSITION, dcdc_voltage2_pid, DCDC_VOLTAGE2_PID_MAX_OUT, DCDC_VOLTAGE2_PID_MAX_IOUT);
	
	for(int i = 0; i < 4; i++)
	{
		control_value_init->q_dcdc[i] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		control_value_init->AD_current_get[i] = 0;
		control_value_init->AD_voltage_get[i] = 0;
		control_value_init->current[i] = 0.0f;
		control_value_init->voltage[i] = 0.0f;
	}
	control_value_init->voltage_middle = 30.0f;
	
	control_value_init->Vin = 0.0f;
}

void control_value_set(control_t *control_value_set)
{
	control_value_set->current[0] = (control_value_set->AD_current_get[0] * Vref / resolution - current_a ) / current_b_1;
	control_value_set->current[1] = (control_value_set->AD_current_get[1] * Vref / resolution - current_a ) / current_b_2;
	control_value_set->current[2] = (control_value_set->AD_current_get[2] * Vref / resolution - current_a ) / current_b_3;
	
	//vin计算
	for(int i = 0; i < 3; i++)
	{
		control_value_set->voltage[i] = control_value_set->AD_voltage_get[i] * Vref / resolution * voltage_stm_to_real;
	}
	
	control_value_set -> i0_mul_r1 = control_value_set -> current_filter[0].out * input_resistance;
	
	PID_calc(&control_value_set->dcdc_voltage1_pid, control_value_set -> voltage_filter[0].out, control_value_set -> i0_mul_r1);
	PID_calc(&control_value_set->dcdc_voltage2_pid, control_value_set -> voltage_filter[1].out, control_value_set -> voltage_middle);
}

void control_value_loop(control_t *control_value_loop)
{
	//DCDC占空比
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,control_value_loop->q_dcdc[0]);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,control_value_loop->q_dcdc[1]);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,control_value_loop->q_dcdc[2]);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,control_value_loop->q_dcdc[3]);
	
	//ADC采样
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&control_value_loop->AD_current_get,3);
	HAL_ADC_Start_DMA(&hadc2,(uint32_t *)&control_value_loop->AD_voltage_get,3);
	
	//Vin计算
	if(control_value_loop -> voltage_filter[0].out + 10.0f * control_value_loop -> current_filter[0].out < 0.0f)
	{
		control_value_loop->Vin = 0.0f;
	}
	else
	{
		control_value_loop -> Vin = control_value_loop -> voltage_filter[0].out + 10.0f * control_value_loop -> current_filter[0].out;
	}
	
	//第一级dcdc
//	for(int i = 0; i < 2; i++)
//	{
//		if(((1 - (control_value_loop->Vin / control_value_loop -> voltage_middle)) * 1000 < 700) && ((1 - (control_value_loop->Vin / control_value_loop -> voltage_middle)) * 1000 > 200))
//		{
//			control_value_loop -> q_dcdc[i] = (1 - (control_value_loop->Vin / control_value_loop -> voltage_middle)) * 1000;
//		}
//		else if((1 - (control_value_loop->Vin / control_value_loop -> voltage_middle)) * 1000 >= 700)
//		{
//			control_value_loop -> q_dcdc[i] = 700;
//		}
//		else
//		{
//			control_value_loop -> q_dcdc[i] = 200;
//		}
//	}
	
	for(int i = 0; i < 2; i++)
	{
		int pid_output = (int)control_value_loop->dcdc_voltage1_pid.out;
		if (pid_output > 700) 
		{
				pid_output = 700;
		} else if (pid_output < 200) 
		{
				pid_output = 200;
		}
		control_value_loop->q_dcdc[i] = pid_output;
	}
	
	//第二级dcdc
//	for(int i = 2; i < 4; i++)
//	{
//		if((((control_value_loop -> voltage_filter[2].out / control_value_loop -> voltage_middle) * 1000) < 700) && (((control_value_loop -> voltage_filter[2].out / control_value_loop -> voltage_middle) * 1000) > 200))
//		{
//			control_value_loop -> q_dcdc[i] =  control_value_loop -> voltage_filter[2].out / control_value_loop -> voltage_middle * 1000;
//		}
//		else if((((control_value_loop -> voltage_filter[2].out / control_value_loop -> voltage_middle) * 1000) > 700))
//		{
//			control_value_loop -> q_dcdc[i] = 700;
//		}
//		else
//		{
//			control_value_loop -> q_dcdc[i] = 200;
//		}
//	}
	
	for(int i = 2; i < 4; i++)
	{
		int pid_output = (int)control_value_loop->dcdc_voltage2_pid.out;
		if (pid_output > 700) 
		{
				pid_output = 700;
		} else if (pid_output < 200) 
		{
				pid_output = 200;
		}
		control_value_loop->q_dcdc[i] = pid_output;
	}
	
	//电压电流一阶低通滤波
	for(int i = 0; i < 3; i++)
	{
		first_order_filter_cali(&control_value_loop->current_filter[i], control_value_loop->current[i]);
	}
	
	for(int i = 0; i < 3; i++)
	{
		first_order_filter_cali(&control_value_loop->voltage_filter[i], control_value_loop->voltage[i]);
	}
//	control_value_loop-> = control_value_loop->dcdc_voltage_pid.out;
}
