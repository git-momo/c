#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>

#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return 1;}

int main(){
	
	int fd;
	int save_fd;
	int ret;
	int random;
	int index=-1;
	int array[1000]={0};
	fd=open("in.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd<0)
		DEAD("open err");
	srand((unsigned int)time(NULL));
	save_fd=dup(STDOUT_FILENO);
	if(save_fd<0)
		DEAD("dup err");
	ret=dup2(fd,STDOUT_FILENO);
	if(ret<0)
		DEAD("dup2 err");
	while(++index<1000){
		random=rand()%(1000-100+1)+100;
		printf("%d\t",random);
		array[index]=random;
	}
	printf("\n");
	//close(fd);
	dup2(STDOUT_FILENO,save_fd);
	close(save_fd);
	//bubblesort
	int i,j,tmp;
	for(i=0;i<=index;i++){
		for(j=0;j<=index-i;j++){
			if(array[j]>array[i+1]){
				tmp=array[i];
				array[i]=array[i+1];
				array[i+1]=tmp;
				
			}
		}

	}
	for(i=0;i<1000;i++)
		printf("%d\t",array[i]);
	
	index=0;
	save_fd=dup(STDIN_FILENO);
	//fd=open("in.txt",O_RDWR);
	ret=dup2(fd,STDIN_FILENO);
	if(ret<0)
		DEAD("dup2 err");
	while((scanf("%d",&array[index++]))!=EOF);
	for(i=0;i<=index;i++){
		for(j=0;j<=index-i;j++){
			if(array[j]>array[i+1]){
				tmp=array[i];
				array[i]=array[i+1];
				array[i+1]=tmp;
				
			}
		}

	}
	close(fd);
	dup2(STDIN_FILENO,save_fd);
	close(save_fd);
	fd=open("out.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	save_fd=dup(STDOUT_FILENO);
	ret=dup2(fd,STDOUT_FILENO);
	if(ret<0)
		DEAD("dup2 err");
	for(i=0;i<1000;i++)
		printf("%d\t",array[i]);
	
	
	
}


