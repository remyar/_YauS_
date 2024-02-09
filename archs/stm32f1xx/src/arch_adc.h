#ifndef _ARCH_ADC_H_
#define _ARCH_ADC_H_

#include "arch.h"

extern ADC_HandleTypeDef hadc1;

enum
{
    ADC_STATE_INIT = 0,
    ADC_STATE_START,
    ADC_STATE_WAIT_END,
    ADC_STATE_FINISH,
    ADC_STATE_MAX
};

enum
{
    ADC_PA0 = 0,
    ADC_PA1,
    MAX_ADC
};

void ARCH_AdcInit(uint8_t periphNum);
void ARCH_AdcProcess(void);
uint32_t ARCH_AdcGetValue(uint8_t channel);

#endif