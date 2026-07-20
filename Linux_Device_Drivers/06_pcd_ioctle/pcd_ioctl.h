#ifndef PCD_IOCTL_H
#define PCD_IOCTL_H

#include<linux/ioctl.h>

#define MAGIC_NO 'P'

#define LED_ON   _IO(MAGIC_NO,1)
#define LED_OFF  _IO(MAGIC_NO,2)
#define LED_TOGGLE _IO(MAGIC_NO,3)
#define GET_LED_STATUS  _IOR(MAGIC_NO,4,int)
#endif
