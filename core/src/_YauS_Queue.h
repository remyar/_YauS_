//==============================================================================
//
// PROJET       : PORTABLE 2013
// HEADER       : Event.h
//
//==============================================================================
#ifndef _YAUS_QUEUE_H
#define _YAUS_QUEUE_H

//------------------------------------------------------------------------------
// Fichiers Inclus
//------------------------------------------------------------------------------
#include "./_YauS_.h"

//------------------------------------------------------------------------------
// Definition de type
//------------------------------------------------------------------------------

enum
{
	YAUS_QUEUE_NO_HANDLE = 0, //- NULL
	YAUS_QUEUE_USB_RX_HANDLE,
	YAUS_QUEUE_USB_TX_HANDLE,
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
	YAUS_QUEUE_RS4851_RX_HANDLE,
	YAUS_QUEUE_RS4851_TX_HANDLE,
};

typedef struct
{
	uint32_t nbQueueIsUsed;
	uint32_t nbQueueUsedMax;
} s_YAUS_QUEUE_INFO;

typedef struct
{
	uint32_t handle;
	uint32_t idx;
	uint8_t data[YAUS_MAX_LENGTH_MSG];
} s_YAUS_QUEUE; //-- 16 octets par element dans la pipe

extern s_YAUS_QUEUE queue[YAUS_MAX_MSG];

//------------------------------------------------------------------------------
// Fonctions publiques
//------------------------------------------------------------------------------
void YAUS_msgInit(void);

uint32_t YAUS_msgSend(uint32_t handle, void *data);
uint32_t YAUS_msgGetNbElement(uint32_t handle);
uint32_t YAUS_msgRead(uint32_t handle, void *data);
#endif //--- EVENT_H
