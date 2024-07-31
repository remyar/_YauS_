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
#include <_YauS_.h>

/** CONSTANTS ******************************************************/

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/
extern PTR_TASK_RUN_FUNC funcIdleHook;

/** DECLARATIONS ***************************************************/

void YAUS_TickInit(void);
uint32_t YAUS_TickCount(void);
uint32_t YAUS_TickNbCountSince(uint32_t lastCount);
void YAUS_HandleIT(void);
void YAUS_Delay(uint32_t ms);

#endif /* _LLD_TICK_H_INCLUDED */
