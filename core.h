#include <pthread.h>

typedef struct{
	int x;
	int y;
}coords;

typedef struct{
	void * data;
	pthread_mutex_t mutex;
}tdata;

typedef enum{
	GENERATE,
	ATTACK,
	DEFENSE
}status;

typedef enum{
	false,
	true
} bool;

typedef enum{
	BLUE,
	GREEN,
	DARK,
	YELLOW,
	RED,
	PINK
}color;

typedef color Color;
typedef coords Coords;
typedef tdata Tdata;
typedef status Status;