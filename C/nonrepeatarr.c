#include<stdio.h>

int main(){
	int arr[]={10,2,3,3,2,5,5};
	int i;
	int result=0;
	    
	    int ele=sizeof(arr)/sizeof(arr[0]);
        
	for(i=0;i<ele;i++)
		result^=arr[i];
	
		printf("%d\t",result);
	return 0;
}

