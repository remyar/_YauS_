#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include "arch.h"

extern UART_HandleTypeDef huart1;

void ARCH_UartInit(uint32_t uartNum, unsigned long baud, PTR_LL_CALLBACK_FUNC ptrCallback);
void ARCH_UartSendByteSync(uint32_t uartNum, uint8_t data);
void ARCH_UartSendByte(uint32_t uartNum, uint8_t data);

#endif