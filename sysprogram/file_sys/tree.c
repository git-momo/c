#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>


#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess),return 1;}
#define DEATH_2(mess){printf("%s\n",mess);return;}
#define FILE_MAX_SIZE 1024


void fsize(char *name){
	struct stat st;
	int ret;
	DIR *dirp;
	struct dirent *item;
	char parent_name[1024]={0};
	//jduge the file whether directory
	ret=stat(name,&st);
	if(ret<0){
		sprintf(name,"%s:stat err",name);
		DEAD(name);
	}
	if((st.st_mode&S_IFMT)==S_IFDIR){
		//directory
		dirp=opendir(name);
		if(dirp==NULL)
			DEAD("opendir err");
		while((item=readdir(dirp))!=NULL){
			if(strcmp(item->d_name,".")==0||strcmp(item->d_name,"..")==0)
				continue;
			//judge filename length
			if((strlen(name)+item->d_reclen+2)>FILE_MAX_SIZE)
				DEATH_2("file` name is too long");
			sprintf(parent_name,"%s/%s",name,item->d_name);
			fsize(parent_name);
			memset(parent_name,0,FILE_MAX_SIZE);
		}
		closedir(dirp);
	}else{
		printf("%s\n",name);
	}
}

int main(int argc,char **argv){
	

	char file_name[FILE_MAX_SIZE]={0};
	//judge arguments
	if(argc==1){
		strncpy(file_name,".",1);
		fsize(file_name);	
	}
	else
		while(argc-->1){
			memset(file_name,0,sizeof(file_name));
			strcpy(file_name,*++argv);
			fsize(file_name);	
		}
	
}
