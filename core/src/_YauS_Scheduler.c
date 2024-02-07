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
#include "./_YauS_.h"
#include "./_YauS_Tick.h"
#include "./_YauS_Type.h"

/** CONSTANTS ******************************************************/

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/
PTR_TASK_RUN_FUNC funcIdleHook = NULL;
s_TASK tasks[YAUS_MAX_TASKS];
//static uint32_t _tick = 0;

/** DECLARATIONS ***************************************************/

void YAUS_TaskForce(uint32_t handle)
{
    uint8_t i;

    /* on cherche un slot de libre */
    for (i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == handle)
        {
            //--- Execution des taches
            if (tasks[i].run != NULL)
            {
                tasks[i].status = RUNNING_STATUS;
                tasks[i].run();
                tasks[i].status = END_RUN_STATUS;
            }
        }
    }
}

uint32_t YAUS_TaskCreate(char *name, void *initFunc, void *runFunc, int16_t period, int16_t priority)
{
    uint8_t i, j;

    /* on cherche un slot de libre */
    for (i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx == NULL)
        {

            j = 0xFF;
            do
            {
                j++;
                if (j >= MAX_CHARS_STRING)
                {
                    tasks[i].name[MAX_CHARS_STRING - 1] = 0;
                    break;
                }
                tasks[i].name[j] = name[j];
            } while (name[j] != 0);

            tasks[i].init = (PTR_TASK_INIT_FUNC)initFunc;
            tasks[i].run = (PTR_TASK_RUN_FUNC)runFunc;

            tasks[i].status = WAITING_STATUS;
            if (tasks[i].run != NULL)
                tasks[i].status = READY_STATUS;
            if (tasks[i].init != NULL)
                tasks[i].status = INIT_STATUS;

            tasks[i].defaultPeriod = period;
            tasks[i].defaultPriority = priority;

            tasks[i].period = tasks[i].defaultPeriod;
            tasks[i].priority = tasks[i].defaultPriority;

            tasks[i].counter = tasks[i].period;

            tasks[i].taskUseTick = 0;
            tasks[i].taskUseTickMin = 0xFFFFFFFF;
            tasks[i].taskUseTickMax = 0x00000000;

            tasks[i].idx = i + 1;
            break;
        }
    }
    return (uint32_t)tasks[i].idx;
}

void YAUS_HookTick(void *hookFunc)
{
    if (hookFunc != NULL)
        funcIdleHook = (PTR_TASK_RUN_FUNC)hookFunc;
}

void YAUS_Init(void)
{
    uint8_t i;

    for (i = 0; i < YAUS_MAX_TASKS; i++)
    {
        tasks[i].init = NULL;
        tasks[i].run = NULL;
    }

#ifdef YAUS_USE_ARCH
    ARCH_Init();
#endif

    YAUS_msgInit();

    YAUS_TickInit();

#ifdef YAUS_USE_MODULE_DRIVERS
    //-- Modules
    DRIVERS_Init();
#endif

#ifdef YAUS_USE_MODULE_CONSOLE
    CONS_Init();
#endif

}

void YAUS_Update(void)
{
    uint8_t i;

    for (i = 0; i < YAUS_MAX_TASKS; i++)
    {
        //--- Mise a jour des compteurs si la tache est active
        if (tasks[i].status == READY_STATUS)
        {
            if (tasks[i].counter > 0)
                tasks[i].counter--;
        }
    }

    if (funcIdleHook != NULL)
    {
        funcIdleHook();
    }
}

void YAUS_Run(bool blocking)
{
    uint32_t tick;
    static uint8_t i = 0;

    do
    {
        /*
        if (YAUS_TickNbCountSince(_tick) >= 1)
        {
            _tick = YAUS_TickCount();
            YAUS_Update();
        }*/

        //-- Mise a jours des taches ayant besoin d'etre initialis� */
        if ((tasks[i].init != NULL) && (tasks[i].status == INIT_STATUS))
        {
            if (tasks[i].init() == true)
                tasks[i].status = READY_STATUS;
            else
                tasks[i].status = WAITING_STATUS;
        }

        //--- Execution des taches
        if (tasks[i].run != NULL)
        {
            //--- Les taches ayant fini de s'executer redeviennent pretes
            if ((tasks[i].status == END_RUN_STATUS) && (tasks[i].counter == 0))
            {
                tasks[i].counter = tasks[i].period;
                tasks[i].status = READY_STATUS;
            }

            //--- Les taches pretes avec un timer a 0 deviennent actives
            if ((tasks[i].status == READY_STATUS) && (tasks[i].counter == 0))
            {
                tick = YAUS_TickCount();
                tasks[i].status = RUNNING_STATUS;
                tasks[i].run();
                tasks[i].status = END_RUN_STATUS;

                tasks[i].taskUseTick += YAUS_TickNbCountSince(tick);
                if (YAUS_TickNbCountSince(tick) < tasks[i].taskUseTickMin)
                    tasks[i].taskUseTickMin = YAUS_TickNbCountSince(tick);
                if (YAUS_TickNbCountSince(tick) > tasks[i].taskUseTickMax)
                    tasks[i].taskUseTickMax = YAUS_TickNbCountSince(tick);
            }
        }
        i++;
        if (i >= YAUS_MAX_TASKS)
            i = 0;
    } while (blocking);
}
