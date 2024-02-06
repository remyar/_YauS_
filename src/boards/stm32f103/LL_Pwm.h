#ifndef _LL_PWM_H_
#define _LL_PWM_H_

#include "arch.h"

enum
{
    LL_PWM_PB0 = 0,
    LL_MAX_PWM
};

void LLPWM_Init(uint8_t periphNum, uint32_t freq);
void LLPWM_SetDutyCycle(uint8_t periphNum, float percent);

#endif