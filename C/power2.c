#include<stdio.h>

int main(){
	int n=16;
	if(n&& !(n &(n-1)))/*n not equal to 0 and  power of 2 always has exactly one bit set*/
	  printf("power of 2");
	else
	  printf("not power of 2");
	  }
