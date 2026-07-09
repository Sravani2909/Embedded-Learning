#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include <linux/cdev.h>

static dev_t devicenum;
static struct cdev my_cdev;
static struct file_operations fops={
};
static int __init chardriver_init(void){
	alloc_chrdev_region(&devicenum,0,1,"mychardriver");
	printk(KERN_INFO "Major=%d Minor=%d\n",MAJOR(devicenum),MINOR(devicenum));

	cdev_init(&my_cdev,&fops);
	cdev_add(&my_cdev,devicenum,1);
	return 0;
}

static void __exit chardriver_exit(void){
	cdev_del(&my_cdev);
		unregister_chrdev_region(devicenum,1);
		printk(KERN_INFO "Driver Removed\n");
	}
module_init(chardriver_init);
module_exit(chardriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sravani");
MODULE_DESCRIPTION("Simple Character Driver");
