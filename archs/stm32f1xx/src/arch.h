#ifndef _ARCH_H_
#define _ARCH_H_

#include "stm32f1xx.h"

#include <_YauS_.h>
#include "arch_gpio.h"
#include "arch_it.h"
#include "arch_adc.h"
#include "arch_uart.h"
#include "arch_rs485.h"
#include "arch_pwm.h"
#include "arch_tick.h"
#include "arch_i2c.h"
#include "arch_vi2c.h"
#include "arch_usb.h"
#include "arch_spi.h"
#include "arch_vspi.h"

//-- ARDUINO WRAPPPER
#include "arduino_wrapper.h"

#define USE_ALTERNATE_FUNCTION (1 << 0)

#define Has_flag(X, Y) (((X & Y) == Y) ? true : false)



void ARCH_Init(void);

#endif