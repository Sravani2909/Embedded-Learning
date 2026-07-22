#include<stdio.h>
#include<string.h>

int main(){
	char s[]="madam";
	int i=0,flag=0;
	int len=strlen(s);

	for(i=0;i<len/2;i++)
	{
		if(s[i]!=s[len-i-1]){
                printf("not palidrome");
		}
	}
	printf("palidrome");


	return 0;
}

