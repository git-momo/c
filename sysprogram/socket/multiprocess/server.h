#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>



#define SERVER_PORT 8000
#define BACK_LOG 128
#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf(mess);return -1;}

