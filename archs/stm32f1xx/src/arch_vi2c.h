#ifndef _LL_VI2C_H_
#define _LL_VI2C_H_

#include "arch.h"

enum
{
    VI2C1 = 0,
    MAX_VI2C
};

void ARCH_VI2CInit(uint8_t periphNum, uint32_t portScl, uint32_t pinScl, uint32_t portSda, uint32_t pinSda, unsigned long speed, uint32_t flags);
void ARCH_VI2CSendBytes(uint8_t periphNum, uint8_t addr, uint8_t *pData, uint8_t len, bool stop);

void ARCH_VI2C1Init(uint32_t portScl, uint32_t pinScl, uint32_t portSda, uint32_t pinSda, unsigned long speed, uint32_t flags);
void ARCH_VI2C1SendBytes(uint8_t addr, uint8_t *pData, uint8_t len, bool stop);

#endif