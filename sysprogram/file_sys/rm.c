#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return 1;}



void realrm(const char *filename){
	struct stat st;
	struct dirent *fp;
	DIR * dirp;
	char * childname=NULL;
	int ret;
	
		ret=stat(filename,&st);
	if(ret<0)
		DEAD("stat err");
	if((st.st_mode&S_IFMT)!=S_IFDIR)
		unlink(filename);
	else{
		dirp=opendir(filename);
		if(!dirp)
			DEAD("opendir err");
		while((fp=readdir(dirp))){
			if(strcmp(fp->d_name,".")==0||strcmp(fp->d_name,"..")==0)
				continue;
			childname=calloc(strlen(filename)+strlen(fp->d_name)+2,0);
			if(!childname)
				DEAD("malloc err");
			sprintf(childname,"%s/%s",filename,fp->d_name);
			realrm(childname);
		
			if(childname)
				memset(childname,0,strlen(childname)+1);
		}
		closedir(dirp);
		ret=rmdir(filename);
		if(ret<0)
			DEAD("rmdir err");	
		if(childname)
			free(childname);
	}
	
	printf("delete %s sucessful\n",filename);


}

int main(int argc,char **argv){


	if(argc==1)
		DEATH("usage:./rm filename");
	realrm(argv[1]);
	return 0;
}
