/********************************************************************

 FileName:      Scheduler.c
 Dependencies:  See INCLUDES section
 Processor:
 Hardware:
 Complier:
 Company:

********************************************************************

 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release

********************************************************************/

/** INCLUDES *******************************************************/
#include "./_YauS_Tick.h"

/** CONSTANTS ******************************************************/

/** MACROS *********************************************************/
#define DIFF_COUNT(R, C, L) \
	if ((C) >= (L))         \
		(R) = (C) - (L);    \
	else                    \
		(R) = (C) + 0xFFFFFFFF + 1 - (L)

/** VARIABLES ******************************************************/
static uint32_t _tick;

/** DECLARATIONS ***************************************************/

void TICK_Init(void)
{

}

uint32_t TICK_Count(void)
{
	return HAL_GetTick();
}

uint32_t TICK_nbCountSince(uint32_t lastCount)
{
	uint64_t res;
	uint32_t newCount;

	newCount = HAL_GetTick();

	DIFF_COUNT(res, newCount, lastCount);

	return (uint32_t)(res);
}

//=============================================================================
//--- INTERRUPTIONS
//=============================================================================
void TICK_HandleIT(void)
{

	YAUS_Update(); //-- mise a jours des taches

	if (funcIdleHook != NULL)
		funcIdleHook();
}
