#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void *th_fun(void *arg){
	printf("thread i=%ld\n",pthread_self());
	printf("thread pid=%d\n",getpid());
	printf("thread arg=%d\n",*(int *)arg);

}
int main(){
 
	pthread_t tid;
	int n=10;
	pthread_create(&tid,NULL,th_fun,(void *)&n);
	printf("man pid=%d\n",getpid());
	printf("man thread id=%ld\n",pthread_self());
	printf("man child thread id=%ld\n",tid);
	while(1);
}
