#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#define TYPE_MASK 0700
#define IS_DIR 0400
#define IS_FIL 0200
#define ACES_MASK 0070
#define IS_ACES 0040
#define IS_NONACES 0020

#define MAX_NAME_SIZE 1024

#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return -1;}
#define DEATH2(mess){printf("%s\n",mess);return NULL;}


typedef struct node {

	int flag;//judge this file is directory
	char name[1024];
	time_t last_mtime;//last modify time
	struct node *childs;//if this file is directory,this attr would be used
	struct node *next;

}Node ;
typedef Node *Listener;
int daemonized(){

	pid_t pid;
	pid=fork();
	int fd;
	int ret;
	time_t t;
	if(pid<0)
		DEAD("fork err");
	if(pid)
		exit(0);
	umask(0);
	fd=open("/home/momo/listen.log",O_RDWR|O_CREAT|O_APPEND,0644);
	if(fd<0)
		DEAD("open err");	
	pid=setsid();
	if(pid<0)
		DEAD("setsid err");
	ret=dup2(fd,STDOUT_FILENO);
	if(ret<0)
		DEATH("dup2 err...out");
	ret=dup2(fd,STDIN_FILENO);
	if(ret<0)
		DEATH("dup2 err...in");
	ret=dup2(fd,STDERR_FILENO);
	if(ret<0)
		DEATH("dup2 err...err");
	close(fd);

	t=time(&t);
	if(t<0)
		DEATH("time err...");

	printf("daemon process starting %s\n",ctime(&t));
	fflush(stdout);

	return 0;

}
int creat_listener(Node **listener,struct stat *st,const char *pathname,int isadd){
	Listener l=*listener;
	Node * n;
	DIR* dir;
	char tmp[1024]={0};
	struct dirent *dirp;
	int ret;
	n=calloc(sizeof(Node),1);
	if(!n)
		DEAD("malloc err");
	//time
	n->last_mtime=st->st_mtime;
	//name
	strcpy(n->name,pathname);
	if(!isadd)
		n->flag|=IS_NONACES;
	else{
		n->flag|=IS_ACES;
		printf("%s be created at %s",pathname,ctime(&st->st_mtime));
		fflush(stdout);
	}	
	if((st->st_mode&S_IFMT)==S_IFDIR){//directory
		//flag
		n->flag |= IS_DIR;
		dir=opendir(pathname);
		if(!dir)
			DEATH("opendir err...");
		while((dirp=readdir(dir))!=NULL){
			if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
				continue;
			if(strlen(pathname)+strlen(dirp->d_name)+2>MAX_NAME_SIZE)
				DEATH("level too deep");
			//childs;
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%s/%s",pathname,dirp->d_name);
			if(stat(tmp,st)<0)
				DEATH("stat err...");
			ret=creat_listener(&n->childs,st,tmp,isadd);
			if(ret<0)
				DEATH("creat_listener exception...");		
		}		


	}else{//underectory
		//flag
		n->flag|=IS_FIL;
		//childs
		n->childs=NULL;

	}
	//next;
	n->next=*listener;
	*listener=n;

	return 0;
}
void print_listener(Listener listener,int level){
	int i;
	while(listener){
		i=0;
		while(i++<level)
			printf("\t");
		(listener->flag&ACES_MASK)==IS_ACES?printf("accessed "):printf("non-accessed ");
		if((listener->flag&TYPE_MASK)==IS_DIR){//derectory
			printf("type:directory name:%s\n",listener->name);
			print_listener(listener->childs,level+1);

		}	
		else//undirectory
			printf("type:un-directory name:%s\n",listener->name);
		fflush(stdout);

		listener=listener->next;

	}

}
void free_listener(Listener listener){
	Node *tmp;
	while(listener){
		tmp=listener->next;
		if((listener->flag&TYPE_MASK)==IS_DIR){//derectory
			free_listener(listener->childs);	
		}
		free(listener);
		listener=tmp;
	}

}
Listener delete_listener(Listener listener){
	Node *fre=NULL;
	Node *tmp=listener;
	time_t t;
	time(&t);
	while(tmp){
		if((tmp->flag&ACES_MASK)==IS_ACES){//?printf("accessed "):printf("non-accessed ");
			//dont be deleted
			tmp->flag&=(~ACES_MASK);
			tmp->flag|=IS_NONACES;
			if((tmp->flag&TYPE_MASK)==IS_DIR){//derectory
				tmp->childs=delete_listener(tmp->childs);
			}
			fre=tmp;
			tmp=tmp->next;

		}else{//have deleted
			if((tmp->flag&TYPE_MASK)==IS_DIR){//derectory
				printf("type:directory name:%s have be deleted at %s",tmp->name,ctime(&t));
				free_listener(tmp->childs);
				tmp->childs=NULL;	
			}else
				printf("type:undirectory name:%s have be deleted at %s",tmp->name,ctime(&t));
			fflush(stdout);
			if(fre){
				fre->next=tmp->next;
				free(tmp);
				tmp=fre->next;
			}else{//first one;
				fre=tmp;
				tmp=fre->next;
				free(fre);
				fre=NULL;
				listener=tmp;
			}


		}

	}
	return listener;

}
//n->flag&=(~AECS_MASK);
//n->flag|=IS_ACES;


Listener scan_listener(char *pathname,Listener listener){
	Node *n=listener;
	struct stat st;
	int flag=0;
	DIR *dir;
	struct dirent *dirp;
	char tmp[1024]={0};
	if(stat(pathname,&st)<0)
		DEATH2("stat err...");
	if((st.st_mode&S_IFMT)==S_IFDIR)
		flag=IS_DIR;
	else
		flag=IS_FIL;
	while(n){
		//printf("n->name=%s\n",n->name);
		//printf("pathname=%s\n",pathname);
		if(strcmp(n->name,pathname)==0&&(n->flag&TYPE_MASK)==flag){//accessed exist node
			n->flag&=(~ACES_MASK);
			//printf("n->flag=%0d\n",n->flag);
			n->flag|=IS_ACES;
			if(n->last_mtime!=st.st_mtime){//modify
				n->last_mtime=st.st_mtime;
				printf("%s modifed at %s",pathname,ctime(&n->last_mtime));
				fflush(stdout);
			}
			if(flag==IS_DIR){//directory
				dir=opendir(pathname);
				if(!dir)
					DEATH2("opendir err...");
				while((dirp=readdir(dir))!=NULL){
					if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
						continue;
					if(strlen(pathname)+strlen(dirp->d_name)+2>MAX_NAME_SIZE)
						DEATH2("level too deep");
					//childs;
					memset(tmp,0,sizeof(tmp));
					sprintf(tmp,"%s/%s",pathname,dirp->d_name);
					if((n->childs=scan_listener(tmp,n->childs))==NULL)
						DEATH2("scan_listener exception...");
				}
			}//undirectory

			break;
		}
		n=n->next;
	}
	if(!n){//new node
		//		n=calloc(sizeof(Node),1);
		//		if(!n)
		//			DEATH2("malloc err...");
		//flag
		//		flag|=IS_ACES;
		//		n->flag=falg;
		//name
		//		strcpy(n->name,pathname);
		//		//childs
		//		if((flag&TYPE_MASK)==IS_DIR){//directory
		if(creat_listener(&n,&st,pathname,1)<0)
			DEATH2("creat_listener err");
		n->next=listener;
		listener=n;	
		//		}


	}
	return listener;



}
int main(char argc,char **argv){

	if(argc==1)
		DEATH("usage:./listener path");


	int ret;
	struct stat st;
	Listener listener=NULL;
	ret=daemonized();
	if(ret<0)
		DEATH("damonized exception...");

	ret=stat(argv[1],&st);
	if(ret<0)
		DEAD("stat err");
	ret=creat_listener(&listener,&st,argv[1],0);
	if(ret<0)
		DEATH("creat_listener exception");
	//print_listener(listener,0);
	while(1){
		sleep(10);
		listener=scan_listener(argv[1],listener);
		//print_listener(listener,0);
		listener=delete_listener(listener);
	}

}
