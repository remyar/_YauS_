/********************************************************************
 FileName:      _YauS_.h
 Dependencies:  See INCLUDES section
 Processor:
 Hardware:
 Compiler:
 Company:

********************************************************************

 File Description:

 Change History:
  Rev   Date         Description
  1.0   07/11/2014   Initial release

********************************************************************/

#ifndef _LLD_TICK_H_INCLUDED
#define _LLD_TICK_H_INCLUDED

/** INCLUDES *******************************************************/
#include "./_YauS_type.h"
#include "../_YauS_Config.h"
#include "./_YauS_.h"

/** CONSTANTS ******************************************************/

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/
extern PTR_TASK_RUN_FUNC funcIdleHook;

/** DECLARATIONS ***************************************************/
__attribute__((weak)) void LLD_TickInit(void);
__attribute__((weak)) uint32_t LLD_GetTick(void) ;

void YAUS_TickInit(void);
uint32_t YAUS_TickCount(void);
uint32_t YAUS_TickNbCountSince(uint32_t lastCount);
void YAUS_HandleIT(void);

#endif /* _LLD_TICK_H_INCLUDED */
