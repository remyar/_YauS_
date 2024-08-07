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

bool ARCH_GetUart1UseRs485(void);

void ARCH_Uart1Init(unsigned long baud, uint32_t flags);
void ARCH_Uart2Init(unsigned long baud, uint32_t flags);
void ARCH_Uart3Init(unsigned long baud, uint32_t flags);

void ARCH_Uart1SendByte(uint8_t data);
void ARCH_Uart2SendByte(uint8_t data);
void ARCH_Uart3SendByte(uint8_t data);

void ARCH_Uart1SendByteSync(uint8_t data);
void ARCH_Uart2SendByteSync(uint8_t data);
void ARCH_Uart3SendByteSync(uint8_t data);

bool ARCH_Uart1Available(void);
bool ARCH_Uart2Available(void);

uint8_t ARCH_Uart1Read(void);
uint8_t ARCH_Uart2Read(void);

uint32_t ARCH_Uart2GetBaudrate(void);

#endif