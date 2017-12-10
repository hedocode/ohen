#ifndef _player
#define _player
#include "player.h"
#endif

typedef struct {
	Player * playersGameInfo;
	int sock;
	int clientCount;
} ListenThreadArgs;