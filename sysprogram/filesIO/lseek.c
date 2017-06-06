#include <stdio.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define DEAD(mess){perror(mess);exit(-1);}

int main (){

	int fd;
	int offset;
	int re;
	int flags;
	int index=5;
	char buf[100]={0};
	char *tmpbuf=buf;
	int size;
	fd=open("a.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd<0)
		DEAD("open err");
	offset=lseek(fd,0,SEEK_END);
	if(!offset){
		printf("a.text`s size is zero\n");
		offset=lseek(fd,0x1000,SEEK_SET);
		if(offset<0)
			DEAD("lseek err");
		re=write(fd,"\0",1);
		if(re<0)
			DEAD("write err 1");
		lseek(fd,0,SEEK_SET);
		printf("a.txt`s size is 1025 charactors now!\n");
	}else{
		offset=lseek(fd,0,SEEK_END);
		if(offset<0)
			DEAD("lseek err");
		printf("a.txt`s size is %d charactors\n",offset);
	}

	flags=fcntl(STDIN_FILENO,F_GETFL);
	if(flags<0)
		DEAD("fcntl err");
	flags|=O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,flags);
	if(flags<0)
		DEAD("fcntl err");
	re=0;
	do{
		if(((tmpbuf+re)<(buf+100))&&(size=(buf+100-tmpbuf)>0)){
			re=read(STDIN_FILENO,tmpbuf+=re,size);
			if(re<0){
				if(errno==EAGAIN)
					sleep(5-(--index));
				else
					DEAD("read err");
				re=0;
			}

		}else{
			re=write(fd,buf,strlen(buf));
			if(re<0)
				DEAD("write err 2");
			re=0;
			tmpbuf=buf;
			index=5;
		
		}
	}
	while(index>0);

}
