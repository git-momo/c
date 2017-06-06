#include <stdio.h>
#include <stdlib.h>


#define STACK_SIZE 100

enum symbols{
	left_1='{',
	left_2='(',
	left_3='['
}

int main(){
	int i=0;
	char buf[1024]={0}; 
	read(STDIN_FILENO,buf,sizeof(buf));
	while(buf[i]!='\0'){
		
	}
}
