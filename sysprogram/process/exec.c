#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return 1;}


int main(int argc,char **argv){

	if(argc<2)
		DEATH("usage:./exec filename");
	

	int fd;
	int ret;
	fd=open(argv[1],O_RDWR);
	if(fd<0)
		DEAD("open err");
	ret=dup2(fd,STDIN_FILENO);
	if(ret<0)
		DEAD("dup err");
	pid_t pid;
	pid=fork();
	if(pid<0)
		DEAD("fork err");
	if(pid){
		while(1);
	}else{
		execl("./upper","upper",NULL);
		DEAD("exec err");
	}
	return 0;


}
