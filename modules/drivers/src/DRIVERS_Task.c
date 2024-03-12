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
__attribute__((weak)) bool ARCH_Uart1Available(void)
{
}

__attribute__((weak)) bool ARCH_GetUart1UseRs485(void)
{
}

__attribute__((weak)) uint8_t ARCH_Uart1Read(void)
{
}

__attribute__((weak)) void ARCH_Uart1SendByte(uint8_t data)
{
}
__attribute__((weak)) void ARCH_Uart2SendByte(uint8_t data)
{
}
__attribute__((weak)) void ARCH_Uart3SendByte(uint8_t data)
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

__attribute__((weak)) uint8_t ARCH_Spi1SendByte(uint8_t data)
{
}

__attribute__((weak)) void ARCH_RS4851SendByteSync(uint8_t data)
{
}

__attribute__((weak)) void ARCH_VSPI1SendBytes(uint8_t *Buf, uint16_t Len)
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
    while (ARCH_Uart1Available() > 0)
    {
        uart1Data.length = 1;
        uart1Data.data[0] = ARCH_Uart1Read();
        YAUS_msgSend(ARCH_GetUart1UseRs485() ? YAUS_QUEUE_RS4851_RX_HANDLE : YAUS_QUEUE_UART1_RX_HANDLE, &uart1Data);
    }

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

    s_MSG_UART uart2Data;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_UART2_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_UART2_TX_HANDLE, &uart2Data))
        {
            for (uint8_t i = 0; i < uart2Data.length; i++)
            {
                ARCH_Uart2SendByte(uart2Data.data[i]);
            }
        }
    }

    s_MSG_UART uart3Data;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_UART3_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_UART3_TX_HANDLE, &uart3Data))
        {
            for (uint8_t i = 0; i < uart3Data.length; i++)
            {
                ARCH_Uart3SendByte(uart3Data.data[i]);
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

    s_MSG_UART rs4851Data;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_RS4851_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_RS4851_TX_HANDLE, &rs4851Data))
        {
            for (uint8_t i = 0; i < rs4851Data.length; i++)
            {
                ARCH_RS4851SendByteSync(rs4851Data.data[i]);
            }
        }
    }

    s_MSG_SPI sMsgSpi1;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_SPI1_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_SPI1_TX_HANDLE, &sMsgSpi1))
        {
            for (uint8_t i = 0; i < sMsgSpi1.length; i++)
            {
                sMsgSpi1.data[i] = ARCH_Spi1SendByte(sMsgSpi1.data[i]);
            }
            YAUS_msgSend(YAUS_QUEUE_SPI1_RX_HANDLE, &sMsgSpi1);
        }
    }

    s_MSG_SPI sMsgVSpi1;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_VSPI1_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_VSPI1_TX_HANDLE, &sMsgVSpi1))
        {
            ARCH_VSPI1SendBytes(sMsgVSpi1.data, sMsgVSpi1.length);
            YAUS_msgSend(YAUS_QUEUE_VSPI1_RX_HANDLE, &sMsgVSpi1);
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
