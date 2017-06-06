#include <stdio.h>
#include <stdlib.h>


int main(){
	
	int num;
	scanf("%d",&num);
	int i;
	int *array;
	int start=0;
	int len=0;
	int tmplen=0;
	array=calloc(sizeof(int),num);
	for(i=0;i<num;i++){
		scanf("%d",&array[i]);
		if(i==0)
			continue;
		else{
			if(array[i]>array[i-1]){
				if(++tmplen>len)
					len=tmplen,start=i-len;
				
			}
			else{
				tmplen=0;
			}
					
		}
			
	
	}

	for(i=0;i<len+1;i++){

		printf("%d",array[start+i]);
	}
	printf("\n");

}
