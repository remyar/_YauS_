#ifndef _ARCH_H_
#define _ARCH_H_

#include "stm32f4xx.h"

#include <_YauS_.h>
#include "arch_it.h"
#include "arch_tick.h"
#include "arch_usb.h"

#define USE_ALTERNATE_FUNCTION (1 << 0)

#define Has_flag(X, Y) (((X & Y) == Y) ? true : false)

void ARCH_Init(void);

#endif