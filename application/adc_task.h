#ifndef ADC_TASK_H
#define ADC_TASK_H

#include "stdint.h"

extern int16_t AD7606B_Data[8];

extern void adc_Task(void const * argument);

#endif
