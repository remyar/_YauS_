#ifndef _ARCH_I2C_H
#define _ARCH_I2C_H

#include "arch.h"

extern I2C_HandleTypeDef hi2c2;

void ARCH_I2CInit(uint32_t periphNum, unsigned long freq, uint32_t flags );
void ARCH_I2CSendBytes(uint32_t periphNum, uint8_t addr, uint8_t *pData, uint8_t length , bool stop);

void ARCH_I2C2Init(uint32_t freq, uint32_t flags);
void ARCH_I2C2SendBytes(uint8_t addr, uint8_t *pData, uint8_t length);

#endif