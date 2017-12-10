#ifndef _pthread
#define _pthread
#include <pthread.h>
#endif

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
