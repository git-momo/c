#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>


int main(){

	char tmp;
	while((tmp=getchar())!=EOF)
		putchar(toupper(tmp));
	return 0;
}
