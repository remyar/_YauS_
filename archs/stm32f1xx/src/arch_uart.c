#include "arch_uart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
static uint8_t dataHuart1;
static uint8_t dataHuart2;
static uint8_t dataHuart3;
static bool useRs4851 = false;
static bool useRs4852 = false;
static bool useRs4853 = false;

void ARCH_UartInit(USART_TypeDef *uartNum, unsigned long baud, uint32_t flags)
{
    UART_HandleTypeDef *huart;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (uartNum == USART1)
    {
        /* Peripheral clock enable */
        if (__HAL_RCC_USART1_IS_CLK_DISABLED())
            __HAL_RCC_USART1_CLK_ENABLE();

        if (Has_flag(flags, USE_ALTERNATE_FUNCTION))
        {
            if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
                __HAL_RCC_GPIOB_CLK_ENABLE();

            if (__HAL_RCC_AFIO_IS_CLK_DISABLED())
                __HAL_RCC_AFIO_CLK_ENABLE();
                
            /**USART1 GPIO Configuration
            PB6     ------> USART1_TX
            PB7     ------> USART1_RX
            */
            GPIO_InitStruct.Pin = GPIO_PIN_6;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = GPIO_PIN_7;
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

            __HAL_AFIO_REMAP_USART1_ENABLE();
        }
        else
        {
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
        }

        huart = &huart1;
    }
    else if (uartNum == USART2)
    {
    }
    else if (uartNum == USART3)
    {
        /* Peripheral clock enable */
        if (__HAL_RCC_USART3_IS_CLK_DISABLED())
            __HAL_RCC_USART3_CLK_ENABLE();
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();

        /** USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        huart = &huart3;
    }
    else
    {
        Error_Handler();
    }

    huart->Instance = uartNum;
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

    if (uartNum == USART1)
    {
        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_UART_Receive_IT(huart, &dataHuart1, 1);
    }
    else if (uartNum == USART2)
    {
        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        HAL_UART_Receive_IT(huart, &dataHuart2, 1);
    }
    else if (uartNum == USART3)
    {
        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        HAL_UART_Receive_IT(huart, &dataHuart3, 1);
    }
    else
    {
        Error_Handler();
    }
}

void ARCH_Uart1Init(uint32_t speed, uint32_t flags)
{
    ARCH_UartInit(USART1, speed, flags);
}
void ARCH_Uart2Init(uint32_t speed, uint32_t flags)
{
    ARCH_UartInit(USART2, speed, flags);
}
void ARCH_Uart3Init(uint32_t speed, uint32_t flags)
{
    ARCH_UartInit(USART3, speed, flags);
}

void ARCH_UartSendByteSync(USART_TypeDef *uartNum, uint8_t data)
{
    HAL_UART_Transmit(&huart1, &data, 1, 10);
}

void ARCH_Uart1SendByteSync(uint8_t data)
{
    ARCH_UartSendByteSync(USART1, data);
}
void ARCH_Uart2SendByteSync(uint8_t data)
{
    ARCH_UartSendByteSync(USART2, data);
}
void ARCH_Uart3SendByteSync(uint8_t data)
{
    ARCH_UartSendByteSync(USART3, data);
}

void ARCH_UartSendByte(USART_TypeDef *uartNum, uint8_t data)
{
    if (uartNum == USART1)
    {
        //-- wait end of last character finish to send
        while (HAL_UART_Transmit_IT(&huart1, &data, 1) != HAL_OK)
            ;
    }
    else if (uartNum == USART2)
    {
        //-- wait end of last character finish to send
        while (HAL_UART_Transmit_IT(&huart2, &data, 1) != HAL_OK)
            ;
    }
    else if (uartNum == USART3)
    {
        //-- wait end of last character finish to send
        while (HAL_UART_Transmit_IT(&huart3, &data, 1) != HAL_OK)
            ;
    }
    else
    {
        Error_Handler();
    }
}

void ARCH_Uart1SendByte(uint8_t data)
{
    ARCH_UartSendByte(USART1, data);
}

void ARCH_Uart2SendByte(uint8_t data)
{
    ARCH_UartSendByte(USART2, data);
}

void ARCH_Uart3SendByte(uint8_t data)
{
    ARCH_UartSendByte(USART3, data);
}

void ARCH_Uart1UseRs485(bool value)
{
    useRs4851 = value;
}

void ARCH_Uart2UseRs485(bool value)
{
    useRs4852 = value;
}

void ARCH_Uart3UseRs485(bool value)
{
    useRs4853 = value;
}

//-- rx received byte callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        s_MSG_UART uartData;
        uartData.length = 1;
        uartData.data[0] = dataHuart1;

        if (useRs4851 == false)
        {
            YAUS_msgSend(YAUS_QUEUE_UART1_RX_HANDLE, &uartData);
        }
        else
        {
            YAUS_msgSend(YAUS_QUEUE_RS4851_RX_HANDLE, &uartData);
        }
        HAL_UART_Receive_IT(huart, &dataHuart1, 1);
    }
    if (huart->Instance == USART2)
    {
        s_MSG_UART uartData;
        uartData.length = 1;
        uartData.data[0] = dataHuart2;

        if (useRs4852 == false)
        {
            YAUS_msgSend(YAUS_QUEUE_UART2_RX_HANDLE, &uartData);
        }
        else
        {
            YAUS_msgSend(YAUS_QUEUE_RS4852_RX_HANDLE, &uartData);
        }
        HAL_UART_Receive_IT(huart, &dataHuart2, 1);
    }
    if (huart->Instance == USART3)
    {
        s_MSG_UART uartData;
        uartData.length = 1;
        uartData.data[0] = dataHuart3;

        if (useRs4853 == false)
        {
            YAUS_msgSend(YAUS_QUEUE_UART3_RX_HANDLE, &uartData);
        }
        else
        {
            YAUS_msgSend(YAUS_QUEUE_RS4853_RX_HANDLE, &uartData);
        }
        HAL_UART_Receive_IT(huart, &dataHuart3, 1);
    }
}