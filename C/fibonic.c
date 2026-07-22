#include<stdio.h>

int main(){
	int a=0,b=1,c,n=5,i;
	printf("%d\t %d\t",a,b);
	for(i=0;i<n;i++){
		c=a+b;

	  printf("%d\t",c);
	  a=b;
	  b=c;
	}
	return 0;
}
