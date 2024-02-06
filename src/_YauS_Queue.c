//==============================================================================
//
// PROJET       : PORTABLE 2013
// MODULE       : Event.c
// DESCRIPTION  : Gestion d'un Evenement
//
//==============================================================================

//==============================================================================
//--- DECLARATIONS
//==============================================================================

//------------------------------------------------------------------------------
// Fichiers Inclus
//------------------------------------------------------------------------------
#include "_YauS_Queue.h"
#include <string.h>

s_YAUS_QUEUE_INFO queueInfo;
s_YAUS_QUEUE queue[YAUS_MAX_MSG];

//==============================================================================
//--- DEFINITIONS
//==============================================================================
void YAUS_msgInit(void)
{
	UINT32 i;

	for (i = 0; i < YAUS_MAX_MSG; i++)
	{
		queue[i].handle = YAUS_QUEUE_NO_HANDLE;
		queue[i].idx = 0;
		memset(queue[i].data, 0, YAUS_MAX_LENGTH_MSG);
	}
}

//------------------------------------------------------------------------------
// FONCTION    : QUEUE_Send
//
// DESCRIPTION :
//------------------------------------------------------------------------------
UINT32 YAUS_msgSend(UINT32 handle, void *data)
{
	UINT32 returnVal = YAUS_QUEUE_NO_HANDLE;
	UINT32 i, idx;

	idx = 0;

	/* on cherche un idx du message en cours */
	for (i = 0; i < YAUS_MAX_MSG; i++)
	{
		if (queue[i].handle == handle)
		{
			if (idx < queue[i].idx)
				idx = queue[i].idx;
		}
	}

	/* on chehce un slot de libre */
	for (i = 0; i < YAUS_MAX_MSG; i++)
	{
		if (queue[i].handle == YAUS_QUEUE_NO_HANDLE)
		{
			queue[i].handle = handle;
			queue[i].idx = idx + 1;
			memcpy(queue[i].data, data, YAUS_MAX_LENGTH_MSG);
			queueInfo.nbQueueIsUsed++;
			if (queueInfo.nbQueueIsUsed > queueInfo.nbQueueUsedMax)
				queueInfo.nbQueueUsedMax = queueInfo.nbQueueIsUsed;
			returnVal = 1;
			break;
		}
	}

	if (queueInfo.nbQueueIsUsed >= (YAUS_MAX_MSG - 16))
	{
		//-- si on arrive ici alors c'est que la queue et presque pleine
		//CONS_flush();
		//CONS_print("WARNING : number of YAUS_msg = %d / %d \r\n", (UINT16)queueInfo.nbQueueIsUsed, YAUS_MAX_MSG);
	}

	if (queueInfo.nbQueueIsUsed >= YAUS_MAX_MSG)
	{
		CONS_flush();
		//-- si on arrive ici alors c'est que la queue et presque pleine
		CONS_print("FATAL ERROR : number of YAUS_msg = %d / %d \r\n", (UINT16)queueInfo.nbQueueIsUsed, YAUS_MAX_MSG);
		CONS_flush();
		while (1)
			;
	}
	return returnVal;
}

//------------------------------------------------------------------------------
// FONCTION    : QUEUE_Send
//
// DESCRIPTION :
//------------------------------------------------------------------------------
UINT32 YAUS_msgGetNbElement(UINT32 handle)
{
	UINT32 i, nb = 0;

	/* on cherche un idx du message en cours */
	for (i = 0; i < YAUS_MAX_MSG; i++)
	{
		if (queue[i].handle == handle)
		{
			nb++;
		}
	}

	return nb;
}

//------------------------------------------------------------------------------
// FONCTION    : QUEUE_Send
//
// DESCRIPTION :
//------------------------------------------------------------------------------
UINT32 YAUS_msgRead(UINT32 handle, void *data )
{
	UINT32 returnLen = 0;
	UINT32 i, idx;

	if (handle != YAUS_QUEUE_NO_HANDLE)
	{
		idx = 0xFFFFFFFF;
		/* on cherche un idx du message en cours */
		for (i = 0; i < YAUS_MAX_MSG; i++)
		{
			if (queue[i].handle == handle)
			{
				if (idx > queue[i].idx)
					idx = queue[i].idx;
			}
		}
		for (i = 0; i < YAUS_MAX_MSG; i++)
		{
			if ((queue[i].handle == handle) && (queue[i].idx == idx))
			{
				memcpy(data, queue[i].data, YAUS_MAX_LENGTH_MSG);
				returnLen = YAUS_MAX_LENGTH_MSG;

				/* on libére le slot */
				queue[i].handle = YAUS_QUEUE_NO_HANDLE;
				queue[i].idx = YAUS_QUEUE_NO_HANDLE;
				memset(queue[i].data, 0, YAUS_MAX_LENGTH_MSG);
				queueInfo.nbQueueIsUsed--;
				break;
			}
		}
	}
	return returnLen;
}
