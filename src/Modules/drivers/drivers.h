#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include <_YauS_.h>
#include "arch.h"
#include "LL_Gpio.h"
#include "LL_Uart.h"
#include "LL_I2c.h"
#include "LL_VI2c.h"
#include "LL_Pwm.h"
#include "LL_Adc.h"


void DRIVERS_Init(void);
void DRIVERS_ProcessImmediat(void);

#endif