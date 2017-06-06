#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return 1;}



int main(int argc,char **argv){


	int ret;
	struct stat st;
	mode_t mode;
	int fd;
	char buf[1024]={0};
	FILE* stream;
	char *line;
	char *delim[3];
	int index;
	char user[12]={0};
	char group[12]={0};
	if(argc==1)
		DEATH("usage:./ll filename");
	ret=access(argv[1],F_OK);
	if(ret<0)
		DEAD("access err");
	ret=stat(argv[1],&st);
	if(ret<0)
		DEAD("stat err");
	mode=st.st_mode&S_IFMT;
	switch (mode){
		case S_IFDIR:
			write(STDOUT_FILENO,"d",1);
			break;
		case S_IFREG:
			write(STDOUT_FILENO,"-",1);
			break;
		case S_IFIFO:
			write(STDOUT_FILENO,"f",1);
			break;
		case S_IFBLK:
			write(STDOUT_FILENO,"d",1);
			break;
		default:
			write(STDOUT_FILENO,"-",1);	
			break;
	}
	//user
	mode=st.st_mode&S_IRWXU;
	if(mode&S_IRUSR)
		write(STDOUT_FILENO,"r",1);
	else
		write(STDOUT_FILENO,"-",1);
	if(mode&S_IWUSR)
		write(STDOUT_FILENO,"w",1);
	else
		write(STDOUT_FILENO,"-",1);
	if(mode&S_IXUSR)
		write(STDOUT_FILENO,"x",1);
	else
		write(STDOUT_FILENO,"-",1);
	mode=st.st_mode&S_IRWXG;
	if(mode&S_IRGRP)
		write(STDOUT_FILENO,"r",1);
	else
		write(STDOUT_FILENO,"-",1);
	if(mode&S_IWGRP)
		write(STDOUT_FILENO,"w",1);
	else
		write(STDOUT_FILENO,"-",1);
	if(mode&S_IXGRP)
		write(STDOUT_FILENO,"x",1);
	else
		write(STDOUT_FILENO,"-",1);
	mode=st.st_mode&S_IRWXO;
	if(mode&S_IROTH)
		write(STDOUT_FILENO,"r",1);
	else
		write(STDOUT_FILENO,"-",1);
	if(mode&S_IWOTH)
		write(STDOUT_FILENO,"w",1);
	else
		write(STDOUT_FILENO,"-",1);
	if(mode&S_IXOTH)
		write(STDOUT_FILENO,"x",1);
	else
		write(STDOUT_FILENO,"-",1);

	write(STDOUT_FILENO,"\t",strlen("\t"));
	fprintf(stdout,"%ld\t",st.st_nlink);

	//user
	//fd=open("/etc/passwd",O_RDONLY);
	/*if(fd<0)
	  DEAD("open err");
	  while((ret=read(fd,buf,sizeof(buf))))*/
	stream=fopen("/etc/passwd","r");
	if(!stream)
		DEAD("fopen err");
	int ugflag=2;
	int flag;
	do{
		line=fgets(buf,sizeof(buf),stream);
		index=3;
		flag=1;
		do{

			if(flag){
				flag=0;
				delim[--index]=strtok(buf,":");
			}
			else
				delim[--index]=strtok(NULL,":");
		}while(delim[index]&&index);
		if(atoi(delim[0])==st.st_uid){
			strcpy(user,delim[2]);
			ugflag--;
		}
		if(atoi(delim[0])==st.st_gid){
			strcpy(group,delim[2]);
			ugflag--;
		}			
		memset(buf,0,sizeof(buf));
	}while(line&&ugflag);
	fprintf(stdout,"%s\t%s\n",user,group);






}
