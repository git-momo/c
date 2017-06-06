#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#define DEATH(num){printf("%s\n",strerror(num));return -1;}
void *th_fun(void *arg){
	int n=10;
	while(n){
		printf("n=%d\n",n--);sleep(1);
	}
	return (void *)1;
}
int main (){
	int ret;
	void *retval;
	pthread_t tid;
	pthread_create(&tid,NULL,th_fun,NULL);
	pthread_detach(tid);
	ret=pthread_join(tid,&retval);
	if(ret!=0){
		DEATH(ret);
	}
	else
		printf("man thread recovry sucessful,retval=%d\n",(int)retval);
	return 0;
	
	

}
