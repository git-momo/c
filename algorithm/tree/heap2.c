#include <stdio.h>
#include <stdlib.h>


typedef struct node {


	char data;
	int priority;
}Node;

typedef struct heap{
	
	Node *array;//record data
	int size;
	int capacity;
	
}Heap;


Heap *create_heap(int capacity){

	Heap *heap=calloc(sizeof(Heap),1);
	heap->capacity=capacity;
	heap->array=calloc(sizeof(Node),capacity);
	heap->size=0;
	heap->array[0].priority=1000;
	


	char tmp;
	while((tmp=getchar())==' '||tmp=='\n');
	while(tmp!=EOF&&heap->size<heap->capacity-1){
		heap->array[++heap->size].data=tmp;
		scanf("%d",&heap->array[heap->size].priority);
		while((tmp=getchar())==' '||tmp=='\n');
	}

	int position=heap->size/2;
	if(position){
		int parent;
		int child;
		int tmp;
		for(parent=1,child=parent*2;parent<=position;parent++,child=2*parent){

			if(child<heap->size&&(heap->array[child].priority)<(heap->array[child+1].priority))
				child++;
			if((heap->array[parent].priority)<(heap->array[child].priority)){
				tmp=heap->array[parent].priority;
				heap->array[parent].priority=heap->array[child].priority;
				heap->array[child].priority=tmp;		
			}
		}
	}
	return heap;
}

void print_heap(Heap *heap){
	int i;
	for(i=1;i<=heap->size;i++){
		printf("data=%c priority=%d\n",heap->array[i].data,heap->array[i].priority);
	}

}
int main(){

	int capacity;
	Heap *heap;
	printf("input ur heap`s capacity\n");
	scanf("%d",&capacity);
	
	heap=create_heap(capacity);

	print_heap(heap);
	
}
