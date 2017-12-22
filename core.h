#ifndef _core
#define _core
#include <pthread.h>

typedef struct{
	int x;
	int y;
}Coords;

typedef struct{
	void * data;
	pthread_mutex_t mutex;
}tdata;

typedef enum{
	GENERATE,
	ATTACK,
	DEFENSE
}Status;

typedef enum{
	false,
	true
} bool;

typedef enum{
	MAIN,
	DISPLAY_GAME_INFO,
	DISPLAY_ACTION_CHOICE,
	DISPLAY_UPGRADES
} Menu;

#endif