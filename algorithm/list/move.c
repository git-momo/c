#include <stdio.h>
#include <stdlib.h>


void  reverse(int start,int end,int *array){

	int tmp;
	while(start<end)
		tmp=array[start],array[start++]=array[end],array[end--]=tmp;
	
}

int main(){

	
	int num;
	int _num;
	scanf("%d",&num);
	scanf("%d",&_num);
	int i=0;
	int *array=calloc(sizeof(int),num);
	while(i<num)
		scanf("%d",&array[i++]);


	_num%=num;
	//_num=num-_num;
	reverse(0,_num-1,array);
	for(i=0;i<num;i++)
		printf("%d",array[i]);
	printf("\n");	
	reverse(_num,num-1,array);
	for(i=0;i<num;i++)
		printf("%d",array[i]);
	printf("\n");	
	reverse(0,num-1,array);
	for(i=0;i<num;i++)
		printf("%d",array[i]);
	printf("\n");	
		

}
