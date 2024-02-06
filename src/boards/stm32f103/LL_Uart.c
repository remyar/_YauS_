#include "LL_Uart.h"

#ifdef LL_USE_UART_1
static UART_HandleTypeDef huart1;
static uint8_t dataHuart1;
static PTR_LL_CALLBACK_FUNC huart1RxCallback = NULL;
#endif

void LLUART_Init(uint8_t uartNum, unsigned long baud, PTR_LL_CALLBACK_FUNC ptrCallback)
{
    UART_HandleTypeDef *huart;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    switch (uartNum)
    {
#ifdef LL_USE_UART_1
    case LL_UART1:

        /* Peripheral clock enable */
        if (__HAL_RCC_USART1_IS_CLK_DISABLED())
            __HAL_RCC_USART1_CLK_ENABLE();
        if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
            __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */

        huart = &huart1;
        huart->Instance = USART1;
        huart1RxCallback = ptrCallback;

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        break;
#endif
#ifdef LL_USE_UART_2
    case LL_UART2:
        huart = &huart2;
        huart->Instance = USART2;
        break;
#endif
#ifdef LL_USE_UART_3
    case LL_UART3:
        huart = &huart1;
        huart->Instance = USART3;
        break;
#endif
    default:
        printf("NO LL_UART AVAILABLE");
        return;
    }

    huart->Init.BaudRate = baud;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(huart) != HAL_OK)
    {
        HardFault_Handler();
    }

    switch (uartNum)
    {
#ifdef LL_USE_UART_1
    case LL_UART1:
        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_UART_Receive_IT(huart, &dataHuart1, 1);
        break;
#endif
#ifdef LL_USE_UART_2
    case LL_UART2:
        break;
#endif
#ifdef LL_USE_UART_3
    case LL_UART3:
        break;
#endif
    default:
        break;
    }
}

void LLUART_SendByte(uint8_t uartNum, uint8_t data)
{
    switch (uartNum)
    {
#ifdef LL_USE_UART_1
    case LL_UART1:
        HAL_UART_Transmit_IT(&huart1, &data, 1);
        break;
#endif
    }
}

#ifdef LL_USE_UART_1
/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}
#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef LL_USE_UART_1
    if (huart->Instance == USART1)
    {
        huart1RxCallback(NULL, &dataHuart1, 1);
        HAL_UART_Receive_IT(huart, &dataHuart1, 1);
    }
#endif
    return;
}
