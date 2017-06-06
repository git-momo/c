#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define DEAD(mess){perror(mess);exit(-1);}


int main(){

	int fds[2];
	int ret;
	ret=pipe(fds);
	if(ret<0)
		DEAD("pipe err");
	struct stat st;
	fstat(fds[0],&st);
	printf("st_dev=%ld\t",st.st_dev);
	printf("st_ion=%ld\t",st.st_ino);
	printf("st_mode=%o\t",st.st_mode);
	printf("st_nlink=%ld\t",st.st_nlink);
	printf("st_uid=%d\t",st.st_uid);
	printf("st_gid=%d\t",st.st_gid);
	printf("st_size=%ld\n",st.st_size);
}
