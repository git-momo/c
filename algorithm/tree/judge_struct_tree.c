#include <stdio.h>
#include <stdlib.h>
#define DEATH(mess){printf("%s\n",mess);exit(-1);}

typedef struct node{

	char data;
	int left;
	int right;
}Node;
typedef Node *Tree;

judge(int t1,Tree tree1,int t2,Tree tree2){
	if(tree1[t1].data!=tree[t2].data)
		return 0;
	else
		
}

int main(){


	int size;
	int i;
	char tmp;
	char left;
	char right;
	Tree tree1;
	Tree tree2;
	int *check;
	int t1;
	int t2;

	printf("input first tree\n");
	scanf("%d",&size);
	tree1=calloc(sizeof(Node),size);
	check=calloc(sizeof(int),size);
	while((tmp=getchar())==' '||tmp=='\n');
	for(i=0;i<size;i++){
		scanf("%c %c %c\n",&tmp,&left,&right);
		tree1[i].data=tmp;
		if(left=='-')
			tree1[i].left=-1;
		else if(right=='-')
			tree1[i].right=-1;
		else{
			tree1[i].left=left-'0';
			check[tree1[i].left]=1;
			tree1[i].right=right-'0';
			chaeck[tree1[i].right]=1;
		}
	}
	
	scanf("%d",&size);
	if(size!=(i+1))
		DEATH("check ur data");
	while(i>=0)
		if(!check[i--])
			break;
	t1=i+1;
	memset(chack,0,size);

	tree2=calloc(sizeof(Node),size);
	while((tmp=getchar())==' '||tmp=='\n');
	for(i=0;i<size;i++){
		scanf("%c %c %c\n",&tmp,&left,&right);
		tree2[i].data=tmp;
		if(left=='-')
			tree2[i].left=-1;
		else if(right=='-')
			tree2[i].right=-1;
		else{
			tree2[i].left=left-'0';
			check[tree2[i].left]=1;
			tree2[i].right=right-'0';
			chaeck[tree2[i].right]=1;
		}
	}

	judge(t1,tree1,t2,tree2);

}
