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


//adjacency List


typedef struct node1{
	int position;
	struct node1 *next;
	int weights;
}EdgeWeight;
typedef struct node2{
	int position;
	struct node2 *next;
}EdgeUnweight;
typedef struct{
	char data;
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
	AdjList * list
}Graph;
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

