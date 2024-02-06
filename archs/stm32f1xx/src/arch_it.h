#ifndef _ARCH_IT_H_
#define _ARCH_IT_H_

#include <_YauS_.h>

#include "stm32f1xx.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void Error_Handler(void);
void SysTick_Handler(void);

#endif