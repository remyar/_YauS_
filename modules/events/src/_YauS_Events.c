//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// MODULE       : Event.c
// DESCRIPTION  : Gestion d'un Evenement
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "_YauS_Events.h"
#include <_YauS_.h>

//=============================================================================
//--- DEFINITIONS
//=============================================================================

extern s_TASK tasks[YAUS_MAX_TASKS];

//-----------------------------------------------------------------------------
// FONCTION    : EVENT_None
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void EVENT_None(uint8_t TaskId)
{
    for (int i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == TaskId)
        {
            tasks[i].event.type = NULL;
        }
    }
}

//-----------------------------------------------------------------------------
// FONCTION    : EVENT_Clear
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void EVENT_Clear(uint8_t TaskId, uint32_t type)
{
    for (int i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == TaskId)
        {
            if (tasks[i].event.type != NULL)
            {
                tasks[i].event.type &= ~type;
            }
        }
    }
}

void EVENT_Push(uint8_t TaskId, uint32_t type, void *pValue)
{
    for (int i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == TaskId)
        {
            s_EVENT sE;
            sE.type = type;
            if (type == KEYB_EVENT)
            {
                s_KEYB_EVENT *v = (s_KEYB_EVENT *)pValue;
                sE.key.code = v->code;
                sE.key.isLong = v->isLong;
            }
            if (type == LED_EVENT)
            {
                s_LED_EVENT *v = (s_LED_EVENT *)pValue;
                sE.led.state = v->state;
                sE.led.delay = v->delay;
                sE.led.rpm.maxRpm = v->rpm.maxRpm;
                sE.led.rpm.rpm = v->rpm.rpm;
                sE.led.lum = v->lum;
            }
            if (type == DISPLAY_EVENT)
            {
                s_DISPLAY_EVENT *v = (s_DISPLAY_EVENT *)pValue;
                sE.display.index = v->index;
                sE.display.value = v->value;
            }
            if (type == COMPC_EVENT)
            {
                s_COMPC_EVENT *v = (s_COMPC_EVENT *)pValue;
                sE.compc.state = v->state;
                sE.compc.length = v->length;
                //  sE.compc.str = v->str;
            }
            if (type == PLIP_EVENT)
            {
                s_PLIP_EVENT *v = (s_PLIP_EVENT *)pValue;
                sE.plip.code = v->code;
            }
            if (type == MCU_EVENT)
            {
                s_MCU_EVENT *v = (s_MCU_EVENT *)pValue;
                sE.mcu.state = v->state;
            }
            if (type == CANBUS_EVENT)
            {
                s_CANBUS_EVENT *v = (s_CANBUS_EVENT *)pValue;
                sE.canBus.state = v->state;
            }
            if (type == TOUCH_EVENT)
            {
                s_TOUCH_EVENT *v = (s_TOUCH_EVENT *)pValue;
                sE.touch.page = v->page;
                sE.touch.componentId = v->componentId;
                sE.touch.pressed = v->pressed;
            }
            if (type == GET_TEXT_EVENT)
            {
                s_GET_TEXT_EVENT *v = (s_GET_TEXT_EVENT *)pValue;
                sE.getText.page = v->page;
                memcpy(sE.getText.text, v->text, 16);
            }
            if (type == GET_VAL_EVENT)
            {
            }

            tasks[i].event = sE;
        }
    }
}

bool EVENT_HasEvent(void)
{
    bool val = false;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (tasks[i].event.type != NULL)
        {
            val = true;
        }
    }
    return val;
}