#include<stdio.h>

int main(){
	int number=10,temp,count=0;
	temp=number;
/*Method 1 Remove the lst set bit*/
	 while(temp){
                temp=(temp)&(temp-1);//extract the right most bit
                count++;
        }
       printf("%d\t",count);
/*Method2 check the last bit is it 1 or not then remove the last bit and repeat*/
       int count1=0,temp1;
       temp1=number;
       while(temp1){
       count1+=temp1&1;//check the last bit
       temp1>>=1;
       }
       printf("%d\t",count);
       return 0;
}
