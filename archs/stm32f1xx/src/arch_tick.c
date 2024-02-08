#include "arch_tick.h"

void ARCH_TickInit(void)
{

}

uint32_t ARCH_GetTick(void){
    return HAL_GetTick();
}