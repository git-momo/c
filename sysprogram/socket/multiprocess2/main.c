#include  "wrap.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <signal.h>
#include <wait.h>
#include <arpa/inet.h>

void do_sig(int num){

	while(waitpid(0,NULL,WNOHANG)>0);
}
int main(){
	

	int server_fd,client_fd,ret,i;
	pid_t pid;socklen_t len;
	ssize_t n;
	char buf[BUFSIZ];
	struct sockaddr_in serveraddr,clientaddr;
	server_fd=Socket(AF_INET,SOCK_STREAM,0);
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	ret=inet_pton(AF_INET,SERVER_ADDR,&serveraddr.sin_addr.s_addr);
	if(ret<0)
		DEATH("inet_pton err");
	serveraddr.sin_port=htons(SERVER_PORT);
	Bind(server_fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	Listen(server_fd,128);

	//registe signal handler
	struct sigaction act;
	act.sa_flags=0;
	act.sa_handler=do_sig;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD,&act,NULL);
	while(1){
		len=sizeof(clientaddr);
		client_fd=Accept(server_fd,(struct sockaddr *)&clientaddr,&len);
		pid=fork();
		if(pid<0)
			DEATH("fork err");
		if(!pid)
			break;
		else
			close(client_fd);
	}

	if(!pid){
		
		close(server_fd);
		while((n=Read(client_fd,buf,BUFSIZ))>0){
			for(i=0;i<n;i++){
				buf[i]=toupper(buf[i]);
			}
			Write(client_fd,buf,n);
		}
		return 0;
		
	}
	return 0;
	
	
}
