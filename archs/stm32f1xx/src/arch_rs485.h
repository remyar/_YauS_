#ifndef _ARCH_RS485_H
#define _ARCH_RS485_H

#include "arch.h"

void ARCH_RS4851Init(uint32_t speed, GPIO_TypeDef *portDe, uint32_t pinDe, uint32_t flags);
void ARCH_RS4851SendByteSync(uint8_t data);

#endif