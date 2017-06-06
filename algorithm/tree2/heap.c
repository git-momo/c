#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#define MAX_PRI 1000
#define MIN_PRI -1
#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return -1;}
typedef struct heap_node{

	char data;
	int priority;
}Node;
typedef struct heap{

	Node *array;
	int size;
	int capacity;
	int flag;//0 express this is an maximun heap,1 express this is an minimum heap;

}Heap;
typedef Heap *HEAP;


HEAP create_heap(int capacity,int type){
	HEAP heap;
	int parent,pi;
	int child,ci;
	char tmp;
	int pri_tmp;
	heap=calloc(sizeof(Heap),1);
	if(!heap)
		DEAD("malloc err");
	heap->capacity=capacity;
	heap->array=calloc(sizeof(Node),capacity);
	heap->size=0;
	heap->flag=type;
	printf("input ur data\n");
	if(type)
		heap->array[heap->size].priority=MIN_PRI;
	else
		heap->array[heap->size].priority=MAX_PRI;


	while((tmp=getchar())==' '||tmp=='\n');
	while(tmp!=EOF&&(++heap->size)<capacity){

		heap->array[heap->size].data=tmp;
		scanf("%d",&(heap->array[heap->size].priority));
		while((tmp=getchar())==' '||tmp=='\n');

	}
	if(type){//minimum heap

		for(parent=heap->size/2,child=parent*2;parent>0;parent--,child=parent*2){
			if(child<heap->size&&(heap->array[child+1].priority<heap->array[child].priority))
				child++;
			if((heap->array[child].priority)<(heap->array[parent].priority)){
				pri_tmp=heap->array[parent].priority;
				tmp=heap->array[parent].data;
				heap->array[parent].data=heap->array[child].data;
				heap->array[parent].priority=heap->array[child].priority;
				heap->array[child].data=tmp;
				heap->array[child].priority=pri_tmp;
				for(pi=child,ci=pi*2;ci<=heap->size;pi=ci,ci=pi*2){
					if(ci<heap->size&&(heap->array[ci+1].priority)<(heap->array[ci].priority))
						ci++;
					if((heap->array[ci].priority)<(heap->array[pi].priority)){
						pri_tmp=heap->array[pi].priority;
						tmp=heap->array[pi].data;
						heap->array[pi].data=heap->array[ci].data;
						heap->array[pi].priority=heap->array[ci].priority;
						heap->array[ci].data=tmp;
						heap->array[ci].priority=pri_tmp;

					}
				}		
			}

		}

	}else{//maximum heap

		for(parent=heap->size/2,child=parent*2;parent>0;parent--,child=parent*2){
			if(child<heap->size&&(heap->array[child+1].priority>heap->array[child].priority))
				child++;
			if((heap->array[child].priority)>(heap->array[parent].priority)){
				pri_tmp=heap->array[parent].priority;
				tmp=heap->array[parent].data;
				heap->array[parent].data=heap->array[child].data;
				heap->array[parent].priority=heap->array[child].priority;
				heap->array[child].data=tmp;
				heap->array[child].priority=pri_tmp;
				for(pi=child,ci=pi*2;ci<=heap->size;pi=ci,ci=pi*2){
					if(ci<heap->size&&(heap->array[ci+1].priority)>(heap->array[ci].priority))
						ci++;
					if((heap->array[ci].priority)>(heap->array[pi].priority)){
						pri_tmp=heap->array[pi].priority;
						tmp=heap->array[pi].data;
						heap->array[pi].data=heap->array[ci].data;
						heap->array[pi].priority=heap->array[ci].priority;
						heap->array[ci].data=tmp;
						heap->array[ci].priority=pri_tmp;

					}
				}		
			}

		}


	}
	return heap;

}

void print_heap(HEAP heap){

	int i;
	for(i=1;i<=heap->size;i++){

		printf("data=%c,priority=%d\n",heap->array[i].data,heap->array[i].priority);

	}
}
int append_heap(Heap * heap){
	char tmp,tmpi;
	int pri_tmp,child,parent;
	printf("please input ur data\n");
	while((tmp=getchar())==' '||tmp=='\n');
	while(tmp!=EOF){
		if(heap->size==heap->capacity-1)
			DEATH("this heap is full");
		heap->array[++heap->size].data=tmp;
		scanf("%d",&pri_tmp);
		child=heap->size;
		if(heap->flag){//minimum

			for(parent=heap->size/2;pri_tmp<(heap->array[parent].priority);child=parent,parent=child/2){//exchange
				tmpi=heap->array[parent].data;
				heap->array[parent].data=heap->array[child].data;
				heap->array[child].data=tmpi;
				heap->array[child].priority=heap->array[parent].priority;

			}		

			heap->array[child].priority=pri_tmp;

		}else{//maximum
			for(parent=heap->size/2;pri_tmp>(heap->array[parent].priority);child=parent,parent=child/2){//exchange
				tmpi=heap->array[parent].data;
				heap->array[parent].data=heap->array[child].data;
				heap->array[child].data=tmpi;
				heap->array[child].priority=heap->array[parent].priority;

			}		

			heap->array[child].priority=pri_tmp;


		}
		while((tmp=getchar())==' '||tmp=='\n');


	}


}

Node *delete_heap(HEAP heap){
	Node *n=NULL;
	int parent,child,pri_tmp;
	char tmp;
	if(heap->size){
		n=(heap->array)+1;
		parent=1;
		if(heap->size>1){

			heap->array[1].data=heap->array[heap->size].data;
			pri_tmp=heap->array[1].priority=heap->array[heap->size--].priority;
			for(child=2*parent;child<=heap->size;parent=child,child=parent*2){

				if(heap->flag){//minimum

					if(child<heap->size&&(heap->array[child+1].priority<heap->array[child].priority))
						child++;
					if(heap->array[child].priority<pri_tmp){
						tmp=heap->array[parent].data;
						heap->array[parent].data=heap->array[child].data;
						heap->array[child].data=tmp;
						heap->array[parent].priority=heap->array[child].priority;
					}else
						break;
				}else{//maximum
					if(child<heap->size&&(heap->array[child+1].priority>heap->array[child].priority))
						child++;
					if(heap->array[child].priority>pri_tmp){
						tmp=heap->array[parent].data;
						heap->array[parent].data=heap->array[child].data;
						heap->array[child].data=tmp;
						heap->array[parent].priority=heap->array[child].priority;
					}else
						break;

				}
			}
			heap->array[parent].priority=pri_tmp;
		}else
			heap->size--;
	}
	return n;
}

int main(){

	int type,ret;
	int capacity;
	HEAP heap;
	char flag;
	Node *n;
	while(1){
		printf("input which heap u want create:0 express maximum heap and 1 express minimum heap\n");
		scanf("%d",&type);
		if(type==1||type==0)
			break;
	}
	do{
		printf("please input the size of heap u want create\n");
		scanf("%d",&capacity);

	}while(capacity<=0);

	heap=create_heap(capacity,type);
	print_heap(heap);

	printf("please input the option u want to do to this heap:A express append new Node to this heap,D express delet limited value frome this heap,Q express quit\n");
	while((flag=getchar())==' '||flag=='\n');
	while(flag!='Q'){
		switch (flag){
			case 'A':
				append_heap(heap);
				break;
			case 'D':
				n=delete_heap(heap);
				if(n){
					if(heap->flag)
						printf("deleted min data=%c,priority=%d\n",n->data,n->priority);
					else
						printf("deleted max data=%c,priority=%d\n",n->data,n->priority);
					//free(n);
				}else
					printf("heap is null\n");
				break;
			default:
				return -1;
		};
		print_heap(heap);
		printf("please input the option u want to do to this heap:A express append new Node to this heap,D express delet limited value frome this heap,Q express quit\n");
		while((flag=getchar())==' '||flag=='\n');

	}

}
