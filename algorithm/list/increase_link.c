#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct Node *PtrToNode;
struct Node {
	ElementType Data;
	PtrToNode   Next;
};
typedef PtrToNode List;

List Read(); /* 细节在此不表 */
void Print( List L ); /* 细节在此不表 */

List Insert( List L, ElementType X );

int main()
{
	List L;
	ElementType X;
	L = Read();
	scanf("%d", &X);
	L = Insert(L, X);
	Print(L);
	return 0;
}

/* 你的代码将被嵌在这里 */


List Read(){


	int num;
	int data;
	scanf("%d",&num);
	List l=NULL;
	while(num--){
		scanf("%d",&data);
		l=Insert(l,data);
	} 
	return l;
}

void Print( List L ){

	while(L){
		printf("%d\t",L->Data);
		L=L->Next;
	}
}

List Insert( List l, ElementType data ){
	List n=NULL;
	List tmp;
	List fre=NULL;
	tmp=l;
	n=calloc(sizeof(struct Node),0);
	n->Data=data;
	n->Next=NULL;

	if(!l)
		l=n;
	else{
		while(tmp&&(tmp->Data<n->Data)){
			fre=tmp;
			tmp=tmp->Next;
		}
		if(tmp==l){
			n->Next=tmp;
			l=n;
		}else{
			n->Next=fre->Next;
			fre->Next=n;
		}

	}

	return l;

}

