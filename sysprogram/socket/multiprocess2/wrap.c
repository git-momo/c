#include "wrap.h"



int Socket(int domain,int type,int protocal){
	int ret;
	if((ret=socket(domain,type,protocal))<0)
		DEATH("socket err");
	return ret;

}

int Bind(int sockfd,const struct sockaddr * addr,socklen_t addrlen){
	int ret;

	if((ret=bind(sockfd,addr,addrlen))<0)
		DEATH("bind err");
	return ret;

}

int Listen(int sockfd,int back_log){
	
	int ret;
	if((ret=listen(sockfd,back_log))<0)
		DEATH("listen err");
	return ret;

}

int Accept(int sockfd,struct sockaddr * addr,socklen_t *addrlen){

	int ret;

	while((ret=accept(sockfd,addr,addrlen))<0){
		if(errno!=ECONNABORTED||errno!=EINTR)
			DEATH("accept err");	
	}
	return ret;
}

ssize_t Read(int fd,void *buf,size_t count){

	ssize_t ret;
	while((ret=read(fd,buf,count))<0){
		if(errno!=EINTR)
			DEATH("read err");
	}
	return ret;
}

ssize_t Write(int fd,void *buf,size_t count){

	ssize_t ret;
	while((ret=write(fd,buf,count))<0){
		if(errno!=EINTR)
			DEATH("write err");
	
	}
	return ret;
}


ssize_t Readn(int fd,void *buf,size_t count){

	
	size_t nleft;
	size_t nread;

	nleft=count;

	while(nleft>0){
		
		if((nread=read(fd,buf,nleft))<0){
			nread=0;
			break;
		}else if(nread==0)
			break;
		nleft-=nread;
		buf+=nread;
			
	}
	return count - nleft;
}

ssize_t writen(int fd,void * buf ,size_t count){

	size_t nleft;
	size_t nwrite;
	
	nleft=count;

	while(nleft>0){

		if((nwrite=write(fd,buf,nleft))<0){
			nwrite=0;
			break;
		}else if(nwrite==0)
			break;
		nleft-=nwrite;
		buf+=nwrite;
	}
	return count - nwrite;
}

static ssize_t _read(int fd,char *c){

	static ssize_t read_cnt;
	static char buf[BUFSIZ];
	static char * read_ptr;
	ssize_t ret;
	if(read_cnt<=0){
		read_cnt=read(fd,buf,BUFSIZ);
		read_ptr=buf;
		if(read_cnt<=0)
			ret=read_cnt;
		
		
	}
	if(read_cnt>0){

		*c=*read_ptr++;
	
		read_cnt--;
		ret=1;
	}

	return ret;
	
}
ssize_t readline(int fd,char *buf,size_t count){
	

	int i=0,ret;
	char c;
	while((ret=_read(fd,&c))>0&&i<count){
		buf[i++]=c;
		if(c=='\n')
			break;
	}
	if(ret==0)
		buf[i]=0;
	if(ret<0)
		i=-1;

	return i;
	
}









