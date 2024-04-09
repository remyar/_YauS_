#include "arch_vspi.h"

typedef struct
{
    GPIO_TypeDef *GPIO_PortStruct;
    GPIO_InitTypeDef GPIO_PinStruct;
    uint32_t tempo;
} VSPIPINSTRUCT;

static VSPIPINSTRUCT mosiVSPI1PinStruct;
static VSPIPINSTRUCT misoVSPI1PinStruct;
static VSPIPINSTRUCT ssVSPI1PinStruct;
static VSPIPINSTRUCT sclVSPI1PinStruct;

static void _delay_us(uint8_t periphNum)
{
    switch (periphNum)
    {
    case (VSPI1):
        for (int i = 0; i < sclVSPI1PinStruct.tempo; i++)
        {
            __NOP();
        }
        break;
    }
}

static void _setClockLevel(uint8_t periphNum, bool state)
{
    switch (periphNum)
    {
    case (VSPI1):
        HAL_GPIO_WriteFastPin(sclVSPI1PinStruct.GPIO_PortStruct, sclVSPI1PinStruct.GPIO_PinStruct.Pin, state);
        break;
    }
}

static void _setSsLevel(uint8_t periphNum, bool state)
{
    switch (periphNum)
    {
    case (VSPI1):
        HAL_GPIO_WriteFastPin(ssVSPI1PinStruct.GPIO_PortStruct, ssVSPI1PinStruct.GPIO_PinStruct.Pin, state);
        break;
    }
}

static void _setDataLevel(uint8_t periphNum, bool state)
{
    switch (periphNum)
    {
    case (VSPI1):
        HAL_GPIO_WriteFastPin(mosiVSPI1PinStruct.GPIO_PortStruct, mosiVSPI1PinStruct.GPIO_PinStruct.Pin, state);
        break;
    }
}

static bool _getDataLevel(uint8_t periphNum)
{
    bool result = false;
    switch (periphNum)
    {
    case (VSPI1):
        result = HAL_GPIO_ReadPin(misoVSPI1PinStruct.GPIO_PortStruct, misoVSPI1PinStruct.GPIO_PinStruct.Pin);
        break;
    }
    return result;
}

uint8_t _sendAndReceiveByte(uint8_t periphNum, uint8_t data)
{
    uint8_t i;
    uint8_t rData = 0;
    // Ecriture bit a bit, MSB vers LSB
    for (i = 0; i < 8; i++)
    {
        rData = rData << 1;
        _delay_us(periphNum);

        _setDataLevel(periphNum, (data & 0x80) == 0x80);
        _delay_us(periphNum);
        _setClockLevel(periphNum, GPIO_PIN_SET);

        _delay_us(periphNum);
        rData |= _getDataLevel(periphNum);
        _delay_us(periphNum);
        _setClockLevel(periphNum, GPIO_PIN_RESET); //--- Genere une impulsion
        _delay_us(periphNum);

        data <<= 1; //--- Bit suivant
    }
    _setClockLevel(periphNum, GPIO_PIN_RESET);

    return rData;
}

void ARCH_VSPIInit(uint8_t periphNum,
                   uint32_t portScl, uint32_t pinScl,
                   uint32_t portMosi, uint32_t pinMosi,
                   uint32_t portMiso, uint32_t pinMiso,
                   uint32_t portSs, uint32_t pinSs,
                   unsigned long speed, uint32_t flags)
{
    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
        __HAL_RCC_GPIOB_CLK_ENABLE();
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
        __HAL_RCC_GPIOA_CLK_ENABLE();

    switch (periphNum)
    {
    case (VSPI1):
        if ((portScl != (uint32_t)NULL) && (pinScl != (uint32_t)NULL))
        {
            sclVSPI1PinStruct.GPIO_PortStruct = (GPIO_TypeDef *)portScl;
            sclVSPI1PinStruct.GPIO_PinStruct.Pin = pinScl;
            sclVSPI1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_PP;
            sclVSPI1PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            sclVSPI1PinStruct.tempo = (1000000 / (speed * 2)) - 1;
            HAL_GPIO_Init(sclVSPI1PinStruct.GPIO_PortStruct, &sclVSPI1PinStruct.GPIO_PinStruct);
        }
        if ((portMosi != (uint32_t)NULL) && (pinMosi != (uint32_t)NULL))
        {
            mosiVSPI1PinStruct.GPIO_PortStruct = (GPIO_TypeDef *)portMosi;
            mosiVSPI1PinStruct.GPIO_PinStruct.Pin = pinMosi;
            mosiVSPI1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_PP;
            mosiVSPI1PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            mosiVSPI1PinStruct.tempo = (1000000 / (speed * 2)) - 1;
            HAL_GPIO_Init(mosiVSPI1PinStruct.GPIO_PortStruct, &mosiVSPI1PinStruct.GPIO_PinStruct);
        }
        if ((portMiso != (uint32_t)NULL) && (pinMiso != (uint32_t)NULL))
        {
            misoVSPI1PinStruct.GPIO_PortStruct = (GPIO_TypeDef *)portMiso;
            misoVSPI1PinStruct.GPIO_PinStruct.Pin = pinMiso;
            misoVSPI1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_INPUT;
            misoVSPI1PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            misoVSPI1PinStruct.tempo = (1000000 / (speed * 2)) - 1;
            HAL_GPIO_Init(misoVSPI1PinStruct.GPIO_PortStruct, &misoVSPI1PinStruct.GPIO_PinStruct);
        }
        if ((portSs != (uint32_t)NULL) && (pinSs != (uint32_t)NULL))
        {
            ssVSPI1PinStruct.GPIO_PortStruct = (GPIO_TypeDef *)portSs;
            ssVSPI1PinStruct.GPIO_PinStruct.Pin = pinSs;
            ssVSPI1PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_PP;
            ssVSPI1PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            ssVSPI1PinStruct.tempo = (1000000 / (speed * 2)) - 1;
            HAL_GPIO_Init(ssVSPI1PinStruct.GPIO_PortStruct, &ssVSPI1PinStruct.GPIO_PinStruct);
            HAL_GPIO_WriteFastPin(ssVSPI1PinStruct.GPIO_PortStruct, ssVSPI1PinStruct.GPIO_PinStruct.Pin, GPIO_PIN_SET);
        }
        break;
    }
}

void ARCH_VSPI1Init(uint32_t portScl, uint32_t pinScl,
                    uint32_t portMosi, uint32_t pinMosi,
                    uint32_t portMiso, uint32_t pinMiso,
                    uint32_t portSs, uint32_t pinSs,
                    unsigned long speed, uint32_t flags)
{
    ARCH_VSPIInit(VSPI1, portScl, pinScl, portMosi, pinMosi, portMiso, pinMiso, portSs, pinSs, speed, flags);
}

void ARCH_VSPISendBytes(uint8_t periphNum, uint8_t *pData, uint8_t len)
{
    switch (periphNum)
    {
    case VSPI1:
        if ((ssVSPI1PinStruct.GPIO_PortStruct != NULL) && (ssVSPI1PinStruct.GPIO_PinStruct.Pin != (uint32_t)NULL))
        {
            _setSsLevel(periphNum, GPIO_PIN_RESET);
        }
        for (uint16_t i = 0; i < len; i++)
        {
            pData[i] = _sendAndReceiveByte(periphNum, pData[i]);
        }

        if ((ssVSPI1PinStruct.GPIO_PortStruct != NULL) && (ssVSPI1PinStruct.GPIO_PinStruct.Pin != (uint32_t)NULL))
        {
            _setSsLevel(periphNum, GPIO_PIN_SET);
        }
        break;
    }
}

void ARCH_VSPI1SendBytes(uint8_t *pData, uint8_t len)
{
    ARCH_VSPISendBytes(VSPI1, pData, len);
}