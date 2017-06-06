#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_SIZE 8192

int main(int argc,char *argv[]){

	//judge parameter
	if(argc<3){
		printf("usage:filepath srcfile desfile\n");
		return -1;
	}
	int src_fd,des_fd;
	src_fd=open(argv[1],O_RDONLY);
	if(src_fd<0){
		printf("src_file open err\n");
		return -1;
	}
	if((des_fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))<0){
		printf("des_file open err\n");
		return -1;
	}
	char buff[MAX_SIZE]={0};
	int len;
	while((len=read(src_fd,buff,sizeof(buff)))>0){
		write(des_fd,buff,len);
	}
	if(len==0)
		return 0;
	else
		return -1;
}
