//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// MODULE       : Tick.c
// DESCRIPTION  : Gestion du Tick periodique
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "_YauS_.h"
#include "CONS_Task.h"
#include <stdarg.h>
#include <math.h>
//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define CONFIG_SYS_CBSIZE 128
//-----------------------------------------------------------------------------
// Variables globales
//-----------------------------------------------------------------------------
extern const char logo[8][64];

static const UINT8 erase_seq[] = "\b \b";  /* erase sequence	*/
static const UINT8 tab_seq[] = "        "; /* used to expand TABs	*/

UINT8 console_buffer[CONFIG_SYS_CBSIZE + 1]; /* console I/O buffer	*/
UINT8 envname[CONFIG_SYS_CBSIZE];
UINT8 idxConsoleBuffer;

//=============================================================================
//--- DEFINITIONS
//=============================================================================
void CONS_printFloat(FLOAT32 value, UINT8 nbDec)
{
   INT32 entier = (INT32)value;
   FLOAT32 decimal = value - entier;

   CONS_print("%d.%u", entier, (UINT32)(decimal * powf(10.0, (FLOAT32)nbDec)));
}

void CONS_print(const UINT8 *pFormat, ...)
{
   UINT8 tab[64];
   UINT8 length, i;
   va_list ap;

   /* Forward call to vprintf */
   va_start(ap, pFormat);
   length = vsprintf(tab, pFormat, ap);
   va_end(ap);

   for (i = 0; i < length; i++)
      CONS_putc(tab[i]);

   CONS_flush();
}

void _initConsoleBuffer(void)
{
   idxConsoleBuffer = 0;
   console_buffer[idxConsoleBuffer] = 0;
   console_buffer[idxConsoleBuffer + 1] = 0;
}
void _putCharInConsoleBuffer(UINT8 c)
{
   console_buffer[idxConsoleBuffer] = c;
   if (c != 0)
   {
      idxConsoleBuffer++;
      console_buffer[idxConsoleBuffer] = 0;
   }
}

UINT8 _getLastCharInConsoleBuffer(void)
{
   return console_buffer[idxConsoleBuffer - 1];
}

//-----------------------------------------------------------------------------
// FONCTION    : _parse_line
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static INT16 _parse_line(UINT8 *line, UINT8 *argv[])
{
   INT16 nargs = 0;
   while (nargs < 16)
   {
      /* skip any white space */
      while (isblank(*line))
         ++line;

      if (*line == '\0')
      { /* end of line, no more args	*/
         argv[nargs] = NULL;
         return (nargs);
      }

      argv[nargs++] = line; /* begin of argument string	*/

      /* find end of string */
      while (*line && !isblank(*line))
         ++line;

      if (*line == '\0')
      { /* end of line, no more args	*/
         argv[nargs] = NULL;
         return (nargs);
      }

      *line++ = '\0'; /* terminate current arg	 */
   }
   CONS_print("** Too many args (max. %d) **\r\n", (UINT32)16);
   return (nargs);
}
//-----------------------------------------------------------------------------
// FONCTION    : _process_macros
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static void _process_macros(const UINT8 *input, UINT8 *output)
{
   UINT8 c, prev;
   const UINT8 *varname_start = NULL;
   INT16 inputcnt = strlen(input);
   INT16 outputcnt = CONFIG_SYS_CBSIZE;
   INT16 state = 0; /* 0 = waiting for '$'  */
   INT16 i;
   UINT8 *envval;
   INT16 envcnt; /* Varname # of chars */

   /* 1 = waiting for '(' or '{' */
   /* 2 = waiting for ')' or '}' */
   /* 3 = waiting for '''  */
   prev = '\0'; /* previous character   */

   while (inputcnt && outputcnt)
   {
      c = *input++;
      inputcnt--;

      if (state != 3)
      {
         /* remove one level of escape characters */
         if ((c == '\\') && (prev != '\\'))
         {
            if (inputcnt-- == 0)
               break;
            prev = c;
            c = *input++;
         }
      }

      switch (state)
      {
      case 0: /* Waiting for (unescaped) $    */
         if ((c == '\'') && (prev != '\\'))
         {
            state = 3;
            break;
         }
         if ((c == '$') && (prev != '\\'))
         {
            state++;
         }
         else
         {
            *(output++) = c;
            outputcnt--;
         }
         break;
      case 1: /* Waiting for (        */
         if (c == '(' || c == '{')
         {
            state++;
            varname_start = input;
         }
         else
         {
            state = 0;
            *(output++) = '$';
            outputcnt--;

            if (outputcnt)
            {
               *(output++) = c;
               outputcnt--;
            }
         }
         break;
      case 2: /* Waiting for )        */
         if (c == ')' || c == '}')
         {
            envcnt = input - varname_start - 1; /* Varname # of chars */

            /* Get the varname */
            for (i = 0; i < envcnt; i++)
            {
               envname[i] = varname_start[i];
            }
            envname[i] = 0;

            /* Get its value */
            //       envval = ENV_getenv((UINT8 *)envname);

            /* Copy into the line if it exists */
            if (envval != NULL)
            {
               while ((*envval) && outputcnt)
               {
                  *(output++) = *(envval++);
                  outputcnt--;
               }
            }
            /* Look for another '$' */
            state = 0;
         }
         break;
      case 3: /* Waiting for '        */
         if ((c == '\'') && (prev != '\\'))
         {
            state = 0;
         }
         else
         {
            *(output++) = c;
            outputcnt--;
         }
         break;
      }
      prev = c;
   }

   if (outputcnt)
      *output = 0;
   else
      *(output - 1) = 0;
}

//-----------------------------------------------------------------------------
// FONCTION    : _builtin_run_command
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static INT16 _builtin_run_command(const UINT8 *cmd)
{
   UINT8 cmdbuf[CONFIG_SYS_CBSIZE]; /* working copy of cmd		*/
   UINT8 *token;                    /* start of token in cmdbuf	*/
   UINT8 *sep;                      /* end of token (separator) in cmdbuf */
   UINT8 finaltoken[CONFIG_SYS_CBSIZE];
   UINT8 *str = cmdbuf;
   UINT8 *argv[16 + 1]; /* NULL terminated	*/
   INT16 argc, inquotes;
   INT16 repeatable = 1;
   INT16 rc = 0;

   if (!cmd || !*cmd)
   {
      return -1; /* empty command */
   }

   if (strlen(cmd) >= CONFIG_SYS_CBSIZE)
   {
      CONS_print("## Command too long!\r\n");
      return -1;
   }

   strcpy(cmdbuf, cmd);

   /* Process separators and check for invalid
    * repeatable commands
    */
   while (*str)
   {
      /*
       * Find separator, or string end
       * Allow simple escape of ';' by writing "\;"
       */
      for (inquotes = 0, sep = str; *sep; sep++)
      {
         if ((*sep == '\'') && (*(sep - 1) != '\\'))
            inquotes = !inquotes;

         if (!inquotes &&
             (*sep == ';') &&      /* separator		*/
             (sep != str) &&       /* past string start	*/
             (*(sep - 1) != '\\')) /* and NOT escaped	*/
            break;
      }

      /*
       * Limit the token to data between separators
       */
      token = str;
      if (*sep)
      {
         str = sep + 1; /* start of command for next pass */
         *sep = '\0';
      }
      else
         str = sep; /* no more commands for next pass */

      /* find macros in this token and replace them */
      _process_macros(token, finaltoken);

      /* Extract arguments */
      if ((argc = _parse_line(finaltoken, argv)) == 0)
      {
         rc = -1; /* no command at all */
         continue;
      }
      if (CMD_process(argc, argv, &repeatable))
         rc = -1;
   }
   return rc ? rc : repeatable;
}

//-----------------------------------------------------------------------------
// FONCTION    : _run_command
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static INT16 _run_command()
{
   INT16 retVal = 0;
   if (_builtin_run_command(console_buffer) == -1)
   {
      retVal = 1;
   }

   return retVal;
}

//-----------------------------------------------------------------------------
// FONCTION    : KBOOT_readline
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static INT16 _readline(void)
{
   UINT8 c;

   if (CONS_kbhit())
   {
      c = CONS_getc();
   }
   else
   {
      return -2;
   }

   switch (c)
   {
   case '\r': /* Enter		*/
   case '\n':
      _putCharInConsoleBuffer(0);
      CONS_print("\r\n");
      return idxConsoleBuffer;
   case '\0': /* nul			*/
      return -2;
   case 0x03:               /* ^C - break		*/
      _initConsoleBuffer(); /* discard input */
      return (-1);
   case 0x15: /* ^U - erase line	*/
      if (idxConsoleBuffer > 0)
      {
         while (idxConsoleBuffer > 0)
         {
            idxConsoleBuffer--;
            _putCharInConsoleBuffer(0);
            CONS_print(erase_seq);
         }
      }
      return -2;
   case 0x17: /* ^W - erase word	*/
      if (idxConsoleBuffer > 0)
      {
         idxConsoleBuffer--;
         _putCharInConsoleBuffer(0);
         CONS_print(erase_seq);
         while ((idxConsoleBuffer > 0) && (_getLastCharInConsoleBuffer() != ' '))
         {
            idxConsoleBuffer--;
            _putCharInConsoleBuffer(0);
            CONS_print(erase_seq);
         }
      }
      return -2;
   case 0x08: /* ^H  - backspace	*/
   case 0x7F: /* DEL - backspace	*/
      if (idxConsoleBuffer > 0)
      {
         idxConsoleBuffer--;
         _putCharInConsoleBuffer(0);
         CONS_print(erase_seq);
      }
      return -2;
   default:
      /*
       * Must be a normal character then
       */
      if (idxConsoleBuffer < CONFIG_SYS_CBSIZE - 2)
      {
         if (c == '\t')
         { /* expand TABs		*/
            CONS_print("   ");
            _putCharInConsoleBuffer(' ');
            _putCharInConsoleBuffer(' ');
            _putCharInConsoleBuffer(' ');
         }
         else
         {
            CONS_print("%c", c);
         }
         _putCharInConsoleBuffer(c);
         return -2;
      }
      else
      { /* Buffer full		*/
         CONS_print("\a");
      }
      break;
   }
   return -2;
}

//-----------------------------------------------------------------------------
// FONCTION    : _Init
//
// DESCRIPTION : Init de la tache
//-----------------------------------------------------------------------------
static BOOL _Init(void)
{
   _initConsoleBuffer();
   return TRUE;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _Run
//
// DESCRIPTION :  Appel periodique
//-----------------------------------------------------------------------------
static void _Run(void)
{
   static UINT8 ConsoleState = 0;
   UINT16 i;
   UINT8 c = 0;
   INT16 len;

   UINT32 sdFreeSpace, sdTotalSpace;

   switch (ConsoleState)
   {
   case (0):
   {
      if (CONS_kbhit())
      {
         c = CONS_getc();
         if (c == 0x1B) //-- si ESC
            ConsoleState = 1;
      }
      break;
   }
   case (1):
   {
      /* Send the welcome message. */
      CONS_print("\r\n");
      CONS_print("\r\n");
      for (i = 0; i < 7; i++)
      {
         CONS_print("%s\r\n", logo[i]);
      }
      CONS_print("_YauS_ %s \r\n", "0.0.1");
      ConsoleState++;
      break;
   }
   case (2):
   {
      /*   CONS_print("\r\n");
         //-- affichage info SD
         if ( SD_GetFreeSpace(&sdFreeSpace,&sdTotalSpace) )
         {
            CONS_print("SD in slot 0 found ! \r\n");
            CONS_print("used : %d / %d \r\n",sdFreeSpace,sdTotalSpace);
         }
         else
         {
            CONS_print("**** Warning **** \r\n");
            CONS_print("No rootFs found ! \r\n");
         }
*/
      ConsoleState++;
      break;
   }
      /* utilisation de la console a ce moment */
   case (3):
   {
      CONS_print("\r\n");
      CONS_print("#");
      _initConsoleBuffer();
      ConsoleState++;
   }
   default:
   {
      len = _readline();
      switch (len)
      {
      case (-1):
      {
         CONS_print("<INTERRUPT>\r\n");
         CONS_print("\r\n");
         ConsoleState = 3;
         break;
      }
      case (-2):
      {
         /* en cours de reception */
         break;
      }
      default:
      {
         _run_command();
         ConsoleState--;
         break;
      }
      }
      break;
   }
   }
}

void CONS_Init(void)
{
   YAUS_TaskCreate("MODULE-Console", (void *)_Init, (void *)_Run, 10, 0);
}
