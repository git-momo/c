#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *th_fun(void*arg){
	while(1){
		sleep(1);
		pthread_mutex_lock(&mutex);
		printf("hello ");
		usleep(100);
		printf("world\n");
		//pthread_mutex_unlock(&mutex);
		//sleep(1);
	}
}
int main(){


	pthread_attr_t attr;
	pthread_t tid;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&tid,&attr,th_fun,NULL);
	while(1){
		sleep(1);
		//pthread_mutex_lock(&mutex);
		printf("HELLO ");
		usleep(100);
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex);
		//sleep(1);
	}

}
