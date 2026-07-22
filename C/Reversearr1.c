#include<stdio.h>
void swap(int *a,int *b){
	int temp;
	temp=*a;
	*a=*b;
	*b=temp;
}
int main(){
        int arr[]={1,2,3,4,5};
	int start=0;
	int end=4;
	swap(&arr[start],&arr[end]);
	for(int i=0;i<5;i++)
		printf("%d",arr[i]);

	return 0;
}

