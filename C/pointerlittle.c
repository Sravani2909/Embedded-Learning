#include<stdio.h>
union u{
	int x;
	char ch;
};
int main(){
	int t=10;
	char *cp=(char *)&t;
	if(*cp==10){
		printf("little\t");
	}
	else
	{
		printf("big");
	}
	union u u1;
	u1.x=10;
	if(u1.ch==10){
		printf("little");
	}
		else
		{
			printf("big");
		}

	return 0;
}
