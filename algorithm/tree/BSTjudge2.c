#include <stdio.h>
#include <stdlib.h>

int judge(int *a1,int *a2,int size){

	if(size){
		int i;
		int *aa1;
		int *aa2;
		int j=0;
		int j2=0;
		int k=0;
		int k2=0;
		int flag;
		if(*a1!=*a2)
			return 0;
		else{
			int tmp=*a1;
			aa1=calloc(sizeof(int),size);
			aa2=calloc(sizeof(int),size);
			for(i=0;i<size;i++){
				if(a1[i]>tmp){
					a1[j++]=a1[i];
				}else if(a1[i]<tmp){
					aa1[k++]=a1[i];
				}
				if(a2[i]>tmp){
					a2[j2++]=a2[i];
				}else if(a2[i]<tmp){
					aa2[k2++]=a2[i];
				}
			}
			if(j!=j2||k!=k2)
				return 0;
			flag= judge(a1,a2,j)&&judge(aa1,aa2,k);
			free(aa1);
			free(aa2);
			return flag;	
		}
	}
	else
		return 1;

}

int main (){
		
	int size;
	int num;
	int i=0;
	int *array;
	int **arrays;
	scanf("%d",&size);
	scanf("%d",&num);
	arrays=calloc(sizeof(int *),num);
	while(i<num){
		arrays[i++]=calloc(sizeof(int),size);

	}
	array=calloc(sizeof(int),size);
	i=0;
	while(i<size){
		scanf("%d",&array[i++]);
	}
	i=0;
	int j;
	int flag;
	while(i<num){
		for(j=0;j<size;j++){
			scanf("%d",&arrays[i][j]);

		}
		flag=judge(array,arrays[i],size);
		if(flag)
			printf("YES\n");
		else
			printf("NO\n");
		i++;
	}
	

}
