#include<stdio.h>

int main(){
	int num=121,rev=0;
	int temp=num;

         while(temp){
		 rev=rev*10+temp%10;
		 temp=temp/10;
	 }
	 if(rev==num){
		 printf("pali\t");
	 }
	 else{
		 printf("not pali");
           }
	 return 0;
}

