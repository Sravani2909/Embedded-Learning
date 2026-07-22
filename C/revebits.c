#include<stdio.h>

int main(){
unsigned int num= 5;/*0101*/
int i;
unsigned int rev=0;
for(i=0;i<4;i++)
{
	//rev<<=1;
	rev=(rev<<1)|(num &1);
	num>>=1;
}
printf("%u\n",rev);

unsigned int rev1=0,num1=13;
for(i=0;i<32;i++)
{
        rev1<<=1;
        rev1 |=(num1 &1);
        num1>>=1;
}
printf("%u\n",rev1);







return 0;
}
