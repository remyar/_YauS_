#include "arch_uart.h"

UART_HandleTypeDef huart1;
static uint8_t dataHuart1;

void ARCH_UartInit(uint32_t uartNum, unsigned long baud, PTR_LL_CALLBACK_FUNC ptrCallback)
{
    UART_HandleTypeDef *huart;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (uartNum == (uint32_t)USART1)
    {
        /* Peripheral clock enable */
        if (__HAL_RCC_USART1_IS_CLK_DISABLED())
            __HAL_RCC_USART1_CLK_ENABLE();
        if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
            __HAL_RCC_GPIOA_CLK_ENABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        huart = &huart1;
    }
    else
    {
        Error_Handler();
    }

    huart->Instance = (USART_TypeDef *)uartNum;
    huart->Init.BaudRate = baud;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(huart) != HAL_OK)
    {
        Error_Handler();
    }

    if (uartNum == (uint32_t)USART1)
    {
        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_UART_Receive_IT(huart, &dataHuart1, 1);
    }
    else
    {
        Error_Handler();
    }
}

void ARCH_UartSendByteSync(uint32_t uartNum, uint8_t data) {
    if (uartNum == (uint32_t)USART1)
    {
        HAL_UART_Transmit(&huart1, &data, 1 , 10);
    }
    else
    {
        Error_Handler();
    }
}

void ARCH_UartSendByte(uint32_t uartNum, uint8_t data)
{
    if (uartNum == (uint32_t)USART1)
    {
        //-- wait end of last character finish to send
        while ( HAL_UART_Transmit_IT(&huart1, &data, 1) != HAL_OK );
    }
    else
    {
        Error_Handler();
    }
}

//-- rx received byte callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        s_MSG_UART uart1Data;
        uart1Data.length = 1;
        uart1Data.data[0] = dataHuart1;
        YAUS_msgSend(YAUS_QUEUE_UART1_RX_HANDLE, &uart1Data);
        HAL_UART_Receive_IT(huart, &dataHuart1, 1);
    }
}