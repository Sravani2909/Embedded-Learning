#include<stdio.h>


int main(){
	int i[]={10,20,30};
	int *ptr=i;
        (*ptr)++;
	printf("%d %d",i[0],*ptr++);
	return 0;
}
