#include <linux/module.h>
#include <linux/device.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include <linux/cdev.h>

#define DEV_MEM_SIZE 512

static char device_buffer[DEV_MEM_SIZE]="Welcome Sravani;

static dev_t devicenum;
static struct cdev my_cdev;
static struct class *char_class;
static struct device *char_device;
static  ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Read requested for %zu bytes \n",count);
	pr_info("Current file position = %lld\n",*f_pos);

	
	/* Adjust the 'count' */
	if((*f_pos + count) > DEV_MEM_SIZE)
		count = DEV_MEM_SIZE - *f_pos;

	/*copy to user */
	if(copy_to_user(buff,&device_buffer[*f_pos],count)){
		return -EFAULT;
	}

	/*update the current file postion */
	*f_pos += count;

	pr_info("Number of bytes successfully read = %zu\n",count);
	pr_info("Updated file position = %lld\n",*f_pos);

	/*Return number of bytes which have been successfully read */
	return count;
}

static int my_open(struct inode *inode,struct file *file)
{
	printk(KERN_INFO"Driver opened\n");
	return 0;
}
static int my_release(struct inode *inode,struct file *file){
	printk(KERN_INFO "Driver Closed\n");
	return 0;
}
static struct file_operations fops={
	.owner = THIS_MODULE,
	.open = my_open,
	.release=my_release,
};
static int __init chardriver_init(void){
	int ret;

ret = alloc_chrdev_region(&devicenum, 0, 1, "mychardriver");
	if (ret < 0) {
	printk(KERN_ERR "alloc_chrdev_region failed\n");

    	return ret;
}
printk(KERN_INFO "Step 1: alloc_chrdev_region OK\n");

 

cdev_init(&my_cdev,&fops);
printk(KERN_INFO "Step 2: cdev_init OK\n");

ret = cdev_add(&my_cdev, devicenum, 1);
        if (ret < 0) {
    	unregister_chrdev_region(devicenum, 1);
    	return ret;
}
printk(KERN_INFO "Step 3: cdev_add OK\n");
char_class=class_create("char_class");
	if (IS_ERR(char_class))
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(devicenum, 1);
    return PTR_ERR(char_class);
}
printk(KERN_INFO "Step 4: class_create OK\n");

char_device=device_create(char_class,NULL,devicenum,NULL,"char_driver");
	if (IS_ERR(char_device))
{
    	printk(KERN_ERR "Device creation failed\n");

    	class_destroy(char_class);
    	cdev_del(&my_cdev);
    	unregister_chrdev_region(devicenum,1);

    	return PTR_ERR(char_device);
}
printk(KERN_INFO "Step 5: device_create OK\n");
 printk(KERN_INFO "module intilaisation completed\n");


	return 0;
}

static void __exit chardriver_exit(void)
{
    device_destroy(char_class, devicenum);

    class_destroy(char_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(devicenum, 1);

    printk(KERN_INFO "module exit  successfully\n");
}
module_init(chardriver_init);
module_exit(chardriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sravani");
MODULE_DESCRIPTION("Simple Character Driver");
