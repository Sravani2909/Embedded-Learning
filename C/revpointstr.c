#include<stdio.h>
#include<string.h>

int main(){
	char str[]="sravani";
	char *start=str;
	char *end=str+strlen(str)-1;

	while(start < end){
                 char temp=*start;
		*start=*end;
		*end=temp;
		start++;
		end--;
	}
	 printf("%s\n",str);
	 return 0;
}

