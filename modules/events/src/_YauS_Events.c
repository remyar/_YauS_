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

s_EVENT sEvent[YAUS_MAX_TASKS];
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
            sEvent[i].type = (uint32_t)NULL;
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
            if (sEvent[i].type != (uint32_t)NULL)
            {
                sEvent[i].type &= ~type;
            }
        }
    }
}

void EVENT_Immediat(uint8_t TaskId, uint32_t type, void *pValue)
{
    EVENT_Push(TaskId, type, pValue);
    YAUS_TaskForce(TaskId);
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
                sE.led.idxLed = v->idxLed;
                sE.led.delay = v->delay;
            }
            if (type == COMPC_EVENT)
            {
                s_COMPC_EVENT *v = (s_COMPC_EVENT *)pValue;
                sE.compc.length = v->length;
                memcpy(sE.compc.buff, v->buff, v->length);
            }
            if (type == MEAS_PRESS_EVENT)
            {
                s_MEAS_PRESS_EVENT *v = (s_MEAS_PRESS_EVENT *)pValue;
                sE.meas.autoZ = v->autoZ;
            }
            /*
            if (type == DISPLAY_EVENT)
            {
                s_DISPLAY_EVENT *v = (s_DISPLAY_EVENT *)pValue;
                sE.display.index = v->index;
                sE.display.value = v->value;
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
*/
            sEvent[i] = sE;
        }
    }
}

bool EVENT_HasEvent(uint8_t TaskId)
{
    bool val = false;
    for (int i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == TaskId)
        {
            if (sEvent[i].type != (uint32_t)NULL)
            {
                val = true;
            }
        }
    }
    return val;
}

s_EVENT *EVENT_GetEvent(uint8_t TaskId)
{
    s_EVENT *pEvent = NULL;
    for (int i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == TaskId)
        {
            pEvent = &sEvent[i];
        }
    }
    return pEvent;
}