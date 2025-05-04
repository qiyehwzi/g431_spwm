#include "adc_task.h"
#include "cmsis_os.h"
#include "bsp_dwt.h"
#include "ad7606b.h"
#include "main_control_task.h"

void adc_Task(void const * argument)
{
	osDelay(10);
	for(int i = 0; i < 8; i++)
	{
		control_converter.AD7606B_Data[i] = 0;
		control_converter.AD7606B_Cal[i] = 0.0f;
	}
  for(;;)
  {
		AD7606B_Start_Convst();
		DWT_Delay(0.0001);
		while((AD7606B_BUSY == GPIO_PIN_SET))	/* 读取BUSY的状态，为低电平表示电平转换结束，可以读取数据 */
		{
			__NOP();
		}
		AD7606B_Read_AD_Data(control_converter.AD7606B_Data);		/* 读取数据放至数组AD7606B_Data[] */
		for(int i = 0; i < 8; i++)
		{
			control_converter.AD7606B_Cal[i] = (float)((float)control_converter.AD7606B_Data[i]*AD7606B_Range/32768);
		}
		DWT_Delay(0.0001);
  }
}

