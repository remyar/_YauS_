#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include "arch.h"

extern UART_HandleTypeDef huart1;

void ARCH_UartInit(uint32_t uartNum, unsigned long baud, uint32_t flags);
void ARCH_UartSendByteSync(uint32_t uartNum, uint8_t data);
void ARCH_UartSendByte(uint32_t uartNum, uint8_t data);

void ARCH_Uart1Init(unsigned long baud, uint32_t flags);
void ARCH_Uart1SendByte(uint8_t data);

#endif