#include <stdio.h>
#include <stdlib.h>
#define DEATH(mess){printf("%s\n",mess);exit(-1);}
typedef struct tree_node{
	
	char data;
	struct tree_node *left;
	struct tree_node *right;
}Node;
typedef Node * Tree;



Tree creat_tree(char *fre,char *midd,int size){
	Node * n=NULL;
	if(size>0){
		int i=0;
		n=calloc(sizeof(Node),1);
		n->data=fre[0];//record the root
		while(i<size&&(n->data!=midd[i]))i++;
		if(i==size)
			DEATH("check ur data");
		n->left=creat_tree(fre+1,midd,i);
		n->right=creat_tree(fre+1+i,midd+i+1,size-1-i);
	}
	return n;
	

}

void print_tree(Tree tree){
	
	if(tree){
		print_tree(tree->left);
		print_tree(tree->right);
		printf("%c ",tree->data);
	}

}
void print_tree_fre(Tree tree,int size){

	//create stack to save Node *
	Node **stack=calloc(sizeof(Node *),size);
	int i=-1;//record the top of stack

	do{
		printf("%c ",tree->data);
		stack[++i]=tree;//push
		if(tree->left)
			tree=tree->left;
		else{//check right 
			if(tree->right)
				tree=tree->right;
			else{
				while(i!=-1&&!stack[i]->right)i--;
				if(i==-1)//stack is empty
					return;
				else
					tree=stack[i--]->right;	
				
					
			}
				
		}
	}while(1);
		
}
int main(){

	int num,i;
	char fre[1024]={0};
	char midd[1024]={0};
	char tmp;
	Tree tree;
	


	//create a tree by fre sort and midd sort
	printf("please input the fre sort of tree\n");
	i=0;
	while((tmp=getchar())==' ');
	while(tmp!='\n')
		{fre[i++]=tmp;while((tmp=getchar())==' ');}//i record the array size;

	num=i;
	printf("please input the middle sort of tree\n");
	i=0;
	while((tmp=getchar())==' ');
	while(tmp!='\n')
		{midd[i++]=tmp;while((tmp=getchar())==' ');}//i record the array size;

	if(i!=num)
		DEATH("check ur data");

	tree=creat_tree(fre,midd,num);
	//print_tree(tree);

	print_tree_fre(tree,num);

}
