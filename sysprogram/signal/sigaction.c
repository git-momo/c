#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void do_sig(int num){

	printf("num=%d\n",num);
}
int main (){

	
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler=do_sig;
	act.sa_flags=0;
	sigaction(SIGINT,&act,NULL);
	while(1){
		printf("****************\n");
		sleep(1);
	}
	return 0;
}
