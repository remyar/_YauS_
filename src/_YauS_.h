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

#ifndef _YAUS__H_INCLUDED
#define _YAUS__H_INCLUDED

/** INCLUDES *******************************************************/
#include "arch.h"
#include "_YauS_type.h"
#include "_YauS_Config.h"
#include "_YauS_Tick.h"
#include "_YauS_Queue.h"
#ifdef YAUS_USE_MODULE_CONSOLE
#include "CONS_Task.h"
#endif
#ifdef YAUS_USE_MODULE_DRIVERS
#include "drivers.h"
#endif

/** CONSTANTS ******************************************************/
#define YAUS_TimerCreate(X, Y, Z) LLD_TimerCreate(X, Y, Z)
#define YAUS_TimerClose(X) LLD_TimerCreate(X)
#define YAUS_TimerSetPeriod(X, Y) LLD_WriteVal(X, Y)

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/
extern s_TASK tasks[YAUS_MAX_TASKS];

/** DECLARATIONS ***************************************************/
UINT32 YAUS_TaskCreate(STRING name, void *initFunc, void *runFunc, INT16 period, INT16 priority);
void YAUS_HookTick(void *hookFunc);

void YAUS_Init(void *callbackInit);
void YAUS_Update(void);
void YAUS_Run(BOOL blocking);
void YAUS_TaskForce(UINT32 handle);

#endif /* _YAUS__H_INCLUDED */
