#include <stdio.h>
#include <stdlib.h>

typedef struct tree_node{
	
	int data;
	struct tree_node *left;
	struct tree_node *right;

}Node;

typedef Node *Tree;

Tree insert_tree(Tree tree,int data){
	Node *n;
	if(!tree){
		n=calloc(sizeof(Node),1);
		n->data=data;
		n->left=NULL;
		n->right=NULL;
		tree=n;
	}else{

		if(data<tree->data)
			tree->left=insert_tree(tree->left,data);
		else
			tree->right=insert_tree(tree->right,data);
	}
	return tree;
	
}


Tree insert_tree2(Tree tree,int data){

	Node *n;
	Node *fre;
	Node *tmp;
	tmp=tree;
	int flag;
	n=calloc(sizeof(Node),1);
	n->data=data;
	n->left=NULL;
	n->right=NULL;
	if(!tree){
		tree=n;
	}else{
		while(tmp){
			if(data<tmp->data)
				fre=tmp,tmp=tmp->left,flag=0;//left
			else 
				fre=tmp,tmp=tmp->right,flag=1;//right
				

		}
		if(!flag)
			fre->left=n;
		else
			fre->right=n;
	}
	return tree;


	
}
int tree_min(Tree tree){

	while(tree->left)
		tree=tree->left;
	return tree->data;
}
int tree_max(Tree tree){
	while(tree->right)
		tree=tree->right;
	return tree->data;
}
Tree delete_tree2(Tree tree,int data){
	Node *tmp=tree;
	Node *fre=NULL;
	int flag=0;
	while(tmp){
	
		if(data<tmp->data)
			tmp=tmp->left,flag=0;
		else if(data>tmp->data)
			tmp=tmp->right,flag=1;
		else{
			if(tree->left&&tree->right){
				tmp->data=tree_min(tmp->right);
				tmp=tmp->right;
				data=tmp->data;
				flag=1;
			}else if(tree->left){
				tmp->data=tree_max(tmp->left);
				tmp=tmp->left;
				data=tmp->data;
				flag=0;
			}else if(tree->right){
				tmp->data=tree_min(tmp->right);
				tmp=tmp->right;
				data=tmp->data;
				flag=1;
			}else{
				if(fre){
					if(flag)
						fre->right=NULL;
					else
						fre->left=NULL;
				}	
				free(tmp);
			}
		}
		fre=tmp;
		
	}

}
Tree delete_tree(Tree tree,int data){
	if(tree){

		if(data<tree->data)
			tree->left=delete_tree(tree->left,data);
		else if(data>tree->data)
			tree->right=delete_tree(tree->right,data);
		else{
			if(tree->left&&tree->right){
				tree->data=tree_min(tree->right);
				tree->right=delete_tree(tree->right,tree->data);	
			}else if(tree->left){
				tree->data=tree_max(tree->left);
				tree->left=delete_tree(tree->left,tree->data);
			}else if(tree->right){
				tree->data=tree_min(tree->right);
				tree->right=delete_tree(tree->right,tree->data);	
			}else{
				free(tree);
				tree=NULL;
			}
		}
		return tree;
	}
	return NULL;

}
void print_tree_middle(Tree tree,int size){

	Node **stack;
	int i=-1;
	stack=calloc(sizeof(Node *),size);
	while(tree){
		while(tree->left){
			stack[++i]=tree;
			tree=tree->left;
		}
		do{
			printf("%d ",tree->data);
			if(tree->right){
				tree=tree->right;break;}
			else{
				if(i<0){
					tree=NULL;break;}
				else
					tree=stack[i--];
	
			}
		}
		while(1);
	}
		
	
	
}
void print_tree(Tree tree){

	if(tree){
		print_tree(tree->left);
		printf("%d ",tree->data);
		print_tree(tree->right);
	}
}




int main(){

	Tree tree=NULL;
	int num,data;
	scanf("%d",&num);
	int i;
	for(i=0;i<num;i++){
		scanf("%d",&data);
		tree=insert_tree2(tree,data);
		
	}
	//print_tree(tree);

	print_tree_middle(tree,num);
	scanf("%d",&i);
	delete_tree2(tree,i);	
	print_tree_middle(tree,num);


}
