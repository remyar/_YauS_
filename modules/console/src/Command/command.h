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

//-----------------------------------------------------------------------------
// Constantes : defines et enums
//-----------------------------------------------------------------------------

//#define CMD_FLAG_REPEAT		0x0001	/* repeat last command		*/
//#define CMD_FLAG_BOOTD		0x0002	/* command is from bootd	*/

#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)

typedef struct
{
    uint8_t	    *name;		/* Command Name			*/
	uint8_t maxargs;		/* maximum number of arguments	*/
	uint8_t repeatable;		/* autorepeat allowed?		*/
	int16_t (*cmd)(int16_t, uint8_t *const[]);	 /* Implementation function	*/
	uint8_t *usage;								 /* Usage message	(short)	*/
}cmd_tbl_t;

//-----------------------------------------------------------------------------
// Variables et Fonctions partagees
//-----------------------------------------------------------------------------
cmd_tbl_t *CMD_find_cmd(const uint8_t *cmd);

//-----------------------------------------------------------------------------
// Ex�cution des commandes
//-----------------------------------------------------------------------------
int16_t CMD_process(int16_t argc, uint8_t *argv[], int16_t *repeatable);

//-----------------------------------------------------------------------------
// Listes des commandes
//-----------------------------------------------------------------------------
int16_t TASK_list(int16_t argc, uint8_t *const argv[]);
int16_t CPU_reset(int16_t argc, uint8_t *const argv[]);
int16_t CONS_listCmd(int16_t argc, uint8_t *const argv[]);

#endif	/* __COMMAND_H */
