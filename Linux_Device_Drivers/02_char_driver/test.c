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
	char buf[30];

	read(fd, buf, 20);

	printf("%s\n", buf);
	//printf("Device Closed successfully");
	close(fd);
	 printf("Device Closed successfully");

	return 0;
}

