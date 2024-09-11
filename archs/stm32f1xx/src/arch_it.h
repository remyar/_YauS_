#ifndef _ARCH_IT_H_
#define _ARCH_IT_H_

#include <_YauS_.h>

#include "stm32f1xx.h"

#include "arch_uart.h"
#include "arch_i2c.h"
#include "arch_spi.h"
#include "arch_can.h"

extern PCD_HandleTypeDef hpcd_USB_FS;

#define NO_IT 0
#define USB_LP_CAN1_RX0_IT (uint32_t)(1 << 0)

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

void ARCH_ProcessIt(void);

#endif