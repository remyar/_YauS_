//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// HEADER       : Event.h
//
//=============================================================================
#ifndef EVENT_H
#define EVENT_H

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include <arch.h>

//-----------------------------------------------------------------------------
// Definition de type
//-----------------------------------------------------------------------------

#define EVENT_HAS(E, K) (((E)->type & (K)) == (K))

typedef struct
{
    float rpm;
    float maxRpm;
} s_RPM_LED_EVENT;

typedef struct //--- s_MCU_EVENT
{
    uint8_t state;
} s_MCU_EVENT;

typedef struct //--- s_PLIP_EVENT
{
    uint16_t code;
} s_PLIP_EVENT;

typedef struct //--- s_KEYB_EVENT
{
    uint8_t code;
    bool isLong;
} s_KEYB_EVENT;

typedef struct //--- s_LED_EVENT
{
    uint8_t idxLed;
    uint8_t state;
    uint16_t delay;
} s_LED_EVENT;

typedef struct //-- s_MEAS_EVENT
{
    bool autoZ;
} s_MEAS_PRESS_EVENT;

typedef struct //--- s_DISPLAY_EVENT
{
    uint8_t index;
    uint8_t value;
} s_DISPLAY_EVENT;

typedef struct //--- s_COMPC_EVENT
{
    //uint8_t state;
    uint8_t length;
    uint8_t buff[128];
} s_COMPC_EVENT;

typedef struct //--- s_CANBUS_EVENT
{
    uint8_t state;
} s_CANBUS_EVENT;

typedef struct //--- s_DISPLAY_EVENT
{
    uint8_t page;
    uint8_t componentId;
    uint8_t pressed;
} s_TOUCH_EVENT;

typedef struct //--- s_DISPLAY_EVENT
{
    uint8_t page;
    char text[16];
} s_GET_TEXT_EVENT;

typedef struct //--- s_EVENT
{
    uint32_t type;
    s_COMPC_EVENT compc;
    s_KEYB_EVENT key;
    s_LED_EVENT led;
    s_MEAS_PRESS_EVENT meas;
    s_DISPLAY_EVENT display;
    //s_PLIP_EVENT plip;
    //s_MCU_EVENT mcu;
    //s_CANBUS_EVENT canBus;
    s_TOUCH_EVENT touch;
    s_GET_TEXT_EVENT getText;
} s_EVENT;

#define NO_EVENT 0x0000
#define KEYB_EVENT (uint32_t)(1 << 1)
#define LED_EVENT (uint32_t)(1 << 2)
#define DISPLAY_EVENT (uint32_t)(1 << 3)
#define COMPC_EVENT (uint32_t)(1 << 4)
#define PLIP_EVENT (uint32_t)(1 << 5)
#define MCU_EVENT (uint32_t)(1 << 6)
#define CANBUS_EVENT (uint32_t)(1 << 7)
#define TOUCH_EVENT (uint32_t)(1 << 8)
#define GET_TEXT_EVENT (uint32_t)(1 << 9)
#define GET_VAL_EVENT (uint32_t)(1 << 10)
#define ADC_EVENT (uint32_t)(1 << 11)
#define MEAS_EVENT (uint32_t)(1 << 12)
#define CAMP_EVENT (uint32_t)(1 << 13)
#define MEAS_PRESS_EVENT (uint32_t)(1 << 14)

//-----------------------------------------------------------------------------
// Fonctions publiques
//-----------------------------------------------------------------------------
void EVENT_None(uint8_t TaskId);
void EVENT_Clear(uint8_t TaskId, uint32_t type);
void EVENT_Push(uint8_t TaskId, uint32_t type, void *pValue);
bool EVENT_HasEvent(uint8_t TaskId);
s_EVENT *EVENT_GetEvent(uint8_t TaskId);
void EVENT_Immediat(uint8_t TaskId, uint32_t type, void *pValue);

#endif //--- EVENT_H