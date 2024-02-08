#ifndef _ARCH_H_
#define _ARCH_H_

#include "stm32f1xx.h"

#include <_YauS_.h>
#include "arch_gpio.h"
#include "arch_it.h"
#include "arch_uart.h"
#include "arch_pwm.h"
#include "arch_tick.h"
#include "arch_i2c.h"
#include "arch_vi2c.h"

void ARCH_Init(void);

#endif