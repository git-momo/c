/*local chat room client port*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>


#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(flag,mess,num){flag?printf("%s\n",mess):perror(mess);return num;}

#define SERVER_FIFO "SERVER_FIFO"
#define PAK_SIZE 1036//int num,char src[4],char des[4],char data[1024]
#define DATA_SIZE 1024

typedef struct package{

	int num;//1 express log in ,2 express communication ,3 express log out
	char src[4];//record provider name;
	char des[4];//record consumer name;
	char data[1024];//data 
}pk;
typedef pk *PK;

int main(int argc,char **argv){

	if(argc<2)
		DEATH(1,"usage:./chat_cli username",1);
	if(strlen(argv[1])>3)
		DEATH(1,"username too long",1);
	//u need two process,the one responsible for reading terminal and write public pipe,the
	//other one responsible for reading private pipe and write terminal
	pid_t pid;
	pid=fork();
	int num;
	int server_fd;
	int private_fd;
	pk pkbuf;
	char des[4]={0};
	int ret;
	server_fd=open(SERVER_FIFO,O_RDWR);
	if(server_fd<0)
		DEAD("open err:SERVER_FIFO:");
	if(pid<0)
		DEAD("fork err");
	if(pid){
		while(1){
			printf("plz enter the package num:1 log in\t2:communication\t3:log out\n");
			sprintf(pkbuf.src,"%s",argv[1]);
			scanf("%d",&pkbuf.num);
			if(pkbuf.num==2){
				printf("please input ur friends` id\n");
				scanf("%s",des);
				if(strlen(des)>3){
					printf("usernmae is too long\n");
					continue;
				}	
				sprintf(pkbuf.des,"%s",des);
				printf("enter ur data\n");
				scanf("%s",pkbuf.data);
			}
			write(server_fd,&pkbuf,PAK_SIZE);
			memset(&pkbuf,0,PAK_SIZE);

		}
	}else{

		sleep(120);
		private_fd=open(argv[1],O_RDWR);
		if(private_fd<0)
			DEAD("open err:private pipe:")
		while(1){

			ret=read(private_fd,&pkbuf,PAK_SIZE);
			if(pkbuf.num==3)
				exit(1);
			printf("src:%s\n",pkbuf.src);
			printf("%s\n",pkbuf.data);
			memset(&pkbuf,0,PAK_SIZE);

		}
	}

}
