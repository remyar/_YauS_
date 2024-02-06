#ifndef _LL_VI2C_H_
#define _LL_VI2C_H_

#include "arch.h"

enum
{
    LL_VI2C1 = 0,
    LL_MAX_VI2C
};

void LLVI2C_Init(uint8_t periphNum, uint32_t portScl, uint32_t pinScl, uint32_t portSda, uint32_t pinSda, unsigned long speed, PTR_LL_CALLBACK_FUNC ptrCallback);
void LLVI2C_SendBytes(uint8_t periphNum, uint8_t addr, uint8_t *pData, uint8_t len, bool stop);
#endif