#include<stdio.h>
#include<string.h>

int main(){
char s[20]="madam";
int len=strlen(s);
int i,flag=1;
for(i=0;i<len/2;i++)
{
	if(s[i]!=s[len-i-1]){
		flag=0;
		break;
	}
}
if(flag==0)
{
	printf("\n %s is palidrome string",s);
}
else{
	printf("%s is not pali",s);
}
return 0;
}


