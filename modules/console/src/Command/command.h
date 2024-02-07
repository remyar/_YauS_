//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// HEADER       : command.h
//
//=============================================================================
#ifndef _COMMAND_H_
#define _COMMAND_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "../CONS_Task.h"
#include "sensors.h"
#include "sensors_def.h"
#include "measures.h"
#include "measures_def.h"
#include "Units.h"
//#include "config_cmd_all.h"

//-----------------------------------------------------------------------------
// Constantes : defines et enums
//-----------------------------------------------------------------------------

//#define CMD_FLAG_REPEAT		0x0001	/* repeat last command		*/
//#define CMD_FLAG_BOOTD		0x0002	/* command is from bootd	*/

#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)

typedef struct
{
    UINT8	    *name;		/* Command Name			*/
	UINT16		maxargs;	/* maximum number of arguments	*/
	UINT16		repeatable;	/* autorepeat allowed?		*/
	INT16		(*cmd)(INT16, UINT8 * const []); /* Implementation function	*/
	UINT8		*usage;		/* Usage message	(short)	*/
}cmd_tbl_t;

//-----------------------------------------------------------------------------
// Variables et Fonctions partagees
//-----------------------------------------------------------------------------
cmd_tbl_t *CMD_find_cmd (const UINT8 *cmd);


//-----------------------------------------------------------------------------
// Ex�cution des commandes
//-----------------------------------------------------------------------------
INT16 CMD_process(INT16 argc, UINT8 * argv[],INT16 *repeatable);

//-----------------------------------------------------------------------------
// Listes des commandes
//-----------------------------------------------------------------------------
INT16	TASK_list 	 (INT16 argc, UINT8 * const argv[]);
INT16	CPU_reset 	 (INT16 argc, UINT8 * const argv[]);
INT16 CONS_listCmd (INT16 argc, UINT8 * const argv[]);
INT16 WIFI_cmd (INT16 argc, UINT8 * const argv[]);
INT16	SENSORS_cmd(INT16 argc , UINT8* const argv[]);
INT16 MEASURES_cmd(INT16 argc, UINT8 *const argv[]);

#endif	/* __COMMAND_H */
