#include<stdio.h>
void *my_memove(void *dest,const void *src,int n){
	char *d=(char *)dest;
	const char *s=(const char *)src;

	if(d<s){
		while(n--){
			*d++=*s++;
		}
	}
	else
	{
		d=d+n;
		s=s+n;
		 while(n--)
		 {
			 *(--d)=*(--s);
		 }
	}
	return dest;
}


int main(){
	char str[]="ab12345";
	my_memove(str+2,str,5);
	printf("%s\n",str);
	 return 0;
}
