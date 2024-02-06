#include "LL_Pwm.h"

#ifdef LL_USE_PWM_PB0
TIM_HandleTypeDef htim3;
#endif

void LLPWM_Init(uint8_t periphNum, uint32_t freq)
{
    switch (periphNum)
    {
#ifdef LL_USE_PWM_PB0
    case LL_PWM_PB0:
        //-- use tim3 ch3
        if (__HAL_RCC_TIM3_IS_CLK_DISABLED())
            __HAL_RCC_TIM3_CLK_ENABLE();
        if (__HAL_RCC_GPIOB_IS_CLK_DISABLED())
            __HAL_RCC_GPIOB_CLK_ENABLE();

        TIM_MasterConfigTypeDef sMasterConfig = {0};
        TIM_OC_InitTypeDef sConfigOC = {0};

        /* USER CODE BEGIN TIM3_Init 1 */

        uint32_t clk = HAL_RCC_GetPCLK1Freq() * 2;
        uint32_t period = clk / freq;

        /* USER CODE END TIM3_Init 1 */
        htim3.Instance = TIM3;
        htim3.Init.Prescaler = 0;
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
        sConfigOC.Pulse = 32768;
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
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        //HAL_TIM_Base_Start(&htim3);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        break;
#endif
    }
}

void LLPWM_SetDutyCycle(uint8_t periphNum, float percent)
{
     switch (periphNum)
    {
#ifdef LL_USE_PWM_PB0
    case LL_PWM_PB0:
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, ((float)htim3.Init.Period * ((float)percent / 100.0)));
        break;
#endif
    }
}