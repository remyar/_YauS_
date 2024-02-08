//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// MODULE       : main.c
// DESCRIPTION  : initialisation et lancement de l'application
//
//-----------------------------------------------------------------------------
//
// HISTORIQUE   :
//
// 	11.04.2012 - V0.01 : - Creation
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "_YauS_.h"
#include "command.h"
#include <math.h>
// #include "config.h"

//-----------------------------------------------------------------------------
// Fonctions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constantes
//-----------------------------------------------------------------------------
const cmd_tbl_t __u_boot_cmd[] =
    {
        {"task-stats", 1, 0, TASK_list, "Displays a table showing the state of each task"},
        //{"sensors", 2, 1, SENSORS_cmd, "Display all sensors"},
        //{"measures", 2, 1, MEASURES_cmd, "Display all measures"},
        //{"wifi", 2, 0, WIFI_cmd, "Display This"},
        //	{"reset"		, 1		, 0		, CPU_reset			, "Perform RESET of the CPU"},
        {"help", 1, 0, CONS_listCmd, "Display This"},
};

//-----------------------------------------------------------------------------
// Variables globales
//-----------------------------------------------------------------------------
//=============================================================================
//--- DEFINITIONS
//=============================================================================
//-----------------------------------------------------------------------------
// FONCTION    : _cmd_usage
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static int16_t _cmd_usage(const cmd_tbl_t *cmdtp)
{
    CONS_print("%s - %s\r\n", cmdtp->name, cmdtp->usage);
    return 1;
}

//-----------------------------------------------------------------------------
// FONCTION    : _cmd_call
//
// DESCRIPTION : appel de la commande
//-----------------------------------------------------------------------------
static int16_t _cmd_call(cmd_tbl_t *cmdtp, int16_t argc, uint8_t *const argv[])
{
    int16_t result;

    result = (cmdtp->cmd)(argc, argv);
    if (result)
        CONS_print("Command failed, result=%d\r\n", result);
    return result;
}

//-----------------------------------------------------------------------------
// FONCTION    : CMD_find_cmd
//
// DESCRIPTION : Cherche la commande correspondante
//-----------------------------------------------------------------------------
cmd_tbl_t *CMD_find_cmd(const uint8_t *cmd)
{
    cmd_tbl_t *cmdtp;
    cmd_tbl_t *cmdtp_temp = __u_boot_cmd; /*Init value */
    uint16_t i, len;
    const uint8_t *p;
    uint16_t tabSize = sizeof(__u_boot_cmd) / sizeof(__u_boot_cmd[0]);

    /*
     * Some commands allow length modifiers (like "cp.b");
     * compare command name only until first dot.
     */
    len = ((p = strchr(cmd, '.')) == NULL) ? strlen(cmd) : (p - cmd);

    for (i = 0; i < tabSize; i++)
    {
        cmdtp = &__u_boot_cmd[i];
        if (strncmp(cmd, cmdtp->name, len) == 0)
        {
            if (len == strlen(cmdtp->name))
                return cmdtp; /* full match */
        }
    }

    return NULL; /* not found or ambiguous command */
}

//-----------------------------------------------------------------------------
// FONCTION    : CMD_process
//
// DESCRIPTION : gestion de la commande
//-----------------------------------------------------------------------------
int16_t CMD_process(int16_t argc, uint8_t *argv[], int16_t *repeatable)
{
    cmd_tbl_t *cmdtp;

    /* Look up command in command table */
    cmdtp = CMD_find_cmd(argv[0]);
    if (cmdtp == NULL)
    {
        CONS_print("Unknown command '%s' - try 'help'\r\n", argv[0]);
        return 1;
    }

    _cmd_call(cmdtp, argc, argv);

    return -1;
}

int16_t CONS_listCmd(int16_t argc, uint8_t *const argv[])
{
    uint16_t i, len, j;
    uint16_t tabSize = sizeof(__u_boot_cmd) / sizeof(__u_boot_cmd[0]);

    CONS_print("Listing of command supported : \r\n");
    CONS_print("\r\n");
    CONS_print("Command         Description\r\n");
    CONS_print("----------------------------------------\r\n");
    for (i = 0; i < tabSize; i++)
    {
        /* nom de la commande */
        len = strlen(__u_boot_cmd[i].name);
        CONS_print(__u_boot_cmd[i].name);
        for (j = len; j < 16; j++)
        {
            CONS_print(" ");
        }

        CONS_print(__u_boot_cmd[i].usage);
        CONS_print("\r\n");
    }

    return 0;
}

int16_t TASK_list(int16_t argc, uint8_t *const argv[])
{
    uint32_t i, j;
    uint16_t len;

    CONS_print("Task");
    for (j = 4; j < 16; j++)
    {
        CONS_print(" ");
    }
    CONS_print("State  Pri  #  cpu  ");
    CONS_print("\r\n");

    CONS_print("----------------------------------------------");

    CONS_print("\r\n");

    for (i = 0; i < YAUS_MAX_TASKS; i++)
    {
        if (tasks[i].idx != NULL)
        {
            /* nom de la tache */
            len = strlen(tasks[i].name);
            CONS_print("%s", tasks[i].name);
            for (j = len; j < 16; j++)
            {
                CONS_print(" ");
            }

            /* �tat de la tache */
            switch (tasks[i].status)
            {
            case (NO_STATUS):
                CONS_print("NO     ");
                break;
            case (INIT_STATUS):
                CONS_print("INIT   ");
                break;
            case (WAITING_STATUS):
                CONS_print("WAIT   ");
                break;
            case (READY_STATUS):
                CONS_print("READY  ");
                break;
            case (RUNNING_STATUS):
                CONS_print("RUN    ");
                break;
            case (END_RUN_STATUS):
                CONS_print("END    ");
                break;
            }

            /* priorit� */
            CONS_print("%d    ", tasks[i].priority);

            /* num�ros de la tache */
            CONS_print("%d  ", tasks[i].idx);

            /* utilisation CPu */
            float percent = (float)((float)tasks[i].taskUseTick / (float)TICK_Count()) * 100.0;

            if (percent < 1.0)
                CONS_print("<1%% ");
            else
                CONS_print("%d%% ", (uint16_t)percent);

            CONS_print("\r\n");
        }
    }
    return 0;
}
