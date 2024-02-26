#ifndef _LL_VSPI_H_
#define _LL_VSPI_H_

#include "arch.h"

enum
{
    VSPI1 = 0,
    MAX_VSPI
};

void ARCH_VSPIInit(uint8_t periphNum,
                   uint32_t portScl, uint32_t pinScl,
                   uint32_t portMosi, uint32_t pinMosi,
                   uint32_t portMiso, uint32_t pinMiso,
                   uint32_t portSs, uint32_t pinSs,
                   unsigned long speed, uint32_t flags);

void ARCH_VSPI1Init(uint32_t portScl, uint32_t pinScl,
                    uint32_t portMosi, uint32_t pinMosi,
                    uint32_t portMiso, uint32_t pinMiso,
                    uint32_t portSs, uint32_t pinSs,
                    unsigned long speed, uint32_t flags);

void ARCH_VSPISendBytes(uint8_t periphNum, uint8_t *pData, uint8_t len) ;
void ARCH_VSPI1SendBytes(uint8_t *pData, uint8_t len);

#endif