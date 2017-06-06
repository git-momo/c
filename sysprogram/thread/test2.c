#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>


void *th_fun1(void *arg){

	printf("thread1 returnning\n");
	return (void *)1;
}
void *th_fun2(void *arg){
	printf("thread2 exitting\n");
	pthread_exit((void *)2);

}
void *th_fun3(void *arg){

	while(1)
		printf("thread3 is writing\n"),sleep(1);
}
int main(){


	pthread_t tid;
	void*retval;
	pthread_create(&tid,NULL,th_fun1,NULL);
	pthread_join(tid,&retval);
	printf("man thread 1 return value=%d\n",(int)retval);
	pthread_create(&tid,NULL,th_fun2,NULL);
	pthread_join(tid,&retval);
	printf("man thread 2 return value=%d\n",(int)retval);
	pthread_create(&tid,NULL,th_fun3,NULL);
	sleep(3);
	pthread_cancel(tid);
	pthread_join(tid,&retval);
	printf("man thread 3 return value=%d\n",(int)retval);
	return 0;
	
}
