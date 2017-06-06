#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int cofficient;
	int exponent;
	struct node *next;
}unary;
typedef unary *polynomial;
polynomial insert_poly(polynomial poly,unary *n){
		
	unary *fre=NULL;
	unary *tmp=poly;
		if(!poly)
			poly=n;
		else{
			while(tmp&&n->exponent<tmp->exponent){
				fre=tmp;
				tmp=tmp->next;

			}
			if(tmp)
				if(n->exponent==tmp->exponent)
					tmp->cofficient+=n->cofficient,free(n);
				else {
					if(!fre)
						n->next=tmp,poly=n;
					else
						n->next=fre->next,fre->next=n;
				}
			else
				fre->next=n;
			
		}
		return poly;	

}

polynomial input_polynomial(polynomial poly,int num){
	unary *n;
//	unary *fre;
//	unary *tmp;

	while(num--){
//		fre=NULL;
//		tmp=poly;
		n=calloc(sizeof(unary),1);
		scanf("%d",&n->cofficient);
		scanf("%d",&n->exponent);
		n->next=NULL;
		/*if(!poly)
			poly=n;
		else{
			while(tmp&&n->exponent<tmp->exponent){
				fre=tmp;
				tmp=tmp->next;

			}
			if(tmp)
				if(n->exponent==tmp->exponent)
					tmp->cofficient+=n->cofficient;
				else {
					if(!fre)
						n->next=tmp,poly=n;
					else
						n->next=fre->next,fre->next=n;
				}
			else
				fre->next=n;
			
		}*/
		poly=insert_poly(poly,n);
	}
	return poly;

}
void print_poly(polynomial poly){

	while(poly){
		printf("cofficient:%d;exponent:%d\t",poly->cofficient,poly->exponent);
		poly=poly->next;
	}
	printf("\n");
}
polynomial add_poly(polynomial poly1,polynomial poly2){
	unary *next;
	unary *tmp2=poly2;
	while(tmp2){
		next=tmp2->next;
		poly1=insert_poly(poly1,tmp2);
		tmp2=next;	
	}
	return poly1;

}
polynomial multi_poly(polynomial poly1,polynomial poly2){
	//unary *next;
	unary *tmp2=poly2;
	unary *tmp1=poly1;
	polynomial new=NULL;
	unary *n;
	while(tmp2){
		//next=tmp->next;
		while(tmp1){
			n=calloc(sizeof(unary),1);
			n->cofficient=tmp1->cofficient*tmp2->cofficient;
			n->exponent=tmp1->exponent+tmp2->exponent;
			n->next=NULL;
			new=insert_poly(new,n);
			tmp1=tmp1->next;
		}
		tmp1=poly1;
		tmp2=tmp2->next;
	}
	return new;
}

int main(){

	int  num1;
	int num2;
	int i;
	polynomial polynomial1=NULL;
	polynomial polynomial2=NULL;
	printf("polynomial 1:");
	scanf("%d",&num1);
	polynomial1=input_polynomial(polynomial1,num1);
	//print_poly(polynomial1);
	//return 1;
	printf("polynomial 2:");
	scanf("%d",&num2);
	polynomial2=input_polynomial(polynomial2,num2);
	//print_poly(polynomial2);
	//polynomial1=add_poly(polynomial1,polynomial2);
	//print_poly(polynomial1);
	polynomial1=multi_poly(polynomial1,polynomial2);
	print_poly(polynomial1);

}
