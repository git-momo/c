#include <stdio.h>
#include <stdlib.h>

#define DEATH(mess){printf("%s\n",mess);exit(1);}


typedef struct node{
	
	char data;
	struct node *left;
	struct node *right;

}Node;

typedef Node *Tree;



Tree creat_tree(char *fre,char *midd,int size){
	
	Node *n=NULL;
	int i=0;
	if(size>0){
	n=calloc(sizeof(Node),1);
	n->data=fre[0];
	while(i<size&&fre[0]!=midd[i++]);
	if(i==size&&fre[0]!=midd[i-1])
		DEATH("check ur data");
	n->left=creat_tree(fre+1,midd,i-1);
	n->right=creat_tree(fre+i,midd+i,size-i-1);

	}
	return n;
	
	
}

void print_tree(Tree tree){
	if(tree){
	print_tree(tree->left);
	print_tree(tree->right);
	printf("%c  ",tree->data);
	}
}

int main(){

	char tmp;
	int i=0;
	int num;//record node numbers
	char fre[32];
	char midd[32];
	Tree tree;
	while((tmp=getchar())==' ');
	while(tmp!='\n'&&i!=32){
		fre[i++]=tmp;
		while((tmp=getchar())==' ');	

	}
	if(i==32)
		DEATH("size too big");
	num=i;
	i=0;
	
	while((tmp=getchar())==' ');
	while(tmp!='\n'&&i!=32){
		midd[i++]=tmp;
		while((tmp=getchar())==' ');	

	}
	if(i==32)
		DEATH("size too big");
	if(i!=num)
		DEATH("check ur data");
	tree=creat_tree(fre,midd,num);
	print_tree(tree);
		


}
