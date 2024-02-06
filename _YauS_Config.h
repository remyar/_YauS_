/********************************************************************

 FileName:      _YauS_Config.c
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

#ifndef _YAUS_CONFIG_H_INCLUDED
#define _YAUS_CONFIG_H_INCLUDED

/** INCLUDES *******************************************************/

/** CONSTANTS ******************************************************/
#define   YAUS_MAX_TASKS                5
#define   TYPE_BOOL_NOT_DEFINED         1

#define   YAUS_MAX_MSG                  64
#define   YAUS_MAX_LENGTH_MSG           32

enum
{
  YAUS_QUEUE_NO_HANDLE = 0, //- NULL
  YAUS_QUEUE_UART1_RX_HANDLE,
  YAUS_QUEUE_UART1_TX_HANDLE,
  YAUS_QUEUE_UART2_RX_HANDLE,
  YAUS_QUEUE_UART2_TX_HANDLE,
  YAUS_QUEUE_UART3_RX_HANDLE,
  YAUS_QUEUE_UART3_TX_HANDLE,
  YAUS_QUEUE_I2C1_RX_HANDLE,
  YAUS_QUEUE_I2C1_TX_HANDLE,
  YAUS_QUEUE_I2C2_RX_HANDLE,
  YAUS_QUEUE_I2C2_TX_HANDLE,
  YAUS_QUEUE_VI2C1_RX_HANDLE,
  YAUS_QUEUE_VI2C1_TX_HANDLE,
  YAUS_QUEUE_PWMPB0_TX_HANDLE,
};

/** MACROS *********************************************************/

/** VARIABLES ******************************************************/

/** DECLARATIONS ***************************************************/

#endif /* _YAUS_CONFIG_H_INCLUDED */
