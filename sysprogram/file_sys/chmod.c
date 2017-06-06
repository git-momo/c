#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return 1;}


int main(int argc ,char **argv){


	if(argc<3)
		DEATH("usage:./chmod filename mode");

	int ret;
	ret=access(argv[1],F_OK);
	if(ret<0)
		DEAD("access file");
	mode_t mod=(mode_t)strtol(argv[2],NULL,8);
	ret=chmod(argv[1],mod);
	if(ret<0)
		DEAD("chmod err");
	return 0;	

}

