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
#include "_YauS_Config.h"

/** CONSTANTS ******************************************************/

#define MAX_CHARS_STRING 16

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/

/** DECLARATIONS ***************************************************/
#ifndef NULL
#define NULL 0
#endif

#if TYPE_BOOL_NOT_DEFINED
typedef enum //--- BOOL
{
    FALSE = 0,
    TRUE
} BOOL; // Undefined size
#endif

typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef unsigned long long UINT64;
typedef float FLOAT32;

typedef signed char INT8;
typedef signed short INT16;
typedef signed long INT32;

typedef char CHAR8;
typedef CHAR8 *STRING;
typedef unsigned char STRING_TAB[MAX_CHARS_STRING];

typedef unsigned short int WORD; // 16-bit unsigned
typedef unsigned long DWORD;     // 32-bit unsigned
typedef signed char CHAR;        // 8-bit signed
typedef signed short int SHORT;  // 16-bit signed
typedef signed long LONG;

#ifndef HIGH
#define HIGH TRUE
#endif
#ifndef LOW
#define LOW FALSE
#endif

// #define  ENABLE   TRUE
// #define  DISABLE  FALSE

#ifndef bool

#define bool uint8_t
#endif
#ifndef true
#define true TRUE
#endif
#ifndef false
#define false FALSE
#endif

typedef union //--- u_UINT8
{
    struct
    {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
    } bits;
    UINT8 val;
} u_UINT8;

typedef BOOL (*PTR_TASK_INIT_FUNC)(void);
typedef void (*PTR_TASK_RUN_FUNC)(void);
typedef void (*PTR_LL_CALLBACK_FUNC)(unsigned char ,unsigned char * , unsigned char);

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
    STRING_TAB name;         //-- nom de la tache ( pour debug )
    UINT8 idx;               //-- index de la tache ( uniquement pour le slot )
    e_STATUS status;         //-- status de la tache
    INT16 period;            //-- periode d'ex�cution de la tache
    INT16 priority;          //-- priorit� actuelle de la tache ( utile si deux tache doivent etre �x�cuter en m�me temps )
    INT16 counter;           //-- Temps restant avant l'ex�cution de la tache
    PTR_TASK_INIT_FUNC init; //-- fonction d'init
    PTR_TASK_RUN_FUNC run;   //-- fonction run
    INT16 defaultPeriod;     //-- periode d'�x�cution initiale ( lors de la cr�ation de la tache )
    INT16 defaultPriority;   //-- priorit� de la tache initiale ( lors de la cr�ation de la tache )

    UINT32 taskUseTick;
    UINT32 taskUseTickMin;
    UINT32 taskUseTickMax;

} s_TASK;

typedef union
{
    WORD Val;
    BYTE v[2];
    struct
    {
        BYTE LB;
        BYTE HB;
    } byte;
    struct
    {
        BYTE b0 : 1;
        BYTE b1 : 1;
        BYTE b2 : 1;
        BYTE b3 : 1;
        BYTE b4 : 1;
        BYTE b5 : 1;
        BYTE b6 : 1;
        BYTE b7 : 1;
        BYTE b8 : 1;
        BYTE b9 : 1;
        BYTE b10 : 1;
        BYTE b11 : 1;
        BYTE b12 : 1;
        BYTE b13 : 1;
        BYTE b14 : 1;
        BYTE b15 : 1;
    } bits;
} WORD_VAL, WORD_BITS;

typedef union
{
    DWORD Val;
    WORD w[2];
    BYTE v[4];
    struct
    {
        WORD LW;
        WORD HW;
    } word;
    struct
    {
        BYTE LB;
        BYTE HB;
        BYTE UB;
        BYTE MB;
    } byte;
    struct
    {
        WORD_VAL low;
        WORD_VAL high;
    } wordUnion;
    struct
    {
        BYTE b0 : 1;
        BYTE b1 : 1;
        BYTE b2 : 1;
        BYTE b3 : 1;
        BYTE b4 : 1;
        BYTE b5 : 1;
        BYTE b6 : 1;
        BYTE b7 : 1;
        BYTE b8 : 1;
        BYTE b9 : 1;
        BYTE b10 : 1;
        BYTE b11 : 1;
        BYTE b12 : 1;
        BYTE b13 : 1;
        BYTE b14 : 1;
        BYTE b15 : 1;
        BYTE b16 : 1;
        BYTE b17 : 1;
        BYTE b18 : 1;
        BYTE b19 : 1;
        BYTE b20 : 1;
        BYTE b21 : 1;
        BYTE b22 : 1;
        BYTE b23 : 1;
        BYTE b24 : 1;
        BYTE b25 : 1;
        BYTE b26 : 1;
        BYTE b27 : 1;
        BYTE b28 : 1;
        BYTE b29 : 1;
        BYTE b30 : 1;
        BYTE b31 : 1;
    } bits;
} DWORD_VAL;

typedef struct
{
    UINT8 addr;
    UINT8 length;
    UINT8 data[YAUS_MAX_LENGTH_MSG - 3];
    UINT8 stop;
} s_MSG_I2C;

typedef struct {
    UINT8 length;
    UINT8 data[YAUS_MAX_LENGTH_MSG - 1];
} s_MSG_UART;

typedef struct {
    float dutyCycle;
    float frequence;
}s_MSG_PWM;

#endif /* _YAUS__H_INCLUDED */
