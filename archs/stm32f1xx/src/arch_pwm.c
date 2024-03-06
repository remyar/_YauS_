
#include "arch_pwm.h"

TIM_HandleTypeDef htim3;

void ARCH_PwmInit(uint32_t periphNum, unsigned long freq, uint32_t flags)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    uint32_t clk;
    uint32_t period;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    switch (periphNum)
    {
    case (PWMB0):
        //-- use tim3 ch3
        if (__HAL_RCC_TIM3_IS_CLK_DISABLED())
            __HAL_RCC_TIM3_CLK_ENABLE();
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();

        /* USER CODE BEGIN TIM3_Init 1 */
        clk = HAL_RCC_GetPCLK1Freq() * 2;
        period = clk / freq;

        htim3.Init.Prescaler = 0;

        while (period > 65000)
        {
            htim3.Init.Prescaler++;
            period /= 2;
        }

        /* USER CODE END TIM3_Init 1 */
        htim3.Instance = TIM3;

        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim3.Init.Period = period;
        htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
        if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
        {
            HardFault_Handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
        {
            HardFault_Handler();
        }
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
        {
            HardFault_Handler();
        }

        /* USER CODE BEGIN TIM3_Init 2 */

        /* USER CODE END TIM1_Init 2 */
        /**TIM1 GPIO Configuration
        PB0     ------> TIM3_CH3
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // HAL_TIM_Base_Start(&htim3);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        break;

    case (PWMB1):
        //-- use tim3 ch4
        if (__HAL_RCC_TIM3_IS_CLK_DISABLED())
            __HAL_RCC_TIM3_CLK_ENABLE();
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();

        /* USER CODE BEGIN TIM3_Init 1 */
        clk = HAL_RCC_GetPCLK1Freq() * 2;
        period = clk / freq;

        htim3.Init.Prescaler = 0;

        while (period > 65000)
        {
            htim3.Init.Prescaler++;
            period /= 2;
        }

        /* USER CODE END TIM3_Init 1 */
        htim3.Instance = TIM3;

        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim3.Init.Period = period;
        htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
        if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
        {
            HardFault_Handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
        {
            HardFault_Handler();
        }
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
        {
            HardFault_Handler();
        }

        /* USER CODE BEGIN TIM3_Init 2 */

        /* USER CODE END TIM1_Init 2 */
        /**TIM1 GPIO Configuration
        PB1     ------> TIM3_CH4
        */
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // HAL_TIM_Base_Start(&htim3);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
        break;
    }
}

void ARCH_PwmSetDutyCycle(uint8_t periphNum, float percent)
{
    switch (periphNum)
    {
    case PWMB0:
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, ((float)htim3.Init.Period * ((float)percent / 100.0)));
        break;
    case PWMB1:
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, ((float)htim3.Init.Period * ((float)percent / 100.0)));
        break;
    }
}

void ARCH_Pwm1Init(unsigned long freq, uint32_t flags)
{
    ARCH_PwmInit(PWMB0, freq, flags);
}

void ARCH_Pwm1SetDutyCycle(float percent)
{
    ARCH_PwmSetDutyCycle(PWMB0, percent);
}

void ARCH_Pwm2Init(unsigned long freq, uint32_t flags)
{
    ARCH_PwmInit(PWMB0, freq, flags);
}

void ARCH_Pwm2SetDutyCycle(float percent)
{
    ARCH_PwmSetDutyCycle(PWMB1, percent);
}