#include <stdio.h>
#include <stdlib.h>

#define DEATH(mess){printf("%s\n",mess);exit(-1);}
typedef struct heap_node{
	
	char data;
	int priority;
	
}Node;
struct heap{

	Node *array;
	int size;//record current size
	int capacity;//record MAX size
};
typedef struct heap *Heap;


Heap creat_heap(int capacity){

	Heap heap;
	int i=0,parent;
	char tmp;
	int pri_tmp;
	heap=calloc(sizeof(struct heap),1);
	heap->capacity=capacity;
	heap->array=calloc(sizeof (Node),capacity);
	heap->array[i++].priority=1000;//define sentry
	while((tmp=getchar())==' '||tmp=='\n');
	do{
		printf("tmp=%c\n",tmp);
		heap->array[i].data=tmp;
		scanf("%d",&heap->array[i].priority);
		printf("priority=%d\n",heap->array[i].priority);
		while((tmp=getchar())==' '||tmp=='\n');
		
	}while(i++<=capacity&&tmp!=EOF);//u can use '\' to replace
	if(i>capacity)
		DEATH("check ur data");
	heap->size=--i;
	//ajustment
	for(parent=i/2;parent>0;parent--,i=(parent*2+1)){
		if(i>1&&i%2&&(heap->array[i].priority<heap->array[i-1].priority))
			i--;
		if(heap->array[parent].priority<heap->array[i].priority){
			pri_tmp=heap->array[i].priority;
			heap->array[i].priority=heap->array[parent].priority;
			heap->array[parent].priority=pri_tmp;	
		}
		
	}
	return heap;
	
	
	

}
void print_heap(Heap heap){
	
	int i;
	for (i=1;i<=heap->size;i++){

		printf("data=%c ",heap->array[i].data);
		printf("priority=%d\n",heap->array[i].priority);
		
	}

}
int main(){
	
	int max_size;//capacity
	//Node *array;
	Heap heap;
	printf("input ur heap`s capacity\n");
	scanf("%d",&max_size);
	printf("input ur data\n");
	heap=creat_heap(max_size);
	print_heap(heap);
	
	

}
