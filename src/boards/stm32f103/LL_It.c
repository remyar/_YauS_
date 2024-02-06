#include "LL_It.h"


void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    TICK_HandleIT();
    /* USER CODE END SysTick_IRQn 1 */
}


