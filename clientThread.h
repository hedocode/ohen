#ifndef _clientThread
#define _clientThread
#include "player.h"


typedef struct {
	Player ** playersGameInfo;
	int sock;
	int clientCount;
	bool * dead;
} ListenServerThreadArgs;
#endif