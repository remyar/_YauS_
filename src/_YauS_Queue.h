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
#include "_YauS_.h"

//------------------------------------------------------------------------------
// Definition de type
//------------------------------------------------------------------------------
typedef struct
{
	UINT32 nbQueueIsUsed;
	UINT32 nbQueueUsedMax;
} s_YAUS_QUEUE_INFO;

typedef struct
{
	UINT32 handle;
	UINT32 idx;
	UINT8 data[YAUS_MAX_LENGTH_MSG];
} s_YAUS_QUEUE; //-- 16 octets par element dans la pipe

extern s_YAUS_QUEUE queue[YAUS_MAX_MSG];

//------------------------------------------------------------------------------
// Fonctions publiques
//------------------------------------------------------------------------------
void YAUS_msgInit(void);

UINT32 YAUS_msgSend(UINT32 handle, void *data);
UINT32 YAUS_msgGetNbElement(UINT32 handle);
UINT32 YAUS_msgRead(UINT32 handle, void *data);
#endif //--- EVENT_H
