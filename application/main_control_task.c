#include "main_control_task.h"
#include "cmsis_os.h"
#include "tim.h"
#include "stdio.h"
#include "user_lib.h"
#include "string.h"
#include "TD.h"
#include "bsp_dwt.h"

control_t control_converter;

int pid_output1;
int pid_output2;

int q_dcdc_temp_1 = 980;
int q_dcdc_temp_2;

int q_dcdc_temp1_test = 900;

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
    DWT_Delay(0.0001);
  }
}

void control_value_init(control_t *control_value_init)
{
	const static fp32 adc_order_filter[1] = {0.1};
	
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
		control_value_init->q_dcdc[i] = 500;
	}
	for(int i = 0; i < 3; i++)
	{
		control_value_init->current[i] = 0.0f;
		control_value_init->voltage[i] = 0.0f;
	}
	control_value_init->voltage_30 = 30.0f;
	
	control_value_init->Vin = 0.0f;	
	
	control_value_init->mppt_last_power = 0.0f;
	control_value_init->mppt_step = 0.001f;
	control_value_init->mppt_direction = -1;
}

void control_value_set(control_t *control_value_set)
{
	
	control_value_set->current[0] =	(control_value_set->AD7606B_Cal[3] / 1000.0f) * 1.216f - 0.453f;
	
	control_value_set->voltage[0] = - (control_value_set->AD7606B_Cal[0] / 1000.0f * 101.0f / 6.0f) * 1.01617f;
	
	control_value_set->voltage[1] = - (control_value_set->AD7606B_Cal[1] / 1000.0f * 101.0f / 6.0f) / 0.997f;

	control_value_set->voltage[2] = - (control_value_set->AD7606B_Cal[2] / 1000.0f * 101.0f / 6.0f);
	
	control_value_set -> i0_mul_r1 = control_value_set -> current_filter[0].out * input_resistance;
	control_value_set -> v_batterty_div_30 = (int)(control_value_set -> voltage_filter[2].out / control_value_set -> voltage_30 * 1000.0f);
	
	PID_calc(&control_value_set->dcdc_voltage1_pid, control_value_set -> voltage_filter[0].out, control_value_set -> i0_mul_r1);
	PID_calc(&control_value_set->dcdc_voltage2_pid, control_value_set -> voltage_filter[1].out, control_value_set -> voltage_30);
}

void control_value_loop(control_t *control_value_loop)
{
	static uint32_t task_counter = 0;  // 添加任务周期计数器
	
	//DCDC_D
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,control_value_loop->q_dcdc[0]);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,control_value_loop->q_dcdc[1]);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,control_value_loop->q_dcdc[2]);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,control_value_loop->q_dcdc[3]);

	//Vin
	if(control_value_loop -> voltage_filter[0].out + 10.0f * control_value_loop -> current_filter[0].out < 0.0f)
	{
		control_value_loop->Vin = 0.0f;
	}
	else
	{
		control_value_loop -> Vin = control_value_loop -> voltage_filter[0].out + 10.0f * control_value_loop -> current_filter[0].out;
	}
	
	//first_filter
	for(int i = 0; i < 3; i++)
	{
		first_order_filter_cali(&control_value_loop->current_filter[i], control_value_loop->current[i]);
	}
	
	for(int i = 0; i < 3; i++)
	{
		first_order_filter_cali(&control_value_loop->voltage_filter[i], control_value_loop->voltage[i]);
	}
	
	//first_dcdc

	
	//close_loop
//	for(int i = 0; i < 2; i++)
//	{
//		pid_output1 = (int)(control_value_loop->dcdc_voltage1_pid.out);
//		
//		if (pid_output1 > 50)
//		{
//				pid_output1 = 50;
//		} 
//		else if (pid_output1 < -500) 
//		{
//				pid_output1 = -500;
//		}
//		control_value_loop->q_dcdc[i] = 850;// + pid_output1;
//	}

	//close_loop
//	if((task_counter % 2000) == 2)
//	{
//		// 计算当前输入功率
//		control_value_loop->current_power = control_value_loop->voltage_filter[0].out * control_value_loop->current_filter[0].out;
//		
//		// 更新占空比
//		q_dcdc_temp_1 -= (int)(control_value_loop->mppt_direction * control_value_loop->mppt_step * 1000);
//		
//		// 扰动观察法逻辑
//		if (control_value_loop->current_power > control_value_loop->mppt_last_power) 
//		{
//				// 功率增加，保持方向
//				control_value_loop->mppt_direction *= 1;
//		}
//		else if(control_value_loop->current_power < control_value_loop->mppt_last_power) 
//		{
//				// 功率减少，反转方向
//				control_value_loop->mppt_direction *= -1;
//		}
//		else
//		{
//				goto GOON;
//		}
//		
//		if (q_dcdc_temp_1 > 980) 
//		{
//				q_dcdc_temp_1 = 980;
//		} 
//		else if (q_dcdc_temp_1 < 600) 
//		{
//				q_dcdc_temp_1 = 600;
//		}
//		
//		for (int i = 0; i < 2; i++) 
//		{
//			control_value_loop->q_dcdc[i] = q_dcdc_temp_1;
//		}
//		
//		GOON:
//		
//		// 保存当前功率供下一周期比较
//		control_value_loop->mppt_last_power = control_value_loop->current_power;

//	}

	if((task_counter % 200) == 2)
	{
		// 计算当前输入功率
		control_value_loop->current_power = control_value_loop->voltage_filter[0].out * control_value_loop->current_filter[0].out;
		
		// 更新占空比
		q_dcdc_temp_1 -= (int)(control_value_loop->mppt_direction * control_value_loop->mppt_step * 1000);
		
		// 扰动观察法逻辑
		if (control_value_loop->voltage_filter[0].out > control_value_loop->i0_mul_r1) 
		{
				// 功率增加，保持方向
				control_value_loop->mppt_direction = 1;
		}
		else if(control_value_loop->voltage_filter[0].out < control_value_loop->i0_mul_r1) 
		{
				// 功率减少，反转方向
				control_value_loop->mppt_direction = -1;
		}
		else
		{
				goto GOON;
		}
		
		if (q_dcdc_temp_1 > 980) 
		{
				q_dcdc_temp_1 = 980;
		} 
		else if (q_dcdc_temp_1 < 400) 
		{
				q_dcdc_temp_1 = 400;
		}
		
		for (int i = 0; i < 2; i++) 
		{
			control_value_loop->q_dcdc[i] = q_dcdc_temp_1;
		}
		
		GOON:
		
		// 保存当前功率供下一周期比较
		control_value_loop->mppt_last_power = control_value_loop->current_power;

	}
	
	//second_dcdc

	
	for(int i = 2; i < 4; i++)
	{
		pid_output2 = (int)(control_value_loop->dcdc_voltage2_pid.out);
		
		q_dcdc_temp_2 = control_value_loop -> v_batterty_div_30 - pid_output2;
		
		if (q_dcdc_temp_2 > 980) 
		{
				q_dcdc_temp_2 = 980;
		} 
		else if (q_dcdc_temp_2 < 250) 
		{
				q_dcdc_temp_2 = 250;
		}
		
		control_value_loop->q_dcdc[i] = q_dcdc_temp_2;
	}
	
	if(task_counter > 200)
	{
		task_counter = 0;
	}
	task_counter++;
}

