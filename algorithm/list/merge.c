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
void Print( List L ); /* 细节在此不表；空链表将输出NULL */

List Merge( List L1, List L2 );

int main()
{
    List L1, L2, L;
    L1 = Read();
    L2 = Read();
    Print(L1);
    Print(L2);
    L = Merge(L1, L2);
    Print(L);
    //Print(L1);
    //Print(L2);
    return 0;
}

/* 你的代码将被嵌在这里 */

List Read(){

	int num;//records num
	scanf("%d",&num);
	PtrToNode n=NULL;
	List l=NULL;
	PtrToNode tmp;
	PtrToNode fre;
	while(num--){
		tmp=l;
		fre=tmp;
		n=calloc(sizeof (struct Node),0);
		scanf("%d",&n->Data);
		n->Next=NULL;

		if(!l)
			l=n;
		else{
			while(tmp&&(tmp->Data<n->Data)){
				fre=tmp;
				tmp=tmp->Next;
			}
			if(tmp==l){
				n->Next=l;
				l=n;
			}else{

				n->Next=tmp;
				fre->Next=n;
			}
			
		}
	}

	return l;


}
List Merge( List l1, List l2 ){

	PtrToNode tmp2=l2;
	PtrToNode tmp=l1;
	PtrToNode fre=NULL;
	while(l2){
		while(tmp&&tmp2->Data>tmp->Data){
			fre=tmp;
			tmp=tmp->Next;
		}
		l2=l2->Next;
		if(!fre){
			tmp2->Next=l1;
			l1=tmp2;
		}else{
			fre->Next=tmp2;
			tmp2->Next=tmp;	
		}
		tmp=tmp2;
		fre=NULL;
		tmp2=l2;
	}

	return l1;

}

void Print( List L ){

	while(L){
		printf("%d\t",L->Data);
		L=L->Next;	
	}
	printf("\n");
}

