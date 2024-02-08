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
#include <_YauS_.h>
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

static const uint8_t erase_seq[] = "\b \b";  /* erase sequence	*/
static const uint8_t tab_seq[] = "        "; /* used to expand TABs	*/

uint8_t console_buffer[CONFIG_SYS_CBSIZE + 1]; /* console I/O buffer	*/
uint8_t envname[CONFIG_SYS_CBSIZE];
uint8_t idxConsoleBuffer;

//=============================================================================
//--- DEFINITIONS
//=============================================================================
void CONS_printFloat(float value, uint8_t nbDec)
{
   int32_t entier = (int32_t)value;
   float decimal = value - entier;

   CONS_print("%d.%u", entier, (uint32_t)(decimal * powf(10.0, (float)nbDec)));
}

void CONS_print(const uint8_t *pFormat, ...)
{
   uint8_t tab[64];
   uint8_t length, i;
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
void _putCharInConsoleBuffer(uint8_t c)
{
   console_buffer[idxConsoleBuffer] = c;
   if (c != 0)
   {
      idxConsoleBuffer++;
      console_buffer[idxConsoleBuffer] = 0;
   }
}

uint8_t _getLastCharInConsoleBuffer(void)
{
   return console_buffer[idxConsoleBuffer - 1];
}

//-----------------------------------------------------------------------------
// FONCTION    : _parse_line
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static int16_t _parse_line(uint8_t *line, uint8_t *argv[])
{
   int16_t nargs = 0;
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
   CONS_print("** Too many args (max. %d) **\r\n", (uint32_t)16);
   return (nargs);
}
//-----------------------------------------------------------------------------
// FONCTION    : _process_macros
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static void _process_macros(const uint8_t *input, uint8_t *output)
{
   uint8_t c, prev;
   const uint8_t *varname_start = NULL;
   int16_t inputcnt = strlen(input);
   int16_t outputcnt = CONFIG_SYS_CBSIZE;
   int16_t state = 0; /* 0 = waiting for '$'  */
   int16_t i;
   uint8_t *envval;
   int16_t envcnt; /* Varname # of chars */

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
static int16_t _builtin_run_command(const uint8_t *cmd)
{
   uint8_t cmdbuf[CONFIG_SYS_CBSIZE]; /* working copy of cmd		*/
   uint8_t *token;                    /* start of token in cmdbuf	*/
   uint8_t *sep;                      /* end of token (separator) in cmdbuf */
   uint8_t finaltoken[CONFIG_SYS_CBSIZE];
   uint8_t *str = cmdbuf;
   uint8_t *argv[16 + 1]; /* NULL terminated	*/
   int16_t argc, inquotes;
   int16_t repeatable = 1;
   int16_t rc = 0;

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
static int16_t _run_command()
{
   int16_t retVal = 0;
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
static int16_t _readline(void)
{
   uint8_t c;

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
static bool _Init(void)
{
   _initConsoleBuffer();
   return true;
}

//-----------------------------------------------------------------------------
// FONCTION    :  _Run
//
// DESCRIPTION :  Appel periodique
//-----------------------------------------------------------------------------
static void _Run(void)
{
   static uint8_t ConsoleState = 0;
   uint16_t i;
   uint8_t c = 0;
   int16_t len;

   //uint32_t sdFreeSpace, sdTotalSpace;

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
