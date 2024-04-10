#include "arch_rs485.h"

typedef struct
{
    GPIO_TypeDef *GPIO_PortStruct;
    GPIO_InitTypeDef GPIO_PinStruct;
} RS485PINSTRUCT;

static RS485PINSTRUCT rs4851PinStruct;

void ARCH_RS4851Init(uint32_t speed, GPIO_TypeDef *portDe, uint32_t pinDe, uint32_t flags)
{
    if (portDe == GPIOB)
    {
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();
    }

    rs4851PinStruct.GPIO_PortStruct = portDe;

    rs4851PinStruct.GPIO_PinStruct.Mode = GPIO_MODE_OUTPUT_PP;
    rs4851PinStruct.GPIO_PinStruct.Pin = pinDe;
    rs4851PinStruct.GPIO_PinStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(rs4851PinStruct.GPIO_PortStruct, &rs4851PinStruct.GPIO_PinStruct);
    HAL_GPIO_WriteFastPin(rs4851PinStruct.GPIO_PortStruct, rs4851PinStruct.GPIO_PinStruct.Pin, GPIO_PIN_RESET);
    
    ARCH_Uart1Init(speed, flags);
    ARCH_Uart1UseRs485(true);
}

void ARCH_RS4851SendByteSync(uint8_t data)
{
    HAL_GPIO_WriteFastPin(rs4851PinStruct.GPIO_PortStruct, rs4851PinStruct.GPIO_PinStruct.Pin, GPIO_PIN_SET);
    ARCH_Uart1SendByteSync(data);
    HAL_GPIO_WriteFastPin(rs4851PinStruct.GPIO_PortStruct, rs4851PinStruct.GPIO_PinStruct.Pin, GPIO_PIN_RESET);
}

bool ARCH_RS4851Available(void){
    return ARCH_Uart1Available();
}

uint8_t ARCH_RS4851Read(void)
{
    return ARCH_Uart1Read();
}
