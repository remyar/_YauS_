#include "arch_i2c.h"

I2C_HandleTypeDef hi2c2;

void ARCH_I2CInit(uint32_t periphNum, unsigned long freq, uint32_t flags)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (periphNum == (uint32_t)I2C2)
    {
        /* Peripheral clock enable */
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();

        /* Peripheral clock enable */
        if (__HAL_RCC_I2C2_IS_CLK_DISABLED())
            __HAL_RCC_I2C2_CLK_ENABLE();

        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        hi2c2.Instance = I2C2;
        hi2c2.Init.ClockSpeed = freq;
        hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
        hi2c2.Init.OwnAddress1 = 0;
        hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        hi2c2.Init.OwnAddress2 = 0;
        hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        if (HAL_I2C_Init(&hi2c2) != HAL_OK)
        {
            Error_Handler();
        }

        /* I2C2 interrupt Init */
        HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
    }
}

void ARCH_I2CSendBytes(uint32_t periphNum, uint8_t addr, uint8_t *pData, uint8_t length, bool stop)
{
    if (periphNum == (uint32_t)I2C2)
    {
        //-- stop not used
        if ((addr & I2C_READ) == false)
        {
            HAL_I2C_Master_Transmit(&hi2c2, addr, pData, length, 100);
        }
        if ((addr & I2C_READ) == true)
        {
            if (HAL_I2C_Master_Receive(&hi2c2, addr, pData, length, 100) == HAL_OK)
            {
            }
            else
            {
            }
        }
    }
    if (periphNum == (uint32_t)VI2C1)
    {
        ARCH_VI2CSendBytes(periphNum, addr, pData, length, stop);
    }
}

void ARCH_I2C2Init(uint32_t freq, uint32_t flags)
{
    ARCH_I2CInit((uint32_t)I2C2, freq, flags);
}

void ARCH_I2C2SendBytes(uint8_t addr, uint8_t *pData, uint8_t length)
{
    ARCH_I2CSendBytes((uint32_t)I2C2, addr, pData, length, NULL);
}