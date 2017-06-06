#include <stdio.h>
#include <stdlib.h>
#define MAX_PRI 1000
#define MIN_PRI -1
#define MINI_HEAP 0
#define MAXI_HEAP 1
typedef struct {

	char data;//if havent data,please assigned it '0'
	int priority;

}Node;

typedef struct {

	Node *array;//minimum heap ,record all leaf node
	int size;//record the end position of arrray
	int capacity;//data num
	Node *huff;//maximum heap,huffman tree;
	int huff_size;//record the end position of huff
}Huff;
void delete_heap(Node *array,int *size,Node *node,int flag){
	int child,parent;
	char tmp;
	int pri_tmp;
	if(node){

		node->data=array[1].data;
		node->priority=array[1].priority;
	}
	tmp=array[1].data=array[*size].data;
	pri_tmp=array[1].priority=array[(*size)--].priority;
	if(flag==MINI_HEAP){

		for(parent=1,child=parent*2;child<=*size;parent=child,child*=2){
			if(child<*size&&array[child+1].priority<array[child].priority)
				child++;
			if(array[child].priority<pri_tmp){
				array[parent].data=array[child].data;
				array[parent].priority=array[child].priority;
			}else
				break;
		}		
		array[parent].data=tmp;
		array[parent].priority=pri_tmp;
	}
	if(flag==MAXI_HEAP){
		for(parent=1,child=parent*2;child<=*size;parent=child,child*=2){
			if(child<*size&&array[child+1].priority>array[child].priority)
				child++;
			if(array[child].priority>pri_tmp){
				array[parent].data=array[child].data;
				array[parent].priority=array[child].priority;
			}else
				break;
		}		
		array[parent].data=tmp;
		array[parent].priority=pri_tmp;
	}

}
void delete_array(Huff *huff,Node *node){
	delete_heap(huff->array,&huff->size,node,MINI_HEAP);
}
int add_heap(Node *array,int *size,char data,int pri,int flag){

	int child,parent;
	array[++(*size)].data=data;
	array[*size].priority=pri;
	if(flag==MINI_HEAP){

		for(child=*size,parent=child/2;array[parent].priority>pri;child=parent,parent/=2){
			array[child].data=array[parent].data;
			array[child].priority=array[parent].priority;
		}
		array[child].data=data;
		array[child].priority=pri;

	}else if(flag==MAXI_HEAP){
		for(child=*size,parent=child/2;array[parent].priority<pri;child=parent,parent/=2){
			array[child].data=array[parent].data;
			array[child].priority=array[parent].priority;
		}
		array[child].data=data;
		array[child].priority=pri;


	}
	return child;
}
int add_huff(Huff *huff,const Node *node){//return value is new node`s position,u can ignored it if u dont care it
	return  add_heap(huff->huff,&huff->huff_size,node->data,node->priority,MAXI_HEAP);

}
int add_array(Huff *huff,const Node *node){//return value is new node`s position,u can ignored it if u dont care it
	return  add_heap(huff->array,&huff->size,node->data,node->priority,MINI_HEAP);

}
void print_heap(Node *array,int size){
	printf(".......\n");

	int i;
	for(i=1;i<=size;i++){

		printf("data=%c priority=%d\n",array[i].data,array[i].priority);
	}
	printf(".......\n");
}
void creat_huff(Huff *huff){
	printf("please input datas,usage:data priority\n");
	char tmp;
	int pri_tmp,child,parent,i;
	Node node_tmp,node_tmp2;
	//while((tmp=getchar())==' '||tmp=='\n');
	while(huff->size<huff->capacity-1){//read input and adjust array to be minimum heap
		while((tmp=getchar())==' '||tmp=='\n');
		huff->array[++huff->size].data=tmp;
		do{
			scanf("%d",&pri_tmp);
		}while(pri_tmp<=0);
		for(child=huff->size,parent=child/2;huff->array[parent].priority>pri_tmp;child=parent,parent/=2){
			tmp=huff->array[parent].data;
			huff->array[parent].data=huff->array[child].data;
			huff->array[child].data=tmp;
			huff->array[child].priority=huff->array[parent].priority;
		}
		huff->array[child].priority=pri_tmp;
	}

	//start code
	/*if(huff->size==1){

	  }*/
	while(huff->size>1){
		delete_array(huff,&node_tmp);
		if(node_tmp.data!='0')
			add_huff(huff,&node_tmp);
		//printf("data=%c priority=%d\n",node_tmp.data,node_tmp.priority);
		delete_array(huff,&node_tmp2);
		if(node_tmp2.data!='0')
			add_huff(huff,&node_tmp2);
		//printf("data2=%c priority2=%d\n",node_tmp2.data,node_tmp2.priority);
		node_tmp.data='0';
		node_tmp.priority=node_tmp.priority+node_tmp2.priority;
		i=add_huff(huff,&node_tmp);
		add_array(huff,&node_tmp);
		//printf("data3=%c priority3=%d\n",node_tmp.data,node_tmp.priority);
		//printf("huff_size=%d,size=%d\n",huff->huff_size,huff->size);
		//print_heap(huff->huff,huff->huff_size);

	}

}
void print_code(Node *array,int position,int size){
	if(position<=size){
		if(array[position].data=='0'){
			print_code(array,2*position,size);
			printf("0 ");
			print_code(array,2*position+1,size);
			printf("1 ");
			if(position==1)
				printf("\n");
		}else{
			printf("%c ",array[position].data);
		}

	}
}
void print_code2(Node *array,int size){
	
	int i;
	int j;
	for(i=1;i<=size;i++){

		if(array[i].data!='0'){
			printf("%c ",array[i].data);
			for(j=i;j>1;j/=2){
				if(j%2)
					printf("1");
				else
					printf("0");
			}
			printf("\n");		
		}
	}
}
int main(){

	Huff *huff;
	int i;	
	//create an
	huff=calloc(sizeof(Huff),1);
	do{ 
		printf("please input the size of data u want to be encoded\n");
		scanf("%d",&i);
	}while(i<2);
	huff->capacity=i+1;
	huff->array=calloc(sizeof(Node),huff->capacity);
	huff->huff=calloc(sizeof(Node),2*i);//2*i-1+1;
	huff->array[0].priority=MIN_PRI;
	huff->huff[0].priority=MAX_PRI;
	huff->size=0;
	huff->huff_size=0;
	//printf("before code\n");
	//print_heap(huff->array,huff->size);
	creat_huff(huff);
	printf("afte code\n");
	print_heap(huff->huff,huff->huff_size);
	//print_code(huff->huff,1,huff->huff_size);
	print_code2(huff->huff,huff->huff_size);
}
