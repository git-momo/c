#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define YIELD 100
#define CONTAINER_SIZE 10
typedef struct{

	int container[CONTAINER_SIZE];
	int start;
	int end;
}Factory;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
void *th_fun(void *arg){
	Factory *factory=(Factory *)arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while(factory->end==factory->start)//queue is empty
			pthread_cond_wait(&cond,&mutex);
		factory->start=(factory->start+1)%CONTAINER_SIZE;
		printf("consumer %ld get No.%d...\n",pthread_self(),factory->container[factory->start]+1);
		if(factory->container[factory->start]==99)
			exit(0);
		pthread_mutex_unlock(&mutex);

	}

}
int main(){	

	Factory *factory;
	int count;
	pthread_t tid;
	pthread_attr_t attr;
	srand(time(NULL));
	factory=calloc(sizeof(Factory),1);
	factory->start=factory->end=-1;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	for(count=0;count<10;count++){
		pthread_create(&tid,&attr,th_fun,factory);
	}


	for(count=0;count<YIELD;count++){
		sleep(1);
		while((factory->end+1)%CONTAINER_SIZE+1==factory->start)//queue is full
			usleep(10);	
		pthread_mutex_lock(&mutex);
		factory->end=(factory->end+1)%CONTAINER_SIZE;
		factory->container[factory->end]=count;
		pthread_mutex_unlock(&mutex);
		//pthread_cond_signal(&cond);
		pthread_cond_broadcast(&cond);
		printf("producter created NO.%d ...\n ",count+1);

	}
	//pthread_cond_broadcast(&completed);
	pthread_exit(NULL);

}
