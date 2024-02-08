#ifndef _ARCH_PWM_H
#define _ARCH_PWM_H

#include "arch.h"

enum{
    PWMB0,
    PWMB1,
    PWM_MAX
};

void ARCH_PwmInit(uint32_t uartNum, unsigned long baud, PTR_LL_CALLBACK_FUNC ptrCallback);
void ARCH_PwmSetDutyCycle(uint8_t periphNum, float percent);

#endif