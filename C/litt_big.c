#include<stdio.h>

int main(){
unsigned int x=0x12345678;

	unsigned int y=(((x>>24)&(0x000000FF))|((x>>8)&(0x0000FF00))|((x<<8)&(0x00FF0000))|((x<<24)&(0xFF000000)));
	printf("0x%x\n",y);
	return 0;
}
