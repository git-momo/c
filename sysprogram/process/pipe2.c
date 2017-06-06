#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>
#define DEAD(mess){perror(mess);exit(-1);}

int main(){

	int fds[2];
	pid_t  pid;
	char buf[64]="i am SB";
	if(pipe(fds)<0)
		DEAD("pipe err");
	pid=fork();
	if(pid<0)
		DEAD("fork err");
	if(pid){
		close(fds[0]);
		write(fds[1],buf,strlen(buf));
		wait(NULL);
	}else{
		read(fds[0],buf,sizeof(buf));
		printf("child read=%s\n",buf);	
	}
}
