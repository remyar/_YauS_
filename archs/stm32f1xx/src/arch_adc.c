#include "arch_adc.h"

//static ADC_ChannelConfTypeDef sConfig = {0};
ADC_HandleTypeDef hadc1;
static uint8_t ADC_channels[MAX_ADC];
static uint8_t ADC_nbChannel = 0;
static uint8_t ADC_idxChannel = 0;
static uint32_t ADC_value[MAX_ADC];
static uint8_t ADC_state = ADC_NOT_USED;

bool _allreadyConfigured(uint8_t pin)
{
    bool allreadyExist = false;
    for (uint8_t i = 0; i < MAX_ADC; i++)
    {
        if (ADC_channels[i] == pin)
        {
            allreadyExist = true;
            break;
        }
    }
    return allreadyExist;
}

uint32_t _getChannelFromPeriphNum(uint32_t periphNum)
{
    uint32_t channel = (uint32_t)NULL;
    switch (periphNum)
    {
    case (ADC_PA0):
        channel = ADC_CHANNEL_0;
        break;
    case (ADC_PA1):
        channel = ADC_CHANNEL_1;
        break;
    }
    return channel;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        ADC_value[ADC_idxChannel] = HAL_ADC_GetValue(hadc);
        ADC_state = ADC_STATE_FINISH;
    }
}

void ARCH_AdcInit(uint8_t periphNum)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (__HAL_RCC_ADC1_IS_CLK_DISABLED())
        __HAL_RCC_ADC1_CLK_ENABLE();
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
        __HAL_RCC_GPIOA_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;

    switch (periphNum)
    {
    case (ADC_PA0):
    {
        if (_allreadyConfigured(GPIO_PIN_0) == false)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_0;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

            ADC_channels[ADC_nbChannel] = ADC_PA0;
            ADC_nbChannel++;
        }
        break;
    }
    case (ADC_PA1):
    {
        if (_allreadyConfigured(GPIO_PIN_1) == false)
        {
            GPIO_InitStruct.Pin = GPIO_PIN_1;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

            ADC_channels[ADC_nbChannel] = ADC_PA1;
            ADC_nbChannel++;
        }
        break;
    }
    }

    HAL_ADC_DeInit(&hadc1);

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        HardFault_Handler();
    }

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    ADC_state = ADC_STATE_INIT;
}

void ARCH_AdcProcess(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    switch (ADC_state)
    {
    case (ADC_NOT_USED):
        break;
    case (ADC_STATE_INIT):
        sConfig.Channel = _getChannelFromPeriphNum(ADC_channels[ADC_idxChannel]);
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) == HAL_OK)
        {
            ADC_state = ADC_STATE_START;
        }
        break;
    case (ADC_STATE_START):
        HAL_ADC_Start(&hadc1);
        ADC_state = ADC_STATE_WAIT_END;
        break;
    case (ADC_STATE_WAIT_END):
        if (HAL_IS_BIT_SET(hadc1.Instance->SR, ADC_FLAG_EOC))
        {
            ADC_value[ADC_idxChannel] = HAL_ADC_GetValue(&hadc1);
            ADC_state = ADC_STATE_FINISH;
        }
        break;
    case (ADC_STATE_FINISH):
        ADC_idxChannel++;
        if (ADC_idxChannel >= MAX_ADC)
        {
            ADC_idxChannel = 0;
        }
        ADC_state = ADC_STATE_INIT;
        break;
    }
}

uint32_t ARCH_AdcGetValue(uint8_t channel)
{
    return ADC_value[channel];
}