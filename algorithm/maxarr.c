#include <stdio.h>
#include <stdlib.h>


int main(){

	int num;
	scanf("%d",&num);
	int sum=0;
	int i;
	int tmp;
	int max=0;
	while(num--){
		scanf("%d",&i);
		tmp=sum+i;
		if(tmp>max)
			max=tmp,sum+=i;
		else if(tmp>0)
			sum+=i;
		else
			sum=0;	
	}
	printf("max=%d\n",max);
	return 1;
}
