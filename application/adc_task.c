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
		osDelay(1);
		while((AD7606B_BUSY == GPIO_PIN_SET))	/* ��ȡBUSY��״̬��Ϊ�͵�ƽ��ʾ��ƽת�����������Զ�ȡ���� */
		{
			__NOP();
		}
		AD7606B_Read_AD_Data(control_converter.AD7606B_Data);		/* ��ȡ���ݷ�������AD7606B_Data[] */
		for(int i = 0; i < 8; i++)
		{
			control_converter.AD7606B_Cal[i] = (float)((float)control_converter.AD7606B_Data[i]*AD7606B_Range/32768);
		}
		osDelay(1);
  }
}

