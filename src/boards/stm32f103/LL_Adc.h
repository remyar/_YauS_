#ifndef _LL_ADC_H_
#define _LL_ADC_H_

#include "arch.h"

enum {
    ADC_STATE_INIT = 0,
    ADC_STATE_START,
    ADC_STATE_WAIT_END,
    ADC_STATE_FINISH,
    ADC_STATE_MAX
};

enum
{
    LL_ADC_PA0 = 0,
    LL_ADC_PA1,
    LL_MAX_ADC
};

void LLADC_Init(uint8_t periphNum);
void LLADC_process(void);

#endif