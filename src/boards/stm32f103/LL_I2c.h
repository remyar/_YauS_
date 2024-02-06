#ifndef _LL_I2C_H_
#define _LL_I2C_H_

#include "arch.h"

enum
{
    LL_I2C1 = 0,
    LL_I2C2,
    LL_MAX_I2C
};

void LLI2C_Init(uint8_t periphNum, unsigned long speed, PTR_LL_CALLBACK_FUNC ptrCallback);
void LLI2C_SendBytes(uint8_t periphNum, uint8_t addr, uint8_t *pData, uint8_t len);

#endif