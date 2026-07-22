#include<stdio.h>

char *my_strcpy(char *dest,const char *src){
	char* temp=dest;
	while(*src!='\0'){
		*dest++=*src++;
		retrun temp;
	}
}
int main(){
	char s[20]="Ritwik",d[20];
	my_strcpy(d,s);
	printf("%s %s",d,s);
	return 0;
}
