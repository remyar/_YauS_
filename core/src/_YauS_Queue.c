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
#include "./_YauS_Queue.h"
#include <string.h>

s_YAUS_QUEUE_INFO queueInfo;
s_YAUS_QUEUE queue[YAUS_MAX_MSG];

//==============================================================================
//--- DEFINITIONS
//==============================================================================
void YAUS_msgInit(void)
{
	uint32_t i;

	for (i = 0; i < YAUS_MAX_MSG; i++)
	{
		queue[i].handle = YAUS_QUEUE_NO_HANDLE;
		queue[i].idx = 0;
		memset(queue[i].data, 0, sizeof(queue[i].data));
	}
}

//------------------------------------------------------------------------------
// FONCTION    : QUEUE_Send
//
// DESCRIPTION :
//------------------------------------------------------------------------------
uint32_t YAUS_msgSend(uint32_t handle, void *data)
{
	uint32_t returnVal = YAUS_QUEUE_NO_HANDLE;
	uint32_t i, idx;

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

	if (queueInfo.nbQueueIsUsed >= (YAUS_MAX_MSG - 4))
	{
		//-- si on arrive ici alors c'est que la queue et presque pleine
#ifdef YAUS_USE_MODULE_DRIVERS
		DRIVERS_Process();
#endif
	}

	if (queueInfo.nbQueueIsUsed >= YAUS_MAX_MSG)
	{
#ifdef YAUS_USE_MODULE_DRIVERS
		DRIVERS_Process();
#else
		//-- si on arrive ici alors c'est que la queue et pleine
		while (1)
			;
#endif
	}
	return returnVal;
}

//------------------------------------------------------------------------------
// FONCTION    : QUEUE_Send
//
// DESCRIPTION :
//------------------------------------------------------------------------------
uint32_t YAUS_msgGetNbElement(uint32_t handle)
{
	uint32_t i, nb = 0;

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
uint32_t YAUS_msgRead(uint32_t handle, void *data)
{
	uint32_t returnLen = 0;
	uint32_t i, idx;

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
				memset(queue[i].data, 0, sizeof(queue[i].data));
				queueInfo.nbQueueIsUsed--;
				break;
			}
		}
	}
	return returnLen;
}
