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
#include "_YauS_Tick.h"
#include "arch.h"

/** CONSTANTS ******************************************************/

/** MACROS *********************************************************/
#define	DIFF_COUNT(R, C, L)			if ((C) >= (L))				\
										(R) = (C) - (L);		\
									else						\
										(R) = (C) + 0xFFFFFFFF + 1 - (L)

/** VARIABLES ******************************************************/
static UINT32	_tick;

/** DECLARATIONS ***************************************************/

void	TICK_Init(void)
{
    HAL_TickInit();
 //   _tick = 0;
}

UINT32	TICK_Count(void)
{
	return HAL_GetTick();
}

UINT32	TICK_nbCountSince(UINT32 lastCount)
{
	UINT64	res;
	UINT32	newCount;

	newCount = HAL_GetTick();

	DIFF_COUNT(res, newCount, lastCount);

	return (UINT32)(res);
}


//=============================================================================
//--- INTERRUPTIONS
//=============================================================================
void  TICK_HandleIT(void)
{

	YAUS_Update();  //-- mise a jours des taches

	if ( funcIdleHook != NULL )
	    funcIdleHook();
}
