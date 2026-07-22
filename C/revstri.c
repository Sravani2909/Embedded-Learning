#include<stdio.h>
#include<string.h>

int main(){
	char str[]="Ritwik";
	int i=0,j=strlen(str)-1;

	while(i<j){
		char temp=str[i];
		     str[i]=str[j];
		     str[j]=temp;
		     i++;
		     j--;
	}
	printf("%s",str);
}
