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

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef YAUS_USE_EVENTS
#include "_YauS_Events.h"
#endif

/** CONSTANTS ******************************************************/

#define MAX_CHARS_STRING 16

#ifndef bool
#define bool uint8_t
#endif
#ifndef true
#define true 1
#define false 0
#endif

#define I2C_WRITE 0
#define I2C_READ 1

/** MACROS *********************************************************/

typedef union //--- u_UINT32
{
    uint8_t tab[sizeof(uint32_t)];
    uint32_t val;
    int32_t sVal;
} u_uint32_t;

typedef union //--- u_UINT32
{
    uint8_t tab[sizeof(uint16_t)];
    uint16_t val;
    int16_t sVal;
} u_uint16_t;

typedef union //--- u_TAB_FLOAT32
{
    uint8_t tab[sizeof(float)];
    uint32_t valU32;
    int32_t valS32;
    float val;
} u_TAB_FLOAT32;

typedef union //--- u_TAB_FLOAT32
{
    uint8_t tab[sizeof(float)];
    uint32_t valU32;
    int32_t valS32;
    float val;
} u_float;

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

#define KEYBOARD_EVENT_TYPE (1 << 0)

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

typedef struct
{
    uint8_t length;
    uint8_t data[YAUS_MAX_LENGTH_MSG - 1];
} s_MSG_UART;

typedef struct
{
    uint8_t length;
    uint8_t data[YAUS_MAX_LENGTH_MSG - 1];
} s_MSG_USB;

typedef struct
{
    float dutyCycle;
    float frequence;
} s_MSG_PWM;

typedef struct
{
    uint8_t addr;
    uint8_t length;
    uint8_t stop;
    uint8_t data[YAUS_MAX_LENGTH_MSG - 3];
} s_MSG_I2C;

typedef struct
{
    uint8_t length;
    uint8_t data[YAUS_MAX_LENGTH_MSG - 1];
} s_MSG_SPI;

typedef struct
{
    uint32_t taskId;
    uint32_t eventType;
    uint8_t eventData[YAUS_MAX_LENGTH_MSG - 8];
} s_MSG_EVENT;

typedef struct
{
    bool longPress;
    uint16_t bpPressed;
} s_EVENT_BP;
/** VARIABLES ******************************************************/

/** DECLARATIONS ***************************************************/

#endif /* _YAUS__H_INCLUDED */
