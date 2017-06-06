#ifndef __WRAP_H_
#define __WRAP_H_

/*err handing*/
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


#define DEATH(mess){perror(mess);exit(-1);}

#define SERVER_ADDR "0.0.0.0"
#define SERVER_PORT 8000

int Socket(int domain,int type,int protocal);
int Bind(int sockfd,const struct sockaddr * addr,socklen_t addrlen);
int Listen(int sockfd,int back_log);
int Accept(int sockfd,struct sockaddr * addr,socklen_t *addrlen);
ssize_t Read(int fd,void *buf,size_t count);
ssize_t Write(int fd,void *buf,size_t count);
ssize_t Readn(int fd,void *buf,size_t count);
ssize_t Writen(int fd,void * buf ,size_t count);
ssize_t Readline(int fd,char *buf,size_t count);

#endif 
