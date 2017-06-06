#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/types>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define DEAD(mess){perror(mess);exit(-1);}

int main(){
	
	int serverfd,ret,confd,len;
	struct sockaddr_in serveraddr,conaddr;
	char tmp[128]={0};
	
	//1.sodcket
	serverfd=socket(AF_INET,SOCK_STREAM,0);
	if(serverfd<0)
		DEAD("socket err");

	//2.bind
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons(8000);
	ret=bind(serverfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(ret<0)
		DEAD("bind err");
	//listen
	ret=listen(serverfd,128);
	if(ret<0)
		DEAD("listen err");
	
	//accept
	bzero(&conaddr,sizeof(conaddr));
	confd=accept(serverfd,(struct sockaddr *)&conaddr,&len);
	if(confd<0)
		DEAD("accept err");
	bzero(tmp,sizeof(tmp));
	printf("client ip:%s\tport%d\n",inet_ntop(AF_INET,&conaddr.sin_addr.s_addr,tmp,sizeof(tmp)),ntohs(conaddr.sin_port));
	write(confd,"hello world",12);
	close(confd);
	close(serverfd);
	return 0;
	
	
}
