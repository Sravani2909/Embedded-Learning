
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

#define NO_OF_DEVICES 4
#define MEM_SIZE_MAX_PCDEV1 1024
#define MEM_SIZE_MAX_PCDEV2 512
#define MEM_SIZE_MAX_PCDEV3 1024
#define MEM_SIZE_MAX_PCDEV4 512


/*pseudo device memort*/
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];
char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];


/*device private data structure*/

struct pcdev_private_data
{
	char *buffer;
	unsigned size;
	const char *serial_number;
	int perm;
	struct cdev cdev;
};
/*driver private data structure*/
struct pcdrv_private_data
{
	int total_devices;
	dev_t device_number;
       struct class *class_pcd;

        struct device *device_pcd;

	struct pcdev_private_data pcdev_data[NO_OF_DEVICES];

};
struct pcdrv_private_data pcdrv_data=
{    .total_devices=NO_OF_DEVICES,
     .pcdev_data={
	     [0]={
		     .buffer=device_buffer_pcdev1,
		     .size=MEM_SIZE_MAX_PCDEV1,
		     .serial_number="PCDEV1XYZ123",
		     .perm=0x1/*RDONLY*/
	   },
	     [1]={
                     .buffer=device_buffer_pcdev2,
                     .size=MEM_SIZE_MAX_PCDEV2,
                     .serial_number="PCDEV2XYZ123",
                     .perm=0x10/*WRONLY*/
           },
           [2]={
                     .buffer=device_buffer_pcdev3,
                     .size=MEM_SIZE_MAX_PCDEV3,
                     .serial_number="PCDEV3 XYZ123",
                     .perm=0x11/*RDWR*/
           },
         [3]={
                     .buffer=device_buffer_pcdev4,
                     .size=MEM_SIZE_MAX_PCDEV4,
                     .serial_number="PCDEV4XYZ123",
                     .perm=0x11/*RDWR*/
           },

	    


     }
};     



static loff_t pcd_lseek(struct file *flip,loff_t off,int whence)
{       pr_info("lseek requested\n");
	return 0;
}
static ssize_t pcd_read(struct file *filp,char __user *buff,size_t count,loff_t *f_pos){
#if 0
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
#endif	/*retrun no of bytes which have been successfully read */
	return count;
}

static ssize_t pcd_write(struct file *filp,const char __user *buff,size_t count,loff_t *f_pos){
   
#if 0
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
#endif      /*retrun no of bytes which have been successfully written */
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

	int ret,i;
        /*1.dynamically allocate a device number */
	ret =alloc_chrdev_region(&pcdrv_data.device_number,0,NO_OF_DEVICES,"pdevs");
         if(ret <0){
		 pr_err("Alloc chrdev failed\n");
	        goto out;
	 }
	 /*.create device class under /sys/class/ */

                pcdrv_data.class_pcd = class_create("pcd_class");
                if(IS_ERR(pcdrv_data.class_pcd)){
                        pr_info("class creation failed\n");
                        ret=PTR_ERR(pcdrv_data.class_pcd);
                        goto unreg_chardev;
                }
		for(i=0;i<NO_OF_DEVICES;i++){

               pr_info("Device number <Major>:<minor>=%d:%d\n",MAJOR(pcdrv_data.device_number+i),MINOR(pcdrv_data.device_number+i));

	/*2.Intialize the cdev structure with fops */
      
	cdev_init(&pcdrv_data.pcdev_data[i].cdev,&pcd_fops);

	/*3.Register a device (cdev structure) with VFS*/

	pcdrv_data.pcdev_data[i].cdev.owner=THIS_MODULE;

	ret=cdev_add(&pcdrv_data.pcdev_data[i].cdev,pcdrv_data.device_number+i,1);
         if(ret <0){
		 pr_err("cdev add failed\n");
		 goto cdev_del;
	 }
       /*5.Populate the sysfs with device information*/
      pcdrv_data.device_pcd =device_create(pcdrv_data.class_pcd,NULL,pcdrv_data.device_number+i,NULL,"pcdev-%d",i+1);
      if(IS_ERR(pcdrv_data.device_pcd)){
	      pr_err("Device create failed\n");
	      ret=PTR_ERR(pcdrv_data.device_pcd);
                        goto class_del;
        }
}
      pr_info("Module init was successful\n");
return 0;
cdev_del:
class_del:
       for(;i>=0;i--){
	       device_destroy(pcdrv_data.class_pcd,pcdrv_data.device_number+i);
	       cdev_del(&pcdrv_data.pcdev_data[i].cdev);
       }
	class_destroy(pcdrv_data.class_pcd);

unreg_chardev:
     	unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);
out:
pr_info("Module insertion failed\n");
  return ret;

  

}
static void __exit pcd_driver_cleanup(void){
	int i;
for(i=0;i<NO_OF_DEVICES;i++){
	device_destroy(pcdrv_data.class_pcd,pcdrv_data.device_number+i);
	
 	cdev_del(&pcdrv_data.pcdev_data[i].cdev);
}
	  class_destroy(pcdrv_data.class_pcd);

	unregister_chrdev_region(&pcdrv_data.device_number,NO_OF_DEVICES);
	pr_info("module unloaded\n");

}
 

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sravani");
MODULE_DESCRIPTION("A pseudo character driver which handles n devices");
