#include <stdio.h>
#include <stdlib.h>

typedef struct BST_Node{


	int data;
	struct BST_Node *left;
	struct BST_Node *right;
	int flag;//assigned 0 express havent accessed,1 express accessed;
}Node;

typedef Node *Tree;
Tree insert_tree(Tree tree,int data){
	if(!tree){
		Node *n;
		n=calloc(sizeof(Node),1);
		n->data=data;
		n->left=n->right=NULL;
		n->flag=0;
		return n;
	}	
	else{
		if(data>tree->data)
			tree->right=insert_tree(tree->right,data);
		else if(data<tree->data)
			tree->left=insert_tree(tree->left,data);
		return tree;
	}


}
int judge(Tree tree,int data){
	if(tree){
		if(tree->flag){
			if(tree->data==data)
				return 0;
			else if(tree->data>data)
				return judge(tree->left,data);
			else
				return judge(tree->right,data);
		}else{
			if(data==tree->data){
				tree->flag=1;return 1;
			}else
				return 0;

		}
	}
	else
		return 0;


}
void reset_flag(Tree tree){
	if(tree){
		tree->flag=0;
		reset_flag(tree->left);
		reset_flag(tree->right);
	}

}
int main(){



	int size;//record the tree size
	int num;//record the num of tree u need to judge;
	int data;
	int i=0;
	int flag=1;//0 expree no,1 express yes
	Tree tree=NULL;
	scanf("%d",&size);
	scanf("%d",&num);
	//read first line to create an tree as basic tree
	while(i++<size){
		scanf("%d",&data);
		tree=insert_tree(tree,data);

	}

	while(num--){
		for(i=0;i<size;i++){
			scanf("%d",&data);
			if(flag)
				flag=judge(tree,data);

		}
		if(flag)
			printf("YES\n");
		else
			printf("NO\n");
		flag=1;
		reset_flag(tree);		
	}


}
