#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return -1;}
#define DEATH2(mess){printf("%s\n",mess);return NULL;}
#define CONTAINER_SIZE 10
#define YILED 1000
#define PROTH_NUM 10
#define CONTH_NUM 100

typedef struct {

	int container[CONTAINER_SIZE];
	int start;
	int end;
}Factory;
typedef struct{
	Factory *factory;
	int th_num;
}Arg;
sem_t sem;//producter
sem_t sem2;//consumer;
void *product(void *arg){
	Arg *_arg=(Arg *)arg;
	int tmp;
	while(1){
		sleep(1);
		sem_wait(&sem);
		//while(((_arg->factory->end+1)%CONTAINER_SIZE==_arg->factory->start))
		//usleep(10);
		if(_arg->factory->end==-1)
			tmp=0;
		else
			tmp=_arg->factory->container[_arg->factory->end];
		_arg->factory->end=(_arg->factory->end+1)%CONTAINER_SIZE;
		if(++tmp>YILED)
			pthread_exit(NULL);
		_arg->factory->container[_arg->factory->end]=tmp;
		sem_post(&sem2);
		printf("No.%ld productor producted No.%d...\n",pthread_self(),tmp);

	}

}
void * consum(void *arg){
	Arg *_arg=(Arg *)arg;
	int tmp;
	while(1){
		sem_wait(&sem2);
		_arg->factory->start=(_arg->factory->start+1)%CONTAINER_SIZE;
		tmp=_arg->factory->container[_arg->factory->start];
		sem_post(&sem);
		printf("No.%ld consumer consumed No.%d...\n",pthread_self(),tmp);
		if(tmp==YILED)
			exit(0);
	}
}
int main(){

	Factory *factory;
	pthread_attr_t attr;
	Arg *arg;
	pthread_t tid;
	int ret,i;
	arg=calloc(sizeof(Arg),1);
	if(!arg)
		DEAD("malloc err");
	factory=calloc(sizeof(Factory),1);
	if(factory==NULL)
		DEAD("malloc err");
	//initialize factory
	factory->start=-1;
	factory->end=-1;
	arg->factory=factory;
	pthread_attr_init(&attr);
	ret=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	if(ret)
		DEATH(strerror(ret));

	ret=sem_init(&sem,0,CONTAINER_SIZE-1);
	if(ret)
		DEATH(strerror(ret));
	ret=sem_init(&sem2,0,0);
	if(ret)
		DEATH(strerror(ret));
	for(i=0;i<PROTH_NUM;i++){
		//arg->th_num=i+1;
		ret=pthread_create(&tid,&attr,product,arg);
		if(ret)
			DEATH(strerror(ret));
	}
	for(i=0;i<CONTH_NUM;i++){
		//arg->th_num=i+1;
		ret=pthread_create(&tid,&attr,consum,arg);
		if(ret)
			DEATH(strerror(ret));
	}
	pthread_exit(0);

}
