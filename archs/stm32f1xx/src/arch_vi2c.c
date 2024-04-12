#include "arch_vi2c.h"

typedef struct
{
    GPIO_TypeDef *GPIO_PortStruct;
    GPIO_InitTypeDef GPIO_PinStruct;
    uint32_t tempo;
} VI2CPINSTRUCT;

static VI2CPINSTRUCT sdaVI2C1PinStruct;
static VI2CPINSTRUCT sclVI2C1PinStruct;

static void _delay_us(uint8_t periphNum)
{
    switch (periphNum)
    {
    case (VI2C1):
        for (int i = 0; i < sclVI2C1PinStruct.tempo; i++)
        {
            __NOP();
        }
        break;
    }
}

static void _setDataIn(uint8_t periphNum)
{
    switch (periphNum)
    {
    case (VI2C1):
        sdaVI2C1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(sdaVI2C1PinStruct.GPIO_PortStruct, &sdaVI2C1PinStruct.GPIO_PinStruct);
    }
}

static void _setDataOut(uint8_t periphNum)
{
    switch (periphNum)
    {
    case (VI2C1):
        sdaVI2C1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_OD;
        HAL_GPIO_Init(sdaVI2C1PinStruct.GPIO_PortStruct, &sdaVI2C1PinStruct.GPIO_PinStruct);
    }
}

static void _setClockLevel(uint8_t periphNum, bool state)
{
    switch (periphNum)
    {
    case (VI2C1):
        HAL_GPIO_WriteFastPin(sclVI2C1PinStruct.GPIO_PortStruct, sclVI2C1PinStruct.GPIO_PinStruct.Pin, state);
        break;
    }
}

static void _setDataLevel(uint8_t periphNum, bool state)
{
    switch (periphNum)
    {
    case (VI2C1):
        HAL_GPIO_WriteFastPin(sdaVI2C1PinStruct.GPIO_PortStruct, sdaVI2C1PinStruct.GPIO_PinStruct.Pin, state);
        break;
    }
}

static bool _getDataLevel(uint8_t periphNum)
{
    bool result = false;
    switch (periphNum)
    {
    case (VI2C1):
        result = HAL_GPIO_ReadPin(sdaVI2C1PinStruct.GPIO_PortStruct, sdaVI2C1PinStruct.GPIO_PinStruct.Pin);
        break;
    }
    return result;
}

static bool _readAck(uint8_t periphNum)
{
    uint8_t status;

    _setDataLevel(periphNum, GPIO_PIN_SET);
    _setDataIn(periphNum);

    _delay_us(periphNum);
    _setClockLevel(periphNum, GPIO_PIN_SET);
    _delay_us(periphNum);
    status = (uint8_t)(_getDataLevel(periphNum));
    _setClockLevel(periphNum, GPIO_PIN_RESET);

    _setDataOut(periphNum);

    return (bool)(status == 0);
}

static void _sendAck(uint8_t periphNum, bool ack)
{
    _setDataLevel(periphNum, ack ? GPIO_PIN_RESET : GPIO_PIN_SET);
    _setDataOut(periphNum);
    _delay_us(periphNum);
    _setClockLevel(periphNum, GPIO_PIN_SET);
    _delay_us(periphNum);
    _setDataLevel(periphNum, GPIO_PIN_RESET);
    _setClockLevel(periphNum, GPIO_PIN_RESET);
}

bool _sendByte(uint8_t periphNum, uint8_t data)
{
    uint8_t i;
    _setDataOut(periphNum); //--- Data en sortie

    // Ecriture bit a bit, MSB vers LSB
    for (i = 0; i < 8; i++)
    {
        _setClockLevel(periphNum, GPIO_PIN_RESET);
        _setDataLevel(periphNum, (data & 0x80) == 0x80);
        _delay_us(periphNum);
        _setClockLevel(periphNum, GPIO_PIN_SET); //--- Genere une impulsion
        _delay_us(periphNum);
        data <<= 1; //--- Bit suivant
    }
    _setClockLevel(periphNum, GPIO_PIN_RESET);

    return _readAck(periphNum);
}

uint8_t _readByte(uint8_t periphNum, bool ackEnabled)
{
    uint8_t i;
    uint8_t localData = 0;

    _setDataIn(periphNum);
    for (i = 0; i < 8; i++)
    {
        localData <<= 1;
        _setClockLevel(periphNum, GPIO_PIN_RESET);
        _delay_us(periphNum);
        _setClockLevel(periphNum, GPIO_PIN_SET);
        _delay_us(periphNum);
        localData |= _getDataLevel(periphNum); //--- Reception du bit
    }
    _setClockLevel(periphNum, GPIO_PIN_RESET);
    _sendAck(periphNum, ackEnabled);
    return localData;
}

static void _start(uint8_t periphNum)
{
    _setDataOut(periphNum);
    _setDataLevel(periphNum, GPIO_PIN_SET);
    _delay_us(periphNum);
    _setClockLevel(periphNum, GPIO_PIN_SET);
    _delay_us(periphNum);
    _setDataLevel(periphNum, GPIO_PIN_RESET);
    _delay_us(periphNum);
    _setClockLevel(periphNum, GPIO_PIN_RESET);
    _delay_us(periphNum);
}

static void _stop(uint8_t periphNum)
{
    _setDataOut(periphNum);
    _setClockLevel(periphNum, GPIO_PIN_RESET);
    _setDataLevel(periphNum, GPIO_PIN_RESET);
    _delay_us(periphNum);
    _setClockLevel(periphNum, GPIO_PIN_SET);
    _delay_us(periphNum);
    _setDataLevel(periphNum, GPIO_PIN_SET);
}

void ARCH_VI2CInit(uint8_t periphNum, uint32_t portScl, uint32_t pinScl, uint32_t portSda, uint32_t pinSda, unsigned long speed, uint32_t flags)
{
    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
        __HAL_RCC_GPIOB_CLK_ENABLE();
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
        __HAL_RCC_GPIOA_CLK_ENABLE();

    switch (periphNum)
    {
    case (VI2C1):

        sdaVI2C1PinStruct.GPIO_PortStruct = (GPIO_TypeDef *)portSda;
        sdaVI2C1PinStruct.GPIO_PinStruct.Pin = pinSda;
        sdaVI2C1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_OD;
        sdaVI2C1PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(sdaVI2C1PinStruct.GPIO_PortStruct, &sdaVI2C1PinStruct.GPIO_PinStruct);

        sclVI2C1PinStruct.GPIO_PortStruct = (GPIO_TypeDef *)portScl;
        sclVI2C1PinStruct.GPIO_PinStruct.Pin = pinScl;
        sclVI2C1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_OD;
        sclVI2C1PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        sclVI2C1PinStruct.tempo = (1000000 / (speed * 2)) - 1;
        HAL_GPIO_Init(sclVI2C1PinStruct.GPIO_PortStruct, &sclVI2C1PinStruct.GPIO_PinStruct);

        _stop(VI2C1);
        break;
    }
}

void ARCH_VI2CSendBytes(uint8_t periphNum, uint8_t addr, uint8_t *pData, uint8_t len, bool stop)
{
    switch (periphNum)
    {
    case VI2C1:
        if ((addr & I2C_READ) == false)
        {
            _start(periphNum);
            _sendByte(periphNum, addr);
            for (uint8_t i = 0; i < len; i++)
            {
                _sendByte(periphNum, pData[i]);
            }
            if (stop == true)
                _stop(periphNum);
        }
        if ((addr & I2C_READ) == true)
        {
            bool sendAck = true;
            _start(periphNum);
            _sendByte(periphNum, addr);
            for (uint8_t i = 0; i < len; i++)
            {
                if (i == (len - 1))
                {
                    sendAck = false;
                }
                pData[i] = _readByte(periphNum, sendAck);
            }
            if (stop == true)
                _stop(periphNum);
            // HAL_I2C_Master_Receive(&hi2c2, addr, pData, len, 100);
/*
            s_MSG_I2C sMsg;
            sMsg.addr = ((addr >> 1) & 0x7F);
            sMsg.length = len;
            memcpy(sMsg.data, pData, len);
            YAUS_msgSend(YAUS_QUEUE_VI2C1_RX_HANDLE, &sMsg);*/
        }
        break;
    }
}

void ARCH_VI2C1Init(uint32_t portScl, uint32_t pinScl, uint32_t portSda, uint32_t pinSda, unsigned long speed, uint32_t flags)
{
    ARCH_VI2CInit(VI2C1, portScl, pinScl, portSda, pinSda, speed, flags);
}

void ARCH_VI2C1SendBytes(uint8_t addr, uint8_t *pData, uint8_t len, bool stop)
{
    ARCH_VI2CSendBytes(VI2C1, addr, pData, len, stop);
}
