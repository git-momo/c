#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess){printf("%s\n",mess);return -1;}
#define DEATH2(mess){printf("%s\n",mess);return NULL;}

#define WEIGHT_MASK 07000
#define WEIGHT 04000
#define UNWEIGHT 02000
#define GRAPHTYPE_MASK 00700
#define MATRIX 00400
#define LIST 00200
#define DIMENSION_MASK 00007//for matrix
#define ONE_DIMENSIONAL 00004//for matrix
#define TWO_DIMENSIONAL 00002//for matrix
#define DIRECTION_MASK 00070
#define DIRECTED 00040
#define UNDIRECTED 00020


#define DataType char
#define MAX_SIZE 100
#define WEIGHT_INITIALIZER 100


//matrix 

typedef struct {
	int v1,v2;
	int weight;	
}Edge;

typedef struct{
	int flag;//record the type of graph
	DataType data_array[MAX_SIZE];
	int **weights;
	int vn,en;//record vertex number and edge number		
}AdjMatrix;


//adjacency List


typedef struct node1{
	int position;
	int weights;
	struct node1 *next;
}EdgeWeight;
typedef struct node2{
	int position;
	struct node2 *next;
}EdgeUnweight;
typedef struct{
	DataType data;
	union{
		EdgeWeight *weights;
		EdgeUnweight *unweights;
	};
}Data;
typedef struct{
	int flag;//record the type of flag
	union{
		Data *UnData_Array;//undirected
		Data **DirData_Array;//directed
	};
	int vn,en;
}AdjList;

//Graph

typedef union Graph_union{
	AdjMatrix * matrix;
	//int test;
	AdjList * list;
}Graph;
void print_matrix (AdjMatrix *matrix){

	int i;
	int j;
	for(i=0;i<(matrix->vn);i++){
		if(i==0)
			printf("   %c",matrix->data_array[i]);
		else
			printf("  %c",matrix->data_array[i]);
	}
	printf("\n");
	if((matrix->flag&DIMENSION_MASK)==ONE_DIMENSIONAL){
		//		if((matrix->flag&DIRECTION_MASK)==DIRECTED)
		//			printf("OUT\n");
		for(i=0;i<(matrix->vn);i++){
			printf("%c",matrix->data_array[i]);
			for(j=0;j<i;j++){
				printf("  %d",matrix->weights[0][(i-1)*i/2+j]);
			}
			printf("\n");
		}

		if((matrix->flag&DIRECTION_MASK)==DIRECTED){//undirected
			//			printf("IN\n");
			for(i=0;i<(matrix->vn);i++){
				if(i==0)
					printf("   %c",matrix->data_array[i]);
				else
					printf("  %c",matrix->data_array[i]);
			}
			printf("\n");
			for(i=0;i<(matrix->vn);i++){
				printf("%c",matrix->data_array[i]);
				for(j=0;j<i;j++){
					printf("  %d",matrix->weights[1][(i-1)*i/2+j]);
				}
				printf("\n");
			}
		}

	}else{
		for(i=0;i<matrix->vn;i++){
			printf("%c",matrix->data_array[i]);
			for(j=0;j<matrix->vn;j++)
				printf("  %d",matrix->weights[i][j]);
			printf("\n");
		}
	}
	printf("edge number is %d\n",matrix->en);


}

//insert edge to matrix
int insert_edge4M(AdjMatrix *matrix,Edge *edge){
	int tmp;
	//judge paramete
	if(edge->v1==edge->v2)
		DEATH("v1 cant equals v2");
	if(edge->v1>=matrix->vn||edge->v2>=matrix->vn||edge->v1<0||edge->v2<0)
		DEATH("position exception");
	if((matrix->flag&DIMENSION_MASK)==ONE_DIMENSIONAL){
		if((matrix->flag&DIRECTION_MASK)==UNDIRECTED){//undirected

			//adjust v1 and v2
			if(edge->v1>edge->v2){
				tmp=edge->v1;
				edge->v1=edge->v2;
				edge->v2=tmp;
			}
			if((matrix->flag&WEIGHT_MASK)==UNWEIGHT&&!(matrix->weights[0][(edge->v2-1)*edge->v2/2+edge->v1]))
				matrix->en++;
			else if((matrix->flag&WEIGHT_MASK)==WEIGHT&&(matrix->weights[0][(edge->v2-1)*edge->v2/2+edge->v1])==100)
				matrix->en++;
			matrix->weights[0][(edge->v2-1)*edge->v2/2+edge->v1]=edge->weight;
		}else{//directed;
			if(edge->v1>edge->v2){//in
				if((matrix->flag&WEIGHT_MASK)==UNWEIGHT&&!(matrix->weights[1][(edge->v1-1)*edge->v1/2+edge->v2]))
					matrix->en++;
				else if((matrix->flag&WEIGHT_MASK)==WEIGHT&&(matrix->weights[1][(edge->v1-1)*edge->v1/2+edge->v2])==100)
					matrix->en++;
				matrix->weights[1][(edge->v1-1)*edge->v1/2+edge->v2]=edge->weight;

			}else{//out
				if((matrix->flag&WEIGHT_MASK)==UNWEIGHT&&!(matrix->weights[0][(edge->v2-1)*edge->v2/2+edge->v1]))
					matrix->en++;
				else if((matrix->flag&WEIGHT_MASK)==WEIGHT&&(matrix->weights[0][(edge->v2-1)*edge->v2/2+edge->v1])==100)
					matrix->en++;
				matrix->weights[0][(edge->v2-1)*edge->v2/2+edge->v1]=edge->weight;
			}
		}	
	}else{//two dimensional
		if((matrix->flag&WEIGHT_MASK)==UNWEIGHT&&!(matrix->weights[edge->v1][edge->v2]))
			matrix->en++;
		else if((matrix->flag&WEIGHT_MASK)==WEIGHT&&(matrix->weights[edge->v1][edge->v2])==100)
			matrix->en++;
		if((matrix->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
			matrix->weights[edge->v1][edge->v2]=edge->weight;
			matrix->weights[edge->v2][edge->v1]=edge->weight;

		}else//directed
			matrix->weights[edge->v1][edge->v2]=edge->weight;




	}
	return 0;

}
//create matrix by flag
AdjMatrix * create_matrix(int flag){
	AdjMatrix * matrix=NULL;
	Edge edge;
	char tmp;
	int i,j,ret;
	matrix=calloc(sizeof(AdjMatrix),1);
	if(!matrix)
		DEAD("malloc err");
	matrix->en=0;
	//flag
	matrix->flag=flag;
	//vn
	do{
		printf("please input the vertex number(>0) of this graph\n");
		scanf("%d",&i);
	}while(i<=0);
	matrix->vn=i;
	//data_array
	printf("please input datas of this graph\n");
	for(i=0;i<(matrix->vn);i++){
		while((tmp=getchar())==' '||(tmp=='\n'));	
		matrix->data_array[i]=tmp;
	}
	//weights
	//en
	//	do{
	//		printf("please input the edge nums (>=0) of this graph\n");
	//		scanf("%d",&i);
	//	}while(i<0);
	//	matrix->en=i;
	//assignment
	if((flag&DIMENSION_MASK)==ONE_DIMENSIONAL){//one dimensional
		if((flag&DIRECTION_MASK)==UNDIRECTED){//one one-dimensional array
			matrix->weights=calloc(sizeof(int *),1);
			if(!matrix->weights)
				DEAD("malloc err");
			matrix->weights[0]=calloc(sizeof(int),(matrix->vn-1)*(matrix->vn)/2);
			if(!matrix->weights[0])
				DEAD("malloc err");
			for(i=0;i<((matrix->vn-1)*matrix->vn/2);i++)
				if((flag&WEIGHT_MASK)==UNWEIGHT)
					matrix->weights[0][i]=0;
				else
					matrix->weights[0][i]=WEIGHT_INITIALIZER;


		}else{//two one-dimensional array,0 express out,1 express in
			matrix->weights=calloc(sizeof(int *),2);
			if(!matrix->weights)
				DEAD("malloc err");
			matrix->weights[0]=calloc(sizeof(int),(matrix->vn-1)*(matrix->vn)/2);//out
			matrix->weights[1]=calloc(sizeof(int),(matrix->vn-1)*(matrix->vn)/2);//in
			if(!matrix->weights[0]||!matrix->weights[1])
				DEAD("malloc err");
			for(i=0;i<((matrix->vn-1)*matrix->vn/2);i++)
				if((flag&WEIGHT_MASK)==UNWEIGHT)
					matrix->weights[0][i]=0,matrix->weights[1][i]=0;
				else
					matrix->weights[0][i]=WEIGHT_INITIALIZER,matrix->weights[1][i]=WEIGHT_INITIALIZER;
		}

	}else{//two dimensional
		matrix->weights=calloc(sizeof(int *),matrix->vn);
		if(!matrix->weights)
			DEAD("malloc err");
		for(i=0;i<matrix->vn;i++){
			matrix->weights[i]=calloc(sizeof(int),matrix->vn);
			if(!matrix->weights[i])
				DEAD("malloc err");
			for(j=0;j<matrix->vn;j++)
				if((flag&WEIGHT_MASK)==WEIGHT)
					matrix->weights[i][j]=WEIGHT_INITIALIZER;

		}
	}
	do{
		printf("please input how many times(>=0) that u want insert edge\n");
		scanf("%d",&i);
	}while(i<0);
	//insert edge
	while(i>0){
		printf("please input the position of source vertex \n");
		scanf("%d",&edge.v1);
		printf("please input the position of destinated vertex \n");
		scanf("%d",&edge.v2);
		if((flag&WEIGHT_MASK)==WEIGHT){
			do{
				printf("please input the weight(<100) of this edge\n");
				scanf("%d",&edge.weight);
			}while(edge.weight>=100);
		}
		else
			edge.weight=1;//unweight

		ret=insert_edge4M(matrix,&edge);
		if(ret<0)
			continue;
		i--;
	}
	print_matrix(matrix);
	return matrix;
}
int find_edge(Data *array,int v1,int v2){
	EdgeWeight *list=(EdgeWeight *)array[v1].weights;
	int i=0;
	while(list){
		if(list->position==v2)
			return i;
		list=list->next;
		i++;
	}
	return -1;

}

//insert edge to List
int insert_edge4L(AdjList *list,Edge *edge){
	if(edge->v1==edge->v2)
		DEATH("v1 cant equals v2");
	if(edge->v1>=list->vn||edge->v2>=list->vn||edge->v1<0||edge->v2<0)
		DEATH("position exception");
	int ret;
	EdgeWeight *tmp,*tmp2;
	EdgeUnweight *untmp,*untmp2;
	//en
	if((list->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
		if((ret=find_edge(list->UnData_Array,edge->v1,edge->v2))<0){//dont find
			list->en++;
			if((list->flag&WEIGHT_MASK)==WEIGHT){//weight
				tmp=calloc(sizeof(EdgeWeight),1);
				tmp2=calloc(sizeof(EdgeWeight),1);
				if(!tmp||!tmp2)
					DEATH("malloc err");
				tmp->position=edge->v2;
				tmp2->position=edge->v1;
				tmp->weights=tmp2->weights=edge->weight;
				tmp->next=list->UnData_Array[edge->v1].weights;
				list->UnData_Array[edge->v1].weights=tmp;
				tmp2->next=list->UnData_Array[edge->v2].weights;
				list->UnData_Array[edge->v2].weights=tmp2;
			}else{//unweight
				untmp=calloc(sizeof(EdgeUnweight),1);
				untmp2=calloc(sizeof(EdgeUnweight),1);
				if(!untmp||!untmp2)
					DEATH("malloc err");
				untmp->position=edge->v2;
				untmp2->position=edge->v1;
				untmp->next=list->UnData_Array[edge->v1].unweights;
				list->UnData_Array[edge->v1].unweights=untmp;
				untmp2->next=list->UnData_Array[edge->v2].unweights;
				list->UnData_Array[edge->v2].unweights=untmp2;

			}
		}else{//finded
			if((list->flag&WEIGHT_MASK)==WEIGHT){//weight
				list->UnData_Array[edge->v1].weights[ret].weights=edge->weight;
				ret=find_edge(list->UnData_Array,edge->v2,edge->v1);
				if(ret<0)
					DEATH("lost insert");
				list->UnData_Array[edge->v2].weights[ret].weights=edge->weight;

			}
			//unweight
		}
	}
	else{//directed
		if((ret=find_edge(list->DirData_Array[0],edge->v1,edge->v2))<0){//cant find
			list->en++;
			if((list->flag&WEIGHT_MASK)==WEIGHT){//weight
				tmp=calloc(sizeof(EdgeWeight),1);
				tmp2=calloc(sizeof(EdgeWeight),1);
				if(!tmp||!tmp2)
					DEATH("malloc err");
				tmp->position=edge->v2;
				tmp2->position=edge->v1;
				tmp->weights=tmp2->weights=edge->weight;
				tmp->next=list->DirData_Array[0][edge->v1].weights;
				list->DirData_Array[0][edge->v1].weights=tmp;
				tmp2->next=list->DirData_Array[1][edge->v2].weights;
				list->DirData_Array[1][edge->v2].weights=tmp2;

			}else{
				untmp=calloc(sizeof(EdgeUnweight),1);
				untmp2=calloc(sizeof(EdgeUnweight),1);
				if(!untmp||!untmp2)
					DEATH("malloc err");
				untmp->position=edge->v2;
				untmp2->position=edge->v1;
				untmp->next=list->DirData_Array[0][edge->v1].unweights;
				list->DirData_Array[0][edge->v1].unweights=untmp;
				untmp2->next=list->DirData_Array[1][edge->v2].unweights;
				list->DirData_Array[1][edge->v2].unweights=untmp2;

			}
		}else{//finded
			if((list->flag&WEIGHT_MASK)==WEIGHT){//weight
				list->DirData_Array[0][edge->v1].weights[ret].weights=edge->weight;
				ret=find_edge(list->DirData_Array[1],edge->v2,edge->v1);
				if(ret<0)
					DEATH("lost insert");
				list->DirData_Array[1][edge->v2].weights[ret].weights=edge->weight;

			}
			//unweight

		}
	}
	return 0;

}

void print_list(AdjList *list){
	int i,j;
	EdgeWeight *node;
	EdgeUnweight *unnode;
	if((list->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
		for(i=0;i<list->vn;i++){
			printf("%c",list->UnData_Array[i].data);
			if((list->flag&WEIGHT_MASK)==WEIGHT){//weight
				node=list->UnData_Array[i].weights;
				while(node){
					printf("-->%c %d",list->UnData_Array[node->position].data,node->weights);
					node=node->next;
				}
				printf("\n");
			}else{
				unnode=list->UnData_Array[i].unweights;
				while(unnode){
					printf("-->%c",list->UnData_Array[unnode->position].data);
					unnode=unnode->next;
				}
				printf("\n");

			}

		}
	}else{//directed
		for(j=0;j<2;j++){
			if(!j)
				printf("OUT\n");
			else
				printf("IN\n");
			for(i=0;i<list->vn;i++){
				printf("%c",list->DirData_Array[j][i].data);
				if((list->flag&WEIGHT_MASK)==WEIGHT){//weight
					node=list->DirData_Array[j][i].weights;
					while(node){
						printf("-->%c %d",list->DirData_Array[j][node->position].data,node->weights);
						node=node->next;
					}
					printf("\n");
				}else{
					unnode=list->DirData_Array[j][i].unweights;
					while(unnode){
						printf("-->%c",list->DirData_Array[j][unnode->position].data);
						unnode=unnode->next;
					}
					printf("\n");

				}
			}
		}
	}
	printf("edge number is %d\n",list->en);


}

//create list by flag

AdjList *create_list(int flag){

	AdjList *list;
	Edge edge;
	int i,ret;
	char tmp;
	list=calloc(sizeof(AdjList),1);
	if(!list)
		DEAD("malloc err");
	//flag
	list->flag=flag;
	//vn;
	do{
		printf("please input the vertex number(>0) of this graph\n");
		scanf("%d",&i);
	}while(i<=0);
	list->vn=i;
	//Data_Array
	if((flag&DIRECTION_MASK)==DIRECTED){//directed
		list->DirData_Array=calloc(sizeof(Data *),2);//0 express out ,1 express in
		if(!list->DirData_Array)
			DEAD("malloc err");
		list->DirData_Array[0]=calloc(sizeof(Data),list->vn);//out
		list->DirData_Array[1]=calloc(sizeof(Data),list->vn);//in
		if(!list->DirData_Array[0]||!list->DirData_Array[1])
			DEAD("malloc err");

	}else{//undirected
		list->UnData_Array=calloc(sizeof(Data),list->vn);
		if(!list->UnData_Array)
			DEAD("malloc err");
	}
	printf("please input datas of this graph\n");
	for(i=0;i<(list->vn);i++){
		while((tmp=getchar())==' '||(tmp=='\n'));
		if((flag&DIRECTION_MASK)==DIRECTED)//directed
			list->DirData_Array[0][i].data=list->DirData_Array[1][i].data=tmp;
		else//undirected
			list->UnData_Array[i].data=tmp;
	}
	//inser edge
	do{
		printf("please input how many times(>=0) that u want insert edge\n");
		scanf("%d",&i);
	}while(i<0);
	while(i>0){
		printf("please input the position of source vertex \n");
		scanf("%d",&edge.v1);
		printf("please input the position of destinated vertex \n");
		scanf("%d",&edge.v2);
		if((flag&WEIGHT_MASK)==WEIGHT){
			do{
				printf("please input the weight(<100) of this edge\n");
				scanf("%d",&edge.weight);
			}while(edge.weight>=100);
		}
		//		else
		//			edge.weight=1;//unweight

		ret=insert_edge4L(list,&edge);
		if(ret<0)
			continue;
		i--;
	}
	print_list(list);
	return list;

}

//DFS
void inner_DFS_matrix(AdjMatrix *matrix,int *visited,int position){
	//business logic
	printf("%c\t",matrix->data_array[position]);
	//assign visited
	visited[position]=1;

	int i,j,k;
	if((matrix->flag&DIMENSION_MASK)==ONE_DIMENSIONAL){//one dimensional

		i=position+1;
		if((matrix->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
			//visited all adjacency vertex
			for(j=(i-1)*i/2+position;i<matrix->vn;i++,j=(i-1)*i/2+position){
				if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[0][j]))||
						(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&(matrix->weights[0][j]!=WEIGHT_INITIALIZER))){
					//printf("A");
					if(!visited[i])
						inner_DFS_matrix(matrix,visited,i);
				}
			}			
		}else{//directed
			//visited all adjacency vertex
			//for(j=i,k=0;j<i+position;j++,k++){
			for(j=(i-1)*i/2+position;i<matrix->vn;i++,j=(i-1)*i/2+position){
				if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[0][j]||matrix->weights[1][j]))||
						(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&
						 (matrix->weights[0][j]!=WEIGHT_INITIALIZER||matrix->weights[1][j]!=WEIGHT_INITIALIZER))){
					//printf("A");
					if(!visited[i])
						inner_DFS_matrix(matrix,visited,i);
				}
			}			
		}
		}else{//two dimensional
			//visited all adjacency vertex
			for(i=position+1;i<matrix->vn;i++){
				if((matrix->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
					if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[position][i]))||
							(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&(matrix->weights[position][i]!=WEIGHT_INITIALIZER))){

						if(!visited[i])
							inner_DFS_matrix(matrix,visited,i);
					}
				}else{//directed
					if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[position][i]||matrix->weights[i][position]))||
							(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&(matrix->weights[position][i]!=WEIGHT_INITIALIZER||
								matrix->weights[i][position]!=WEIGHT_INITIALIZER))){
						if(!visited[i])
							inner_DFS_matrix(matrix,visited,i);
					}
				}
			}
		}

	}
}

void inner_DFS_list(AdjList *list,int * visited,int position){

	Data *array;//out
	Data *array2;//in
	EdgeWeight *_list,*_list2;
	//business logic
	if((list->flag&DIRECTION_MASK)==UNDIRECTED)
		printf("%c\t",list->UnData_Array[position].data);
	else
		printf("%c\t",list->DirData_Array[0][position].data);
	//assign visited
	visited[position]=1;
	if((list->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
		//assign array
		array=list->UnData_Array;
		//assign list
		_list=(EdgeWeight *)array[position].weights;
		//visited all adjacency vertex
		while(_list){
			if(!visited[_list->position])
				inner_DFS_list(list,visited,_list->position);
			_list=(EdgeWeight *)_list->next;
		}

	}else{//directed
		//assign array
		array=list->DirData_Array[0];//out
		array2=list->DirData_Array[1];//in
		//assign list
		_list=(EdgeWeight *)array[position].weights;
		_list2=(EdgeWeight *)array2[position].weights;
		while(_list||_list2){
			if(_list&&!visited[_list->position]){
				inner_DFS_list(list,visited,_list->position);
				//_list=(EdgeWeight *)_list->next;

			}
			if(_list2&&!visited[_list2->position]){
				inner_DFS_list(list,visited,_list2->position);
				//_list2=(EdgeWeight *)_list2->next;

			}
			if(_list)
				_list=_list->next;
			if(_list2)
				_list2=_list2->next;

		}


	}


}
void DFS(Graph * graph,int flag){
	printf("DFS\n");
	int i;
	AdjMatrix *matrix=graph->matrix;
	AdjList *list=graph->list;
	int *visited;//record the vertex whether be visited,0 express dont,1 express visited
	if((flag&GRAPHTYPE_MASK)==MATRIX){//matrix
		visited=calloc(sizeof(int),matrix->vn);
		if(!visited)
			DEAD("malloc err");
		for(i=0;i<matrix->vn;i++){
			if(!visited[i]){//dont be visited
				inner_DFS_matrix(matrix,visited,i);
				printf("\n");
			}

		}
	}else{//list
		visited=calloc(sizeof(int),list->vn);
		if(!visited)
			DEAD("malloc err");
		for(i=0;i<list->vn;i++){
			if(!visited[i]){//dont be visited
				inner_DFS_list(list,visited,i);
				printf("\n");
			}
		}
	}
	if(visited)
		free(visited);
}
//BFS
void inner_BFS_matrix(AdjMatrx *matrix,int *visited,int position){

	int *queue,start=-1,end=-1;
	queue=calloc(sizeof(int),matrix->vn);
	if(!queue)
		DEAD("malloc err");
	printf("%c\t",matrix->data_array[position]);
	visited[position]=1;
	//enqueue
	queue[++end]=position;
	int i,j;
	while(start!=end){//queue is not empty
		//dequeue
		position=queue[++start];
		if((matrix->flag&DIMENSION_MASK)==ONE_DIMENSIONAL){//one dimensional

			i=position+1;
			if((matrix->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
				//visited all adjacency vertex
				for(j=(i-1)*i/2+position;i<matrix->vn;i++,j=(i-1)*i/2+position){
					if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[0][j]))||
							(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&(matrix->weights[0][j]!=WEIGHT_INITIALIZER))){
						//printf("A");
						if(!visited[i]){
							printf("%c\t",matrix->data_array[i]);
							visited[i]=1;
							//enqueue
							queue[++end]=i;
						}
					}
				}			
			}else{//directed
				//visited all adjacency vertex
				//for(j=i,k=0;j<i+position;j++,k++){
				for(j=(i-1)*i/2+position;i<matrix->vn;i++,j=(i-1)*i/2+position){
					if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[0][j]||matrix->weights[1][j]))||
							(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&
							 (matrix->weights[0][j]!=WEIGHT_INITIALIZER||matrix->weights[1][j]!=WEIGHT_INITIALIZER))){
						//printf("A");
						if(!visited[i]){
							printf("%c\t",matrix->data_array[i]);
							visited[i]=1;
							//enqueue
							queue[++end]=i;
						}
					}
				}			
			}
			}else{//two dimensional
				//visited all adjacency vertex
				for(i=position+1;i<matrix->vn;i++){
					if((matrix->flag&DIRECTION_MASK)==UNDIRECTED){//undirected
						if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[position][i]))||
								(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&(matrix->weights[position][i]!=WEIGHT_INITIALIZER))){

							if(!visited[i]){
								printf("%c\t",matrix->data_array[i]);
								visited[i]=1;
								//enqueue
								queue[++end]=i;
							}
						}
					}else{//directed
						if((((matrix->flag&WEIGHT_MASK)==UNWEIGHT)&&(matrix->weights[position][i]||matrix->weights[i][position]))||
								(((matrix->flag&WEIGHT_MASK)==WEIGHT)&&(matrix->weights[position][i]!=WEIGHT_INITIALIZER||
									matrix->weights[i][position]!=WEIGHT_INITIALIZER))){
							if(!visited[i]){
								printf("%c\t",matrix->data_array[i]);
								visited[i]=1;
								//enqueue
								queue[++end]=i;
							}
						}
					}
				}
			}//
		}

	}
}
void BFS(Graph *graph,int flag){
	printf("BFS\n");
	AdjMatrix *matrix;
	AdjList *list;
	int i;
	int *visited;

	if((flag&GRAPHTYPE_MASK)==MATRIX){//matrix

		matrix=graph->matrix;
		visited=calloc(sizeof(int),matrix->vn);
		if(!visited)
			DEAD("malloc err");
		for(i=0;i<matrix->vn;i++){
			if(!visited[i]){
				inner_BFS_matrix(matix,visited,i);
				printf("\n");
			}	

		}
	}else{//list


	}
	if(visited)
		free(visited);

}

int main(int argc ,char **argv){

	if(argc<4)
		DEATH("usage:./graph graphtype (M:adjacency matrix,L:adjacency list) isDirectironal(0 express undirectional,1 express directional) isWight(0 express unweight,1 express weight) [dimensional]\n");

	Graph graph;
	//assignment flag;
	int flag;
	if(atoi(argv[2]))//directironal
		flag|=DIRECTED;
	else
		flag|=UNDIRECTED;
	if(atoi(argv[3]))//weight
		flag|=WEIGHT;
	else
		flag|=UNWEIGHT;
	if(strncmp(argv[1],"M",1)==0){//matrix
		flag|=MATRIX;
		//dimensional
		if(argv[4])//one dimensional
			flag|=ONE_DIMENSIONAL;
		else
			flag|=TWO_DIMENSIONAL;
		//create graph
		graph.matrix=create_matrix(flag);
	}else if(strncmp(argv[1],"L",1)==0){//list

		flag|=LIST;
		graph.list=create_list(flag);

	}else
		DEATH("usage:./graph graphtype(M:adjacency matrix,L:adjacency list)\n");

	DFS(&graph,flag);
	BFS(&graph,flag);	






}
