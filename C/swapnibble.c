#include<stdio.h>

int main(){
	unsigned char x=0xAB;
        unsigned char y=0xCD;
     unsigned char result=(x<<4)|(x>>4);
     unsigned char ret=((x & 0x0F)<<4)|((x & 0xF0)>>4);
     printf("%0X\t",result);
     printf("%0X",ret);
     return 0;
}
