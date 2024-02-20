#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include "arch.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

void ARCH_UartInit(USART_TypeDef *uartNum, unsigned long baud, uint32_t flags);
void ARCH_UartSendByteSync(USART_TypeDef *uartNum, uint8_t data);
void ARCH_UartSendByte(USART_TypeDef *uartNum, uint8_t data);

void ARCH_Uart1UseRs485(bool value);
void ARCH_Uart2UseRs485(bool value);
void ARCH_Uart3UseRs485(bool value);

void ARCH_Uart1Init(unsigned long baud, uint32_t flags);
void ARCH_Uart2Init(unsigned long baud, uint32_t flags);
void ARCH_Uart3Init(unsigned long baud, uint32_t flags);

void ARCH_Uart1SendByte(uint8_t data);
void ARCH_Uart2SendByte(uint8_t data);
void ARCH_Uart3SendByte(uint8_t data);

void ARCH_Uart1SendByteSync(uint8_t data);
void ARCH_Uart2SendByteSync(uint8_t data);
void ARCH_Uart3SendByteSync(uint8_t data);

#endif