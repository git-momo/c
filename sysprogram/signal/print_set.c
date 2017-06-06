#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void print_sigset(sigset_t *set){

	int i;
	int ret;
	for(i=1;i<32;i++){

		ret=sigismember(set,i);
		if(ret)
			putchar('1');
		else
			putchar('0');
	}	
	puts("");
}

int main(){

	sigset_t s,p;
	sigemptyset(&s);
	sigaddset(&s,SIGINT);
	sigprocmask(SIG_BLOCK,&s,NULL);
	while(1){
		sigpending(&p);
		print_sigset(&p);
		sleep(1);

	}

}
