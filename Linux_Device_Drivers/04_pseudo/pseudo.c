
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

#define DEV_MEM_SIZE 512

/*pseudo device memort*/
char device_buffer[DEV_MEM_SIZE];

/*it hold device number*/
dev_t device_number;

/*cdev varaible*/
struct cdev pcd_cdev;
struct class *class_pcd;

struct device *device_pcd;

static loff_t pcd_lseek(struct file *flip,loff_t off,int whence)
{       pr_info("lseek requested\n");
	return 0;
}
static ssize_t pcd_read(struct file *filp,char __user *buff,size_t count,loff_t *f_pos){
        pr_info("read requested for %zy bytes\n",count);
        pr_info("current file position=%lld\n",*f_pos);
	/*adjust the count */
	if((*f_pos + count) >DEV_MEM_SIZE)
		count =DEV_MEM_SIZE -*f_pos;
	/*copy to user */
	if(copy_to_user(buff,&device_buffer[*f_pos],count)){
		return -EFAULT;
        }
        /*update the current file position*/
		*f_pos +=count;
	pr_info("Number of bytes successfully read =%zu\n",count);
	pr_info("updated file position =%lld\n",*f_pos);
	/*retrun no of bytes which have been successfully read */
	return count;
}

static ssize_t pcd_write(struct file *filp,const char __user *buff,size_t count,loff_t *f_pos){
   pr_info("write  requested for %zu bytes\n",count);
        pr_info("current file position=%lld\n",*f_pos);
        /*adjust the count */
        if((*f_pos + count) >DEV_MEM_SIZE)
                count =DEV_MEM_SIZE -*f_pos;
	if(!count){
		pr_err("No space left on the device\n");
		return -ENOMEM;
	}
        /*copy from user */
        if(copy_from_user(&device_buffer[*f_pos],buff,count)){
                return -EFAULT;
        }
        /*update the current file position*/
                *f_pos +=count;
        pr_info("Number of bytes successfully write =%zu\n",count);
        pr_info("updated file position =%lld\n",*f_pos);
        /*retrun no of bytes which have been successfully written */
        return count;
  	
}

static int pcd_open(struct inode *inode,struct file *filp){
pr_info("open was successful\n");

	return 0;
}
static int pcd_release(struct inode *inode,struct file *filp){
	pr_info("close was successful\n");
	return 0;
}

/*file operations of the driver */
struct file_operations pcd_fops={
	.open=pcd_open,
	.write=pcd_write,
	.read=pcd_read,
	.llseek=pcd_lseek,
	.release=pcd_release,
	.owner=THIS_MODULE
};

static int __init pcd_driver_init(void){
	int ret;
        /*1.dynamically allocate a device number */
	ret =alloc_chrdev_region(&device_number,0,1,"pcd_devices");
         if(ret <0)
	        goto out;
               pr_info("Device number <Major>:<minor>=%d:%d\n",MAJOR(device_number),MINOR(device_number));

	/*2.Intialize the cdev structure with fops */
      
	cdev_init(&pcd_cdev,&pcd_fops);

	/*3.Register a device (cdev structure) with VFS*/
	pcd_cdev.owner=THIS_MODULE;

	ret=cdev_add(&pcd_cdev,device_number,1);
         if(ret <0)
		 goto unreg_chardev;
	/*4.create device class under /sys/class/ */
   
		class_pcd = class_create("pcd_class");
		if(IS_ERR(class_pcd)){
			pr_info("class creation failed\n");
			ret=PTR_ERR(class_pcd);
			goto cdev_del;
                }
       /*5.Populate the sysfs with device information*/
      device_pcd =device_create(class_pcd,NULL,device_number,NULL,"pcd");
      if(IS_ERR(device_pcd)){
	      pr_err("Device create failed\n");
	      ret=PTR_ERR(device_pcd);
                        goto class_del;
        }
      pr_info("Module init was successful\n");
return 0;
class_del:
	class_destroy(class_pcd);
cdev_del:
  	cdev_del(&pcd_cdev);

unreg_chardev:
     	unregister_chrdev_region(device_number,1);
out:
  return ret;

}
static void __exit pcd_driver_cleanup(void){
	device_destroy(class_pcd,device_number);
	class_destroy(class_pcd);
 	cdev_del(&pcd_cdev);
	unregister_chrdev_region(device_number,1);
	pr_info("module unloaded\n");
}
 

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sravani");
MODULE_DESCRIPTION("A pseudo character driver which handles n devices");
