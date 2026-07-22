#include<stdio.h>

void *my_memcpy(void *dest,const void *src,size_t size)
{
	char *d=(char *)dest;
	const char *s=(const char *)src;

	while(size--)
	{
		*d=*s;
		d++;
		s++;
	}
	return dest;
}
int main(){
	char src[]="Embedded";
	char dest[20];
	my_memcpy(dest,src,9);
	printf("%s %s\n",src,dest);
	return 0;
}


