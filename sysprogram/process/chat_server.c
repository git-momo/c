
/*local chat room server port*/
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

typedef struct login_node{
	
	char name[4];//record provider name
	int fifo_fd;//client fifo fd
	int fds[2];//apply to main process and child process communication
	struct login_node *next;
}node;
typedef node *Node;


//server is responsible for maintain login link and resolve package,so u need two process at least
int resolvepk (PK package,Node *list);
Node checklogin(char *name,Node list);
Node logout(char *name,Node list);
int main(){

	int ret;
	int serverfifo_fd;
	pk pkbuf;
	Node login_list=NULL;
	int len;
	//create server_fifo as public pipe,responsible for read package from client
	ret=mkfifo(SERVER_FIFO,0644);
	if(ret<0)
		DEAD("mkfifo err:SERVER_FIFO:");
	serverfifo_fd=open(SERVER_FIFO,O_RDWR);
	if(serverfifo_fd<0)
		DEAD("open err:SERVER_FIFO:");

	
	while(1){
		memset(&pkbuf,0,sizeof(pk));
		len=read(serverfifo_fd,&pkbuf,sizeof(pk));
		if(len<0){
			printf("read SERVER_FIFO failed\n");continue;
		}
		ret=resolvepk(&pkbuf,&login_list);		
		
	}
	
}
//resposible for resolve package 
int resolvepk (PK package,Node *list){

	int ret;
	pid_t pid;
	Node src,des;
	switch (package->num){
		case 1://log in : add user to link ,create processs responsible to write data
			//check src whether logged in
			if(checklogin(package->src,*list))
				break;
			//if not log in ,cretae an node
			Node n=calloc(sizeof(node),1);
			if(n==NULL)
				DEATH(0,"calloc exception",1);
			strncpy(n->name,package->src,strlen(package->src));
			ret=mkfifo(package->src,0644);
			if(ret<0){
				free(n);DEATH(1,"mkfifo exception",1);
			}
			n->fifo_fd=open(package->src,O_RDWR);
			if(n->fifo_fd<0){
				free(n);DEATH(1,"open exception",1);
			}
			ret=pipe(n->fds);
			if(ret<0){
				free(n);DEATH(1,"pipe exception",1);
			}
			n->next=*list;
			*list=n;
			pid=fork();
			if(pid<0){
				free(n);DEATH(1,"fork exception",1);
			}
			if(!pid){
				close(n->fds[0]);//0 express write port
	
				while(1){
					ret=read(n->fds[1],package,PAK_SIZE);
					if(ret>0&&package->num!=3)
						ret=write(n->fifo_fd,package,ret);
					else if(ret=0)
						continue;
					else
						exit(1);
						
				}
			}
			break;
		case 2://trans late data;
			//check src whether logged in
			
			if(!(src=checklogin(package->src,*list)))
				break;
			if(!(des=checklogin(package->des,*list))){
				memset(package->data,0,DATA_SIZE);
				sprintf(package->data,"%s havent logged in",package->des);
				write(src->fds[0],package,PAK_SIZE);
				break;
			}
			write(des->fds[0],package,PAK_SIZE);
			break;
		case 3://log out
			if(!(checklogin(package->src,*list)))
				break;
			write(src->fds[0],package,PAK_SIZE);
			*list=logout(package->src,*list);
			break;
		default :
			break;
				
	}
	return 0;

}

Node checklogin(char *name,Node list){

	while(list){
		if(strcmp(name,list->name)==0)
			return list;
		list=list->next;
	}
	return NULL;
}
Node logout(char *name,Node list){
	Node head=list;
	Node fre=NULL;
	while(list){
		if(strcmp(name,list->name)==0)
			if(!fre)
				head=list->next,free(list);
			else
				fre->next=list->next,free(list);
		else
			fre=list,list=list->next;	
	}
	return head;

}
