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
void TICK_Init(void);
uint32_t TICK_Count(void);
uint32_t TICK_nbCountSince(uint32_t lastCount);
void TICK_HandleIT(void);

#endif /* _LLD_TICK_H_INCLUDED */
