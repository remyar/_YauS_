#ifndef _ARCH_PWM_H
#define _ARCH_PWM_H

#include "arch.h"

enum
{
    PWMB0,
    PWMB1,
    PWM_MAX
};

void ARCH_PwmInit(uint32_t periphNum, unsigned long baud, uint32_t flags);
void ARCH_PwmSetDutyCycle(uint8_t periphNum, float percent);

void ARCH_Pwm1Init(unsigned long freq, uint32_t flags);
void ARCH_Pwm1SetDutyCycle(float percent);

void ARCH_Pwm2Init(unsigned long freq, uint32_t flags);
void ARCH_Pwm2SetDutyCycle(float percent);

#endif