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

__attribute__((weak)) void ARCH_I2C2SendBytes(uint8_t addr, uint8_t *pData, uint8_t length)
{
}

__attribute__((weak)) void ARCH_VI2C1SendBytes(uint8_t addr, uint8_t *pData, uint8_t length, bool stop)
{
}

__attribute__((weak)) void ARCH_Pwm1SetDutyCycle(float percent)
{
}

__attribute__((weak)) void ARCH_AdcProcess(void)
{
}

__attribute__((weak)) void CDC_Transmit_FS(uint8_t *Buf, uint16_t Len)
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
            for (uint8_t i = 0; i < uart1Data.length; i++)
            {
                ARCH_Uart1SendByte(uart1Data.data[i]);
            }
        }
    }

    s_MSG_USB usbData;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_USB_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_USB_TX_HANDLE, &usbData))
        {
            CDC_Transmit_FS(usbData.data, usbData.length);
        }
    }

    s_MSG_I2C sMsgI2c;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_I2C2_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_I2C2_TX_HANDLE, &sMsgI2c))
        {
            ARCH_I2C2SendBytes(sMsgI2c.addr, &sMsgI2c.data, sMsgI2c.length);
        }
    }

    s_MSG_I2C sMsgVI2c;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_VI2C1_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_VI2C1_TX_HANDLE, &sMsgVI2c))
        {
            ARCH_VI2C1SendBytes(sMsgVI2c.addr, &sMsgVI2c.data, sMsgVI2c.length, sMsgVI2c.stop);
        }
    }

    s_MSG_PWM sMsgPwm;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_PWMPB0_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_PWMPB0_TX_HANDLE, &sMsgPwm))
        {
            ARCH_Pwm1SetDutyCycle(sMsgPwm.dutyCycle);
        }
    }

    ARCH_AdcProcess();
}

void DRIVERS_Process(void)
{
    _Run();
}

void DRIVERS_Init(void)
{
    YAUS_TaskCreate("MODULE-Driver", (void *)_Init, (void *)_Run, 1, 0);
}
