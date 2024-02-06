#ifndef _LL_UART_H_
#define _LL_UART_H_

#include "arch.h"

enum
{
    LL_UART1 = 0,
    LL_UART2,
    LL_UART3,
    LL_MAX_UART
};

void LLUART_Init(uint8_t uartNum, unsigned long baud, PTR_LL_CALLBACK_FUNC ptrCallback);
void LLUART_SendByte(uint8_t uartNum, uint8_t data);

#endif