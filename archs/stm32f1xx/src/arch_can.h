#ifndef _ARCH_CAN_H
#define _ARCH_CAN_H

#include "arch.h"

#ifndef CAN1_DECODER_FUNC_SIZE
#define CAN1_DECODER_FUNC_SIZE 4
#endif

enum
{
    CAN_1,
    CAN_MAX
};

typedef enum
{
    BR125K = 15,
    BR250K = 7,
    BR500K = 3, // 500K and faster requires good electical design practice
    BR1M = 1
} BitRate;

typedef void (*CAN_DECODER_FUNC)(uint8_t *);

typedef struct
{
    uint16_t id;
    CAN_DECODER_FUNC decoderFunc;
} s_CAN_DECODER;

#define STD_ID_LEN (uint32_t)(1 << 0)
#define EXT_ID_LEN (uint32_t) (1 << 1)

#define PORTA_11_12_XCVR (uint32_t)(1 << 2)
#define PORTB_8_9_XCVR (uint32_t)(1 << 3)
#define PORTA_11_12_WIRE (uint32_t)(1 << 4)
#define PORTB_8_9_WIRE (uint32_t)(1 << 5)
#define PORTA_11_12_WIRE_PULLUP (uint32_t)(1 << 6)
#define PORTB_8_9_WIRE_PULLUP (uint32_t)(1 << 7)

void ARCH_CanInit(uint32_t periphNum, unsigned long baud, uint32_t flags);
void _HAL_CAN_IRQHandler(void);
void ARCH_CanAddIsr(uint16_t id, CAN_DECODER_FUNC ptrFunc);
bool ARCH_UseCan(void);

#endif