#include "drivers.h"

uint32_t taskDriverHandle = NULL;
#ifdef LL_USE_UART_1
void LLUART1_ReceivedBytes(uint8_t addr, uint8_t *data, uint8_t len)
{
    s_MSG_UART sMsgUart;
    sMsgUart.data[0] = data[0];
    sMsgUart.length = len;
    YAUS_msgSend(YAUS_QUEUE_UART1_RX_HANDLE, &sMsgUart);
}
#endif

#ifdef LL_USE_I2C_2
void LLI2C2_ReceivedBytes(uint8_t addr, uint8_t *pData, uint8_t len)
{
    s_MSG_I2C sMsgi2c;
    memcpy(sMsgi2c.data, pData, len);
    sMsgi2c.length = len;
    sMsgi2c.addr = addr;
    YAUS_msgSend(YAUS_QUEUE_I2C2_RX_HANDLE, &sMsgi2c);
}
#endif

#ifdef LL_USE_VIRTUAL_I2C1
void LLVI2C1_ReceivedBytes(uint8_t addr, uint8_t *pData, uint8_t len)
{
    s_MSG_I2C sMsgi2c;
    memcpy(sMsgi2c.data, pData, len);
    sMsgi2c.length = len;
    sMsgi2c.addr = addr;
    YAUS_msgSend(YAUS_QUEUE_VI2C1_RX_HANDLE, &sMsgi2c);
}
#endif

void LLADC_ReceivedBytes(uint8_t addr, uint8_t *pData, uint8_t len)
{
    s_MSG_I2C sMsgi2c;
    memcpy(sMsgi2c.data, pData, len);
    sMsgi2c.length = len;
    sMsgi2c.addr = addr;
    YAUS_msgSend(YAUS_QUEUE_VI2C1_RX_HANDLE, &sMsgi2c);
}

//-----------------------------------------------------------------------------
// FONCTION    : _Init
//
// DESCRIPTION : Init de la tache
//-----------------------------------------------------------------------------
static bool _Init(void)
{
#ifdef LL_USE_UART_1
    LLUART_Init(LL_UART1, 115200, LLUART1_ReceivedBytes);
#endif
#ifdef LL_USE_I2C_2
    LLI2C_Init(LL_I2C2, 400000, LLI2C2_ReceivedBytes);
#endif
#ifdef LL_USE_VIRTUAL_I2C1
    LLVI2C_Init(LL_VI2C1, GPIOB, GPIO_PIN_14, GPIOB, GPIO_PIN_13, 400000, LLVI2C1_ReceivedBytes);
#endif
#ifdef LL_USE_PWM_PB0
    LLPWM_Init(LL_PWM_PB0, 1000);
#endif
#ifdef LL_USE_ADC_PA0
    LLADC_Init(LL_ADC_PA0);
#endif
#ifdef LL_USE_ADC_PA1
    LLADC_Init(LL_ADC_PA1);
#endif
    return true;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _Run
//
// DESCRIPTION :  Appel periodique
//-----------------------------------------------------------------------------
static void _Run(void)
{
#ifdef LL_USE_UART_1
    s_MSG_UART uart1Data;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_UART1_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_UART1_TX_HANDLE, &uart1Data))
        {
            LLUART_SendByte(LL_UART1, uart1Data.data[0]);
        }
    }
#endif
#ifdef LL_USE_I2C_2
    s_MSG_I2C sMsgI2c;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_I2C2_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_I2C2_TX_HANDLE, &sMsgI2c))
        {
            LLI2C_SendBytes(LL_I2C2, sMsgI2c.addr, &sMsgI2c.data, sMsgI2c.length);
        }
    }
#endif
#ifdef LL_USE_VIRTUAL_I2C1
    s_MSG_I2C sMsgVI2c;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_VI2C1_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_VI2C1_TX_HANDLE, &sMsgI2c))
        {
            LLVI2C_SendBytes(LL_VI2C1, sMsgI2c.addr, &sMsgI2c.data, sMsgI2c.length, sMsgI2c.stop);
        }
    }
#endif
#ifdef LL_USE_PWM_PB0
    s_MSG_PWM sMsgPwm;
    if (YAUS_msgGetNbElement(YAUS_QUEUE_PWMPB0_TX_HANDLE) > 0)
    {
        if (YAUS_msgRead(YAUS_QUEUE_PWMPB0_TX_HANDLE, &sMsgPwm))
        {
            LLPWM_SetDutyCycle(LL_PWM_PB0, sMsgPwm.dutyCycle);
        }
    }
#endif
#if defined(LL_USE_ADC_PA0) || defined(LL_USE_ADC_PA1)
    LLADC_process();
#endif
}

void DRIVERS_Init(void)
{
    taskDriverHandle = YAUS_TaskCreate("MODULE_Drivers", (void *)_Init, (void *)_Run, 1, 0);
}

void DRIVERS_ProcessImmediat(void)
{
    YAUS_TaskForce(taskDriverHandle);
}