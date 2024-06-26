#ifndef _ARCH_RS485_H
#define _ARCH_RS485_H

#include "arch.h"

void ARCH_RS4851Init(uint32_t speed, GPIO_TypeDef *portDe, uint32_t pinDe, uint32_t flags);
void ARCH_RS4851SendBytesSync(uint8_t *pData, uint16_t length) ;
void ARCH_RS4851SendByteSync(uint8_t data);
bool ARCH_RS4851Available(void);
uint8_t ARCH_RS4851Read(void);

#endif