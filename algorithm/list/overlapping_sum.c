#include <stdio.h>

double fn( double x, int n );

int main()
{
	double x;
	int n;

	scanf("%lf %d", &x, &n);
	printf("%.2f\n", fn(x,n));

	return 0;
}
double fn( double x, int n ){
	double sum=0;
	if(n){
		sum=1;
		int ntmp=n;
		while(--ntmp){
			sum*=(-1*x);
		}
		sum*=x;
		sum+=fn(x,n-1);
	}
	return sum;
}

