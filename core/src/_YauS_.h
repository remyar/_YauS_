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
#include "./_YauS_Type.h"
#include "../_YauS_Config.h"
#include "./_YauS_Tick.h"
//#include "./_YauS_Queue.h"
#ifdef YAUS_USE_MODULE_CONSOLE
#include "CONS_Task.h"
#endif
#ifdef YAUS_USE_MODULE_DRIVERS
#include "DRIVERS_Task.h"
#endif
#ifdef YAUS_USE_ARCH
#include <arch.h>
#endif
#ifdef YAUS_USE_EVENTS
#include "_YauS_Events.h"
#endif
/** CONSTANTS ******************************************************/
#define YAUS_TimerCreate(X, Y, Z) LLD_TimerCreate(X, Y, Z)
#define YAUS_TimerClose(X) LLD_TimerCreate(X)
#define YAUS_TimerSetPeriod(X, Y) LLD_WriteVal(X, Y)

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/
extern s_TASK tasks[YAUS_MAX_TASKS];

/** DECLARATIONS ***************************************************/
uint32_t YAUS_TaskCreate(char *name, void *initFunc, void *runFunc, int16_t period, int16_t priority);
void YAUS_HookTick(void *hookFunc);

void YAUS_Init(void);
void YAUS_Update(void);
void YAUS_Run(bool blocking);
void YAUS_TaskForce(uint32_t handle);

#endif /* _YAUS__H_INCLUDED */
