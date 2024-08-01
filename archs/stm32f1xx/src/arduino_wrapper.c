#include "arduino_wrapper.h"

static GPIO_TypeDef *_pinToPort(uint16_t pinNum)
{
    GPIO_TypeDef *GPIOx = NULL;
    if ((pinNum >= PA0) && (pinNum <= PA15))
    {
        GPIOx = GPIOA;
    }
    else if ((pinNum >= PB0) && (pinNum <= PB15))
    {
        GPIOx = GPIOB;
    }
    else if ((pinNum >= PC13) && (pinNum <= PC15))
    {
        GPIOx = GPIOC;
    }
    else if ((pinNum >= PD0) && (pinNum <= PD1))
    {
        GPIOx = GPIOD;
    }

    return GPIOx;
}

static uint32_t _pinToPin(uint16_t pinNum)
{
    uint32_t pin = NULL;

    switch (pinNum)
    {
    case (PA0):
    {
        pin = GPIO_PIN_0;
        break;
    }
    case (PA1):
    {
        pin = GPIO_PIN_1;
        break;
    }
    case (PA2):
    {
        pin = GPIO_PIN_2;
        break;
    }
    case (PA3):
    {
        pin = GPIO_PIN_3;
        break;
    }
    case (PA4):
    {
        pin = GPIO_PIN_4;
        break;
    }
    case (PA5):
    {
        pin = GPIO_PIN_5;
        break;
    }
    case (PA6):
    {
        pin = GPIO_PIN_6;
        break;
    }
    case (PA7):
    {
        pin = GPIO_PIN_7;
        break;
    }
    case (PA8):
    {
        pin = GPIO_PIN_8;
        break;
    }
    case (PA9):
    {
        pin = GPIO_PIN_9;
        break;
    }
    case (PA10):
    {
        pin = GPIO_PIN_10;
        break;
    }
    case (PA11):
    {
        pin = GPIO_PIN_11;
        break;
    }
    case (PA12):
    {
        pin = GPIO_PIN_12;
        break;
    }
    case (PA13):
    {
        pin = GPIO_PIN_13;
        break;
    }
    case (PA14):
    {
        pin = GPIO_PIN_14;
        break;
    }
    case (PA15):
    {
        pin = GPIO_PIN_15;
        break;
    }
    case (PB0):
    {
        pin = GPIO_PIN_0;
        break;
    }
    case (PB1):
    {
        pin = GPIO_PIN_1;
        break;
    }
    case (PB2):
    {
        pin = GPIO_PIN_2;
        break;
    }
    case (PB3):
    {
        pin = GPIO_PIN_3;
        break;
    }
    case (PB4):
    {
        pin = GPIO_PIN_4;
        break;
    }
    case (PB5):
    {
        pin = GPIO_PIN_5;
        break;
    }
    case (PB6):
    {
        pin = GPIO_PIN_6;
        break;
    }
    case (PB7):
    {
        pin = GPIO_PIN_7;
        break;
    }
    case (PB8):
    {
        pin = GPIO_PIN_8;
        break;
    }
    case (PB9):
    {
        pin = GPIO_PIN_9;
        break;
    }
    case (PB10):
    {
        pin = GPIO_PIN_10;
        break;
    }
    case (PB11):
    {
        pin = GPIO_PIN_11;
        break;
    }
    case (PB12):
    {
        pin = GPIO_PIN_12;
        break;
    }
    case (PB13):
    {
        pin = GPIO_PIN_13;
        break;
    }
    case (PB14):
    {
        pin = GPIO_PIN_14;
        break;
    }
    case (PB15):
    {
        pin = GPIO_PIN_15;
        break;
    }
    case (PC13):
    {
        pin = GPIO_PIN_13;
        break;
    }
    case (PC14):
    {
        pin = GPIO_PIN_14;
        break;
    }
    case (PC15):
    {
        pin = GPIO_PIN_15;
        break;
    }
    case (PD0):
    {
        pin = GPIO_PIN_0;
        break;
    }
    case (PD1):
    {
        pin = GPIO_PIN_1;
        break;
    }
    }

    return pin;
}

inline void pinMode(uint16_t pinNum, uint8_t state)
{
    GPIO_InitTypeDef GPIO_InitStruct = {.Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_HIGH};
    GPIO_TypeDef *GPIOx = _pinToPort(pinNum);

    switch (state)
    {
    case (OUTPUT):
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        break;
    }
    case (INTPUT):
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        break;
    }
    case (INPUT_PULLUP):
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        break;
    }
    case (INPUT_PULLDOWN):
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        break;
    }
    }

    GPIO_InitStruct.Pin = _pinToPin(pinNum);

    switch ((uint32_t)GPIOx)
    {
    case ((uint32_t)GPIOA):
    {
        if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
            __HAL_RCC_GPIOA_CLK_ENABLE();
        break;
    }
    case ((uint32_t)GPIOB):
    {
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();
        break;
    }
    case ((uint32_t)GPIOC):
    {
        if (__HAL_RCC_GPIOC_IS_CLK_DISABLED())
            __HAL_RCC_GPIOC_CLK_ENABLE();
        break;
    }
    case ((uint32_t)GPIOD):
    {
        if (__HAL_RCC_GPIOD_IS_CLK_DISABLED())
            __HAL_RCC_GPIOD_CLK_ENABLE();
        break;
    }
    }

    if ((GPIOx != NULL) && (GPIO_InitStruct.Pin != NULL))
    {
        HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
    }
}

inline void digitalWrite(uint16_t pinNum , uint8_t state){
    HAL_GPIO_WriteFastPin(_pinToPort(pinNum), _pinToPin(pinNum), state == LOW ? GPIO_PIN_RESET : GPIO_PIN_SET);
}