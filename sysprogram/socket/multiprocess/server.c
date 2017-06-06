#include "server.h"

//daemon return 0 successful,return -1 express defeat
void daemonized(){

	pid_t pid;
	int ret,fd;
	time_t t;	
	//fork to create an new process
	pid=fork();
	if(pid<0)
		DEAD("fork err");
	if(pid)
		exit(0);
	//invoke setsid() to create an new sesssion
	pid=setsid();
	if(pid<0)
		DEAD("setsid err");
	//change working directory to HOME

	ret=chdir("/home/momo");
	if(ret<0)
		DEAD("chdir err");
	umask(0);
	//create logger for recoding state
	fd=open("/home/momo/sys.log",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd<0)
		DEAD("open err");
	ret=dup2(fd,STDOUT_FILENO);
	if(ret<0)
		DEAD("dup2 err");
	ret=dup2(fd,STDIN_FILENO);
	if(ret<0)
		DEAD("dup2 err");
	ret=dup2(fd,STDERR_FILENO);
	if(ret<0)
		DEAD("dup2 err");
	t=time(NULL);
	printf("server start at %s",ctime(&t));
	fflush(stdout);


	close(fd);


}

int main (){

	struct sockaddr_in seraddr,conaddr;
	int sockfd,ret,confd;
	socklen_t addrlen;
	pid_t pid;
	time_t t;
	char str[128];
	//daemon
	daemonized();
	//bussnise logic
	while(1){
		//create listenr socket
		sockfd=socket(AF_INET,SOCK_STREAM,0);//tcp protocal stack	
		if(sockfd<0)
			DEAD("socket err");
		//init seraddr;
		bzero(&seraddr,sizeof(seraddr));
		seraddr.sin_family=AF_INET;
		seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
		seraddr.sin_port=htons(SERVER_PORT);
		//bind seraddr
		ret=bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
		if(ret<0)
			DEAD("bind err");
		//listen connect
		ret=listen(sockfd,BACK_LOG);
		if(ret<0)
			DEAD("listen err");



		while(1){
			//accept
			addrlen=sizeof(conaddr);
			confd=accept(sockfd,(struct sockaddr *)&conaddr,&addrlen);
			if(confd<0)
				DEAD("accept err");
			pid=fork();
			if(pid<0){
				printf("fork failed");
				fflush(stdout);
			}
			if(!pid)
				break;	
		}
		if(!pid){
			t=time(NULL);
			memset(str,0,sizeof(str));
			printf("clinet %s %d connected at %s",inet_ntop(AF_INET,&conaddr.sin_addr.s_addr,str,sizeof(str)),ntohs(conaddr.sin_port),ctime(&t));
			write(confd,"OK",2);
		}

	}
























































































































































































































































































































































































































































































































































































































































































}



