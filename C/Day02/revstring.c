#include<stdio.h>
#include<string.h>
char *mystring_rev(char *str){
      char temp;
      char *start,*end;
      start=str;
      end=str+strlen(str)-1;
      while(start<end){
	      temp=*start;
	      *start=*end;
	      *end=temp;
	      start++;
	      end--;
      }
      return str;
}
int main(){
	char s[20]="SRAVANI";
	printf("%s\t",mystring_rev(s));
	
	return 0;
}

