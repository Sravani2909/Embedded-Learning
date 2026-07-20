#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "pcd_ioctl.h"

int main()
{
    int fd;
    int status;

    fd = open("/dev/pcd_ioctl", O_RDWR);

    if(fd <0){
	    perror("Open");
	    return -1;
    }
    ioctl(fd, LED_ON);
    sleep(1);

   // ioctl(fd, LED_TOGGLE);
   // sleep(1);

    ioctl(fd, GET_LED_STATUS, &status);
    //sleep(1);

    printf("LED Status = %d\n", status);

    //ioctl(fd, LED_OFF);
    //sleep(1);

    close(fd);

    return 0;
}
