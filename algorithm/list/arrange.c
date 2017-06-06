#include <stdio.h>
#include <stdlib.h>

void swap(int *array,int start,int end){
	int tmp;
	tmp=array[start];
	array[start]=array[end];
	array[end]=tmp;
}
void print_array(int *array,int num){
	int i;
	for(i=0;i<num;i++){
		printf("%d\t",array[i]);
	}
	printf("\n");
}

void arrange(int *array,int start,int num){
	int i;
	if(start==num-1)
		print_array(array,num);
	else
	for(i=start;i<num;i++){
		swap(array,start,i);
		arrange(array,start+1,num);
		swap(array,start,i);
		
	}

}

int main(){

	int num;
	scanf("%d",&num);
	int *array;
	int i;
	array=calloc(sizeof(int),num);

	for(i=0;i<num;i++){


		scanf("%d",&array[i]);
	}

	arrange(array,0,num);
	

}
