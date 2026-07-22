#include<stdio.h>

int main(){
	int num=123,rev=0;

	while(num){
		rev=rev*10+num%10;
		num/=10;
	}
	printf("%d\t",rev);
	return 0;
}
