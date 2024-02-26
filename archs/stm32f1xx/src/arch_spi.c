#include "arch_spi.h"

SPI_HandleTypeDef hspi1;

void ARCH_SpiInit(uint32_t periphNum, unsigned long freq, uint32_t flags)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (periphNum == (uint32_t)SPI1)
    {
        /* Peripheral clock enable */
        if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
            __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Peripheral clock enable */
        if (__HAL_RCC_SPI1_IS_CLK_DISABLED())
            __HAL_RCC_SPI1_CLK_ENABLE();

        /**SPI GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        hspi1.Instance = SPI1;
        hspi1.Init.Mode = SPI_MODE_MASTER;
        hspi1.Init.Direction = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi1.Init.NSS = SPI_NSS_SOFT;
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi1.Init.CRCPolynomial = 10;
        if (HAL_SPI_Init(&hspi1) != HAL_OK)
        {
            Error_Handler();
        }
        /* SPI1 interrupt Init */
        // HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
}

uint8_t ARCH_SpiSendByte(uint32_t periphNum, uint8_t data)
{
    uint8_t rxData = 0;
    if (periphNum == (uint32_t)SPI1)
    {
        if (hspi1.Instance != NULL)
        {
            while (HAL_SPI_TransmitReceive(&hspi1, &data, &rxData, 1, 50) != HAL_OK)
                ;
        }
    }
    return rxData;
}

uint8_t ARCH_Spi1SendByte(uint8_t data)
{
    return ARCH_SpiSendByte(SPI1, data);
}