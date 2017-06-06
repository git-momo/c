#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf(mess);return 1;}


int main (int argc ,char **argv){

	if(argc<2)
		DEATH("usage:./outsize filename\n");
	struct stat st;
	int ret;
	ret=stat(argv[1],&st);
	if(ret<0)
	DEAD("stat err");
	printf("size=%ld\n",st.st_size);
	return 0;
}
