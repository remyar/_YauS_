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
#include <_YauS_.h>
#include "./_YauS_Tick.h"

/** CONSTANTS ******************************************************/

/** MACROS *********************************************************/
#define DIFF_COUNT(R, C, L) \
	if ((C) >= (L))         \
		(R) = (C) - (L);    \
	else                    \
		(R) = (C) + 0xFFFFFFFF + 1 - (L)

/** VARIABLES ******************************************************/

/** DECLARATIONS ***************************************************/

void YAUS_TickInit(void)
{
	ARCH_TickInit();
}

uint32_t YAUS_TickCount(void)
{
	return ARCH_GetTick();
}

uint32_t YAUS_TickNbCountSince(uint32_t lastCount)
{
	uint64_t res;
	uint32_t newCount;

	newCount = YAUS_TickCount();

	DIFF_COUNT(res, newCount, lastCount);

	return (uint32_t)(res);
}

//=============================================================================
//--- INTERRUPTIONS
//=============================================================================
void YAUS_HandleIT(void)
{
	YAUS_Update(); //-- mise a jours des taches

	if (funcIdleHook != NULL)
		funcIdleHook();
}
