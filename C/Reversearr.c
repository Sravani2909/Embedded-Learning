#include<stdio.h>

int main(){
	int i,j, arr[]={1,2,3,4,5};
	int ele=sizeof(arr)/sizeof(arr[0]);
	/*for(i=0;i<ele;i++)
		printf("%d",arr[i]);
	printf("\n");
	for(j=ele-1;j>=0;j--)
		printf("%d",arr[j]);*/
        int temp;
	for(i=0;i<ele;i++){
		for(j=ele-1;j>=0;j--)
		{
			temp=arr[i];
			arr[i]=arr[j];
			arr[j]=temp;
		}
	}
	for(i=0;i<ele;i++)
		printf("%d",arr[i]);

	return 0;
}
