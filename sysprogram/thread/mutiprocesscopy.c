#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>


#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return -1;}
#define DEATH2(mess){printf("%s\n");return NULL;}

#define LIMIT_SIZE 0xA00000//10M divide multiparts is source file size excess this limit
#define PART_SIZE 0x600000//6M
#define DEAULT_THREAD_NUM 5


typedef struct{
	char *src;
	char *des;
	off_t size;
}Node;
void * th_fun(void *arg){
	Node *node=(Node *)arg;
	strncpy(node->des,node->src,node->size);
	return NULL;

}
int copy(char* src,off_t size,char *des,int num){
	if(size<num)
		num=1;
	int i,ret;
	pthread_attr_t attr;
	pthread_t tid;
	Node node;
	off_t block_size,surplus_block_size;
	//init thread attribution
	ret=pthread_attr_init(&attr);
	if(ret!=0)
		DEATH(strerror(ret));
	ret=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	if(ret!=0)
		DEATH(strerror(ret));
	block_size=size/num;
	surplus_block_size=size%num;
	for(i=0;i<num;i++){
		node.src=(src+block_size*i);
		node.des=(des+block_size*i);
		if(i==num-1)
			block_size+=surplus_block_size;
		node.size=block_size;
		ret=pthread_create(&tid,&attr,th_fun,&node);
		if(ret!=0)
			DEATH(strerror(ret));
		ret=pthread_join(tid,NULL);
		if(ret!=0)
			DEATH(strerror(ret));
	}
	pthread_attr_destroy(&attr);
	return 0;	

}
int main(int argc,char **argv){


	if(argc<3)
		DEATH("usage:./multiprocesscopy srcfilename desfilename");

	int srcfd,desfd;
	int ret,i,thread_num=DEAULT_THREAD_NUM;
	char *srcaddr,*desaddr;
	off_t size,_size;
	int parts_num;	
	off_t surplus_part_size;//extra byte num that unexcess PART_SIZE	
	if(argv[3]&&(i=atoi(argv[3]))>0)
		thread_num=i;

	srcfd=open(argv[1],O_RDWR);
	desfd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
	if(srcfd<0)
		DEAD("open err");

	size=lseek(srcfd,0,SEEK_END);
	if(size<0)
		DEAD("end lseek err");
	_size=lseek(desfd,size-1,SEEK_SET);
	if(_size!=size-1)
		DEAD("desfd end lseek err");
	ret=write(desfd,"0",1);
	if(ret<0)
		DEAD("desfd write err");
	ret=lseek(srcfd,0,SEEK_SET);
	if(ret<0)
		DEAD("set lseek err");
	ret=lseek(desfd,0,SEEK_SET);
	if(ret<0)
		DEAD("set desfd lseek err");
	if(size>LIMIT_SIZE){// file divided to multiparts for mapping to memory ensure every part unexcess PART_SIZE(6M)
		parts_num=size/PART_SIZE;
		surplus_part_size=size%PART_SIZE;
		for(i=0;i<=parts_num;i++){
			if(i<parts_num){
				srcaddr=mmap(NULL,PART_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,srcfd,PART_SIZE*i);
				desaddr=mmap(NULL,PART_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,desfd,PART_SIZE*i);
				ret=copy(srcaddr,PART_SIZE,desaddr,thread_num);
				if(ret<0)
					DEATH("copy failed\n");
				munmap(srcaddr,PART_SIZE);
				munmap(desaddr,PART_SIZE);
			}
			else{
				srcaddr=mmap(NULL,surplus_part_size,PROT_READ|PROT_WRITE,MAP_SHARED,srcfd,PART_SIZE*i);
				desaddr=mmap(NULL,surplus_part_size,PROT_READ|PROT_WRITE,MAP_SHARED,desfd,PART_SIZE*i);
				ret=copy(srcaddr,surplus_part_size,desaddr,thread_num);
				if(ret<0)
					DEATH("copy failed\n");
				munmap(srcaddr,surplus_part_size);
				munmap(desaddr,surplus_part_size);
			}

		}	

	}else{
		srcaddr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,srcfd,0);
		desaddr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,desfd,0);
		ret=copy(srcaddr,size,desaddr,thread_num);
		if(ret<0)
			DEATH("copy failed\n");
		munmap(srcaddr,size);
		munmap(desaddr,size);


	}
	pthread_exit(0);	


}







