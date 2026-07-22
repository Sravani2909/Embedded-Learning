#include<stdio.h>

int main(){
	int num,c=0,i;
	scanf("%d",&num);
	for(i=2;i<num;i++){
		if(num%i==0)
		{
			//c++;
			printf("Not prime");
		}
	}
	
		printf("prime");
	
	return 0;
}
