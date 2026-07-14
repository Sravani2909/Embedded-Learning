#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd;
	fd=open("/dev/char_driver",O_RDWR);
	if(fd<0){
		perror("open");
		return -1;

	}
	printf("Device opened successfully");
	//printf("Device Closed successfully");
	close(fd);
	 printf("Device Closed successfully");

	return 0;
}

