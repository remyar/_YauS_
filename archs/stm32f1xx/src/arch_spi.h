#ifndef _ARCH_SPI_H
#define _ARCH_SPI_H

#include "arch.h"

extern SPI_HandleTypeDef hspi1;

void ARCH_SpiInit(uint32_t periphNum, unsigned long freq, uint32_t flags);
uint8_t ARCH_SpiSendByte(uint32_t periphNum, uint8_t data);
uint8_t ARCH_Spi1SendByte(uint8_t data);
void ARCH_Spi1Init(unsigned long freq, uint32_t flags);
uint8_t ARCH_SpiSendAndReceivedBytes(uint32_t periphNum, uint8_t *pDataIn, uint8_t *pDataOut, uint16_t length);
uint8_t ARCH_SpiSendBytes(uint32_t periphNum, uint8_t *pDataIn, uint16_t length);

#endif