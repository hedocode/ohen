#ifndef _serverThread
#define _serverThread
#include "../lib/player.h"

typedef struct {
	Player ** playersGameInfo;
	int * clientSockets;
	int sock;
	int playerIndex;
	int playerCount;
	bool * end;
} ListenClientThreadArgs;
#endif