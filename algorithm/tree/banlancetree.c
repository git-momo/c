#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b)(a>b?a:b)
typedef struct tree_node{

	int data;
	struct tree_node *left;
	struct tree_node *right;
	int hight;
}Node;
typedef Node *Tree;
int hight(Tree tree){
	if(tree)
		return tree->hight;
	else
		return -1;

}
Tree doubleleft(Tree tree){
	Node *tmp;
	tmp=tree->left;
	tree->left=tmp->right;
	tmp->right=tree;
	tree->hight=MAX(hight(tree->left),hight(tree->right))+1;
	tmp->hight=MAX(hight(tmp->left),tree->hight)+1;
	return tmp;

}
Tree doubleright(Tree tree){
	Node *tmp;
	tmp=tree->right;
	tree->right=tmp->left;
	tmp->left=tree;
	tree->hight=MAX(hight(tree->left),hight(tree->right))+1;
	tmp->hight=MAX(hight(tmp->right),tree->hight)+1;
	return tmp;
}
Tree leftright(Tree tree){
	tree=doubleright(tree);
	return doubleleft(tree);
}
Tree rightleft(Tree tree){
	tree=doubleleft(tree);
	return doubleright(tree);

}
Tree insert_tree(Tree tree,int data){
	//Node *v=tree;
	//Node *tmp;
	if(!tree){
		Node *n;
		n=calloc(sizeof(Node),1);
		n->data=data;
		n->left=NULL;
		n->right=NULL;
		n->hight=0;

		return n;
	}
	else{

		if(data<tree->data){
			tree->left=insert_tree(tree->left,data);
			if(hight(tree->left)-hight(tree->right)==2){
				if(data<tree->left->data)//ll
					tree=doubleleft(tree);
				else//lr
					tree=leftright(tree);
			}
			
		}
		else{
			tree->right=insert_tree(tree->right,data);
			if(hight(tree->right)-hight(tree->left)==2){
				if(data>tree->right->data)//rr
					tree=doubleright(tree);
				else//rl
					tree=rightleft(tree);
			}

		}
		
		tree->hight=MAX(hight(tree->left),hight(tree->right))+1;
		return tree;
	}

}
void print(Tree tree){
	if(tree){
		print(tree->left);
		printf("%d ",tree->data);
		print(tree->right);
	}
}
int main(){

	int num;
	scanf("%d",&num);
	Tree tree=NULL;
	int data;
	while(num--){
		scanf("%d",&data);
		tree=insert_tree(tree,data);
	}
	print(tree);

}
