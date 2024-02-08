//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// MODULE       : Tick.c
// DESCRIPTION  : Gestion du Tick periodique
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include <_YauS_.h>
#include "DRIVERS_Task.h"
#include <stdarg.h>
#include <math.h>

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables globales
//-----------------------------------------------------------------------------
__attribute__((weak)) void ARCH_Uart1SendByte(uint8_t data)
{
}

//-----------------------------------------------------------------------------
// FONCTION    : _Init
//
// DESCRIPTION : Init de la tache
//-----------------------------------------------------------------------------
static bool _Init(void)
{
    return true;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _Run
//
// DESCRIPTION :  Appel periodique
//-----------------------------------------------------------------------------
static void _Run(void)
{
    s_MSG_UART uart1Data;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_UART1_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_UART1_TX_HANDLE, &uart1Data))
        {
            for (uint8_t i = 0; i < uart1Data.length ; i++ ){
                ARCH_Uart1SendByte(uart1Data.data[i]);
            }
        }
    }
}

void DRIVERS_Process(void)
{
    _Run();
}

void DRIVERS_Init(void)
{
    YAUS_TaskCreate("MODULE-Driver", (void *)_Init, (void *)_Run, 1, 0);
}

