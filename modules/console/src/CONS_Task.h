//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// HEADER       : Tick.h
//
//=============================================================================

#ifndef	_CONSOLE_H_
#define _CONSOLE_H_

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include	"_YauS_.h"
/*
#ifdef USE_USB_CONSOLE
   #include "USB_CDC_Driver.h"

   #define CONS_putc(X)        usb_cdc_putc(X)
   #define CONS_kbhit()        usb_cdc_kbhit()
   #define CONS_getc()         usb_cdc_getc()
   #define  CONS_flush()        usb_cdc_task();DELAY_ms(1)

#endif
#ifdef USE_UART_CONSOLE
   #include "UART_Driver.h"

   #define CONS_putc(X)        uart_putc(X)
   #define CONS_kbhit()        uart_kbhit()
   #define CONS_getc()         uart_getc()
#endif
*/
void CONS_print(const UINT8 *pFormat, ...);
void CONS_printFloat(FLOAT32 value, UINT8 nbDec) ;
void CONS_Init(void);

#endif		//--- _TICK_H_

