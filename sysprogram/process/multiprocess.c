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

int main(int argc,char **argv){

	int process_num;
	if(argc<3)
		DEATH("usage:./multiprocess srcfilename desfilename processnum");
	if(argc>3)
		process_num=atoi(argv[3]);
	int src_fd;
	int len;
	int ret;
	int block_size;
	int i;
	int des_fd;
	pid_t pid;
	char **trans_argv;
	src_fd=open(argv[1],O_RDWR);
	if(src_fd<0)
		DEAD("open src err");
	len=lseek(src_fd,0,SEEK_END);
	if(len<0)
		DEAD("seek err");
	if(len<MINIMUM_SIZE)
		process_num=1;
	ret=lseek(src_fd,0,SEEK_SET);
	if(ret<0)
		DEAD("seek err:reset");

	trans_argv=calloc(sizeof(char *),6);
	for(i=1;i<3;i++){
		trans_argv[i]=calloc(STR_SIZE,1);
		if(!trans_argv[i])DEAD("mlloc err");
	}
	if(!trans_argv)
		DEAD("malloc err");
	des_fd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
	if(des_fd<0)
		DEAD("open des err");
	trans_argv[0]=NULL;
	trans_argv[5]=NULL;
	sprintf(trans_argv[1],"%d",src_fd);
	sprintf(trans_argv[2],"%d",des_fd);
	block_size=len/process_num;
	for(i=0;i<process_num;i++){
		pid=fork();
		if(pid<0)
			DEAD("fork err");
		if(!pid){//args:sec_fd  src_fd  off_set blocksize;
	trans_argv[3]=calloc(STR_SIZE,1),trans_argv[4]=calloc(STR_SIZE,1),sprintf(trans_argv[3],"%d",block_size*i),sprintf(trans_argv[4],"%d",i==process_num-1?block_size+(len%process_num):block_size),execv("./copy",trans_argv);
		DEAD("exec err");
		}
	}
	i=0;
	if(pid)
	while(i<process_num)
		if(waitpid(-1,NULL,WNOHANG)>0)i++;
		else
			printf("process is working"),sleep(1);



	return 0;	

}
