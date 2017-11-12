#include <pthread.h>

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
