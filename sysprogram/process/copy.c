#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define DEATH(mess) { printf("%s\n",mess);return 1; }
#define DEAD(mess) { perror(mess);exit(-1); }
#define MINIMUM_SIZE 128
#define STR_SIZE 5


int main(int argc ,char **argv){
	int src_fd=atoi(argv[1]);
	int des_fd=atoi(argv[2]);
	int offset=atoi(argv[3]);
	int block_size=atoi(argv[4]);
	char buf[4096]={0};
	int len;
	if(block_size>4095)
		DEATH("size to big")
	len=read(src_fd,buf,sizeof(buf));
	if(len<0)
		DEAD("read err");
	write(des_fd,buf,len);

}
