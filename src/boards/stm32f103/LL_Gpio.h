#ifndef _LL_GPIO_H_
#define _LL_GPIO_H_

#include "arch.h"

void HAL_GPIO_WriteFastPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

#endif