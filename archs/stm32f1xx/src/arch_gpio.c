#include "arch_gpio.h"

inline void HAL_GPIO_WriteFastPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    /* Check the parameters */
    (PinState != GPIO_PIN_RESET) ? (GPIOx->BSRR = GPIO_Pin) : (GPIOx->BRR = (uint32_t)GPIO_Pin);
}