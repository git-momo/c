#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>


#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess),return 1;}

void daemnized(int fd){
	pid_t pid;
	pid=fork();
	if(pid<0)
		DEAD("fork err");
	if(pid)
		exit(0);
	else{
		int ret;
		ret=dup2(fd,STDIN_FILENO);
		if(ret<0)
			DEAD("dup2 err");
		dup2(fd,STDOUT_FILENO);
		if(ret<0)
			DEAD("dup2 err");
		dup2(fd,STDERR_FILENO);
		if(ret<0)
			DEAD("dup2 err");
		
		pid=setsid();
		if(pid<0)
			DEAD("setsid err");
		ret=chdir("/home/momo");
		if(ret<0)
			DEAD("chdir err");
		umask(0);
	}
	

}

int main()
{
	int fd;
	char buf[1024]={0};
	fd=open("/home/momo/lisenerd.log",O_RDWR|O_CREAT|O_APPEND,0644);
	if(fd<0)
		DEAD("open err");

	daemnized(fd);
	close(fd);
	time_t t;
	time(&t);
	sprintf(buf," lisener process starting ...%s",ctime(&t));
	//printf(" lisener process starting %s ....\n",ctime(&t));
	write(STDOUT_FILENO,buf,strlen(buf));
	
	while(1){
		

	}	
	
}
