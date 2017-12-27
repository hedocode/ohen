#ifndef _serverThread
#define _serverThread
#include "player.h"

typedef struct {
	Player ** playersGameInfo;
	int * clientSockets;
	int sock;
	int playerIndex;
	int playerCount;
} ListenClientThreadArgs;
#endif