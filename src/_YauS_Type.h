/********************************************************************
 FileName:      _YauS_Type.h
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

#ifndef _YAUS_Type_H_INCLUDED
#define _YAUS_Type_H_INCLUDED

/** INCLUDES *******************************************************/
#include "../_YauS_Config.h"

#if defined(STM32F1)
#include <stm32f1xx.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/** CONSTANTS ******************************************************/

#define MAX_CHARS_STRING 16

#ifndef bool
#define bool uint8_t
#endif
#ifndef true
#define true 1
#define false 0
#endif
/** MACROS *********************************************************/

typedef bool (*PTR_TASK_INIT_FUNC)(void);
typedef void (*PTR_TASK_RUN_FUNC)(void);
typedef void (*PTR_LL_CALLBACK_FUNC)(unsigned char, unsigned char *, unsigned char);

typedef enum
{
    NO_STATUS = 0,
    INIT_STATUS,
    WAITING_STATUS,
    READY_STATUS,
    RUNNING_STATUS,
    END_RUN_STATUS
} e_STATUS;

typedef struct
{
    uint8_t name[MAX_CHARS_STRING]; //-- nom de la tache ( pour debug )
    uint8_t idx;                    //-- index de la tache ( uniquement pour le slot )
    e_STATUS status;                //-- status de la tache
    int16_t period;                 //-- periode d'ex�cution de la tache
    int16_t priority;               //-- priorit� actuelle de la tache ( utile si deux tache doivent etre �x�cuter en m�me temps )
    int16_t counter;                //-- Temps restant avant l'ex�cution de la tache
    PTR_TASK_INIT_FUNC init;        //-- fonction d'init
    PTR_TASK_RUN_FUNC run;          //-- fonction run
    int16_t defaultPeriod;          //-- periode d'�x�cution initiale ( lors de la cr�ation de la tache )
    int16_t defaultPriority;        //-- priorit� de la tache initiale ( lors de la cr�ation de la tache )

    uint32_t taskUseTick;
    uint32_t taskUseTickMin;
    uint32_t taskUseTickMax;

} s_TASK;

/** VARIABLES ******************************************************/

/** DECLARATIONS ***************************************************/

#endif /* _YAUS__H_INCLUDED */
