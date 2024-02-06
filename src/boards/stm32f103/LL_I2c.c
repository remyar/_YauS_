
#include "LL_I2c.h"

#ifdef LL_USE_I2C_2
static I2C_HandleTypeDef hi2c2;
static PTR_LL_CALLBACK_FUNC hi2c2RxCallback = NULL;
#endif

void LLI2C_Init(uint8_t periphNum, unsigned long speed, PTR_LL_CALLBACK_FUNC ptrCallback)
{
    I2C_HandleTypeDef *hi2c;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    switch (periphNum)
    {
#ifdef LL_USE_I2C_2
    case LL_I2C2:
        hi2c = &hi2c2;
        hi2c->Instance = I2C2;
        hi2c->Init.ClockSpeed = speed;
        hi2c2RxCallback = ptrCallback;

        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        if (__HAL_RCC_I2C2_IS_CLK_DISABLED())
            __HAL_RCC_I2C2_CLK_ENABLE();

        break;
#endif
#ifdef LL_USE_I2C_2

#endif
    }

    hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c->Init.OwnAddress1 = 0;
    hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c->Init.OwnAddress2 = 0;
    hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(hi2c) != HAL_OK)
    {
        HardFault_Handler();
    }

    switch (periphNum)
    {
#ifdef LL_USE_I2C_2
    case LL_I2C2:
        /* I2C_2 interrupt Init */
        HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
        break;
#endif
    default:
        break;
    }
}

void LLI2C_SendBytes(uint8_t periphNum, uint8_t addr, uint8_t *pData, uint8_t len)
{
    switch (periphNum)
    {
#ifdef LL_USE_I2C_2
    case LL_I2C2:
        if ((addr & I2C_READ) == false)
        {
            HAL_I2C_Master_Transmit(&hi2c2, addr, pData, len, 100);
        }
        if ((addr & I2C_READ) == true)
        {
            HAL_I2C_Master_Receive(&hi2c2, addr, pData, len, 100);
            hi2c2RxCallback(((addr >> 1) & 0x7F), pData, len);
        }
        break;
#endif
    }
}

#ifdef LL_USE_I2C_2
/**
 * @brief This function handles I2C2 event interrupt.
 */
void I2C2_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hi2c2);
}

/**
 * @brief This function handles I2C2 error interrupt.
 */
void I2C2_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hi2c2);
}
#endif