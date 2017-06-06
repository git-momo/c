#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define PRINT_ERR(mess)(perror(mess);printf("errno=%d\n",errno);exit(-1))
#define FAILE(mess)(fprintf(stdout,"%s:%s\n",mess,strerror(errno)))

/*nonblock to read terminal by change terminal file`s block attribute*/
int main(){

	int fd;
	int re;
	char buf[1024]={0};	
	fd=open("/dev/tty",O_RDONLY|O_NONBLOCK);
	if(fd<0)
		PRINT_ERR("main open");
	while((re=read(fd,buf,sizeof(buf)))<0){
		if(errno==EAGAIN){
		
			FAILE("try again");
		}
		else
			PRINT_ERR(" main read");	
	}
	write(STDOUT_FILENO,buf,re);
	return 0;


}
