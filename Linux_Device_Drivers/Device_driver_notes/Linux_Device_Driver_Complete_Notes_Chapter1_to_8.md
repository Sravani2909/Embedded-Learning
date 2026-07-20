# Linux Device Driver Complete Notes (Revision)

## Chapters Covered

-   Chapter 1: Linux Architecture
-   Chapter 2: User Space vs Kernel Space
-   Chapter 3: System Call Flow
-   Chapter 4: Character Device Driver
-   Chapter 5: Linux Kernel Module
-   Chapter 6: Building Kernel Modules
-   Chapter 7: Device Numbers (dev_t)
-   Chapter 8: struct cdev

------------------------------------------------------------------------

# Chapter 1: Linux Architecture

## User Space

-   Applications execute here.
-   No direct hardware access.
-   Uses system calls.

## Kernel Space

-   Full hardware access.
-   Contains scheduler, memory manager, file system, networking and
    device drivers.

------------------------------------------------------------------------

# Chapter 2: User Space vs Kernel Space

  User Space                  Kernel Space
  --------------------------- ------------------------
  Limited privileges          Full privileges
  No direct hardware access   Direct hardware access
  Uses system calls           Executes kernel code

------------------------------------------------------------------------

# Chapter 3: System Call Flow

Application ↓ System Call ↓ VFS ↓ Device Driver ↓ Hardware

Common system calls: - open() - read() - write() - ioctl() - close()

------------------------------------------------------------------------

# Chapter 4: Character Device Driver

Character devices transfer data one byte at a time.

Examples: - UART - Keyboard - Mouse - Serial Port

Applications use: - open() - read() - write() - close()

------------------------------------------------------------------------

# Chapter 5: Linux Kernel Module

Important APIs:

-   module_init()
-   module_exit()
-   printk()

Module Metadata:

-   MODULE_LICENSE("GPL")
-   MODULE_AUTHOR()
-   MODULE_DESCRIPTION()

Compile: make

Load: sudo insmod hello.ko

Unload: sudo rmmod hello

Logs: sudo dmesg \| tail

------------------------------------------------------------------------

# Makefile

obj-m += hello.o

KDIR := /lib/modules/\$(shell uname -r)/build PWD := \$(shell pwd)

all: `<TAB>`{=html}make -C $(KDIR) M=$(PWD) modules

clean: `<TAB>`{=html}make -C $(KDIR) M=$(PWD) clean

------------------------------------------------------------------------

Generated Files

-   hello.o
-   hello.mod.c
-   hello.mod.o
-   hello.ko
-   Module.symvers
-   modules.order

------------------------------------------------------------------------

# Common Errors

## Missing separator

Cause: - Spaces instead of TAB in Makefile.

Fix: - Use TAB.

## Key was rejected by service

Cause: - Secure Boot enabled.

Check: mokutil --sb-state

Fix: - Disable Secure Boot or sign the module.

------------------------------------------------------------------------

# Chapter 7: Device Numbers

Device number type:

dev_t

Kernel API:

alloc_chrdev_region()

Release:

unregister_chrdev_region()

Macros:

MAJOR() MINOR() MKDEV()

Major Number: Identifies the driver.

Minor Number: Identifies the device handled by that driver.

------------------------------------------------------------------------

# Chapter 8: struct cdev

Declaration:

static struct cdev my_cdev;

Purpose:

Connects: - Device Number - file_operations

cdev_init()

Initializes cdev and associates file_operations.

cdev_add()

Registers the character device with the kernel.

cdev_del()

Removes the character device during cleanup.

------------------------------------------------------------------------

# Driver Initialization Flow

module_init() ↓ alloc_chrdev_region() ↓ cdev_init() ↓ cdev_add() ↓
Driver Ready

Cleanup

cdev_del() ↓ unregister_chrdev_region()

------------------------------------------------------------------------

# Why alloc_chrdev_region() is not enough

It allocates only: - Major Number - Minor Number

It does NOT connect: - open() - read() - write() - release()

That connection is provided by:

struct cdev ↓ file_operations

------------------------------------------------------------------------

# Testing Commands

make

sudo insmod driver.ko

lsmod \| grep driver

sudo dmesg \| tail -20

modinfo driver.ko

sudo rmmod driver

sudo dmesg \| tail -20

------------------------------------------------------------------------

# Important Header Files

#include \<linux/module.h\> #include \<linux/kernel.h\> #include
\<linux/init.h\> #include \<linux/fs.h\> #include \<linux/cdev.h\>

------------------------------------------------------------------------

# Important APIs

module_init() module_exit()

alloc_chrdev_region() unregister_chrdev_region()

cdev_init() cdev_add() cdev_del()

MAJOR() MINOR() MKDEV()

------------------------------------------------------------------------

# Interview Questions

1.  What is a Kernel Module?
2.  What is a Character Device Driver?
3.  What is dev_t?
4.  Difference between Major and Minor Number.
5.  Why alloc_chrdev_region()?
6.  What is struct cdev?
7.  Why cdev_init()?
8.  Why cdev_add()?
9.  Why cdev_del()?
10. What is MODULE_LICENSE("GPL")?
11. Difference between printk() and printf()?
12. Difference between insmod and modprobe?

------------------------------------------------------------------------

# Next Chapter

-   struct file_operations
-   open()
-   release()
-   class_create()
-   device_create()
-   Creating /dev/char_driver
----------------------------------------------------------
------------------------------------------------------------------------

# Chapter 9: struct file_operations

## Purpose

`struct file_operations` connects the VFS to your driver functions.

Whenever an application performs an operation like:

- open()
- read()
- write()
- lseek()
- close()

the Linux VFS looks inside this structure and calls the corresponding driver function.

---

## Declaration

```c
static struct file_operations pcd_fops =
{
    .owner   = THIS_MODULE,
    .open    = pcd_open,
    .release = pcd_release,
    .read    = pcd_read,
    .write   = pcd_write,
    .llseek  = pcd_lseek,
};
```

---

## Members

### owner

```c
.owner = THIS_MODULE
```

Prevents the module from being unloaded while it is in use.

---

### open

```c
.open = pcd_open
```

Called when:

```c
open("/dev/pcd", O_RDWR);
```

---

### release

```c
.release = pcd_release
```

Called when:

```c
close(fd);
```

---

### read

```c
.read = pcd_read
```

Called when:

```c
read(fd, buffer, size);
```

Copies data from kernel space to user space.

Uses:

```c
copy_to_user()
```

---

### write

```c
.write = pcd_write
```

Called when:

```c
write(fd, buffer, size);
```

Copies data from user space to kernel space.

Uses:

```c
copy_from_user()
```

---

### llseek

```c
.llseek = pcd_lseek
```

Changes the current file position (`f_pos`).

Called when:

```c
lseek(fd, offset, whence);
```

------------------------------------------------------------------------

# VFS Flow

Application

↓

open("/dev/pcd")

↓

glibc

↓

System Call

↓

VFS

↓

Find inode

↓

Find Major Number

↓

Find cdev

↓

Find file_operations

↓

pcd_open()

------------------------------------------------------------------------

# Chapter 10 : open()

## Prototype

```c
static int pcd_open(struct inode *inode, struct file *filp)
```

Purpose:

Executed whenever the device file is opened.

Example:

```c
static int pcd_open(struct inode *inode, struct file *filp)
{
    pr_info("Device Opened\n");
    return 0;
}
```

Returns:

- 0 → Success
- Negative value → Failure

------------------------------------------------------------------------

# Chapter 11 : release()

## Prototype

```c
static int pcd_release(struct inode *inode, struct file *filp)
```

Purpose:

Executed when:

```c
close(fd);
```

Example:

```c
static int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("Device Closed\n");
    return 0;
}
```

------------------------------------------------------------------------

# Chapter 12 : class_create()

## Purpose

Creates a device class under:

```text
/sys/class/
```

Example:

```c
char_class = class_create("pcd_class");
```

Creates:

```text
/sys/class/pcd_class/
```

Without this API, `device_create()` cannot create a device node.

------------------------------------------------------------------------

# Chapter 13 : device_create()

## Purpose

Creates the device node.

Example:

```c
device_create(char_class,
              NULL,
              device_number,
              NULL,
              "pcd");
```

Creates:

```text
/ dev / pcd
```

Example output:

```text
crw------- 1 root root 234,0 /dev/pcd
```

------------------------------------------------------------------------

# Initialization Flow

module_init()

↓

alloc_chrdev_region()

↓

cdev_init()

↓

cdev_add()

↓

class_create()

↓

device_create()

↓

Device Ready

------------------------------------------------------------------------

# Cleanup Flow

device_destroy()

↓

class_destroy()

↓

cdev_del()

↓

unregister_chrdev_region()

------------------------------------------------------------------------

# Important Functions Learned

1. alloc_chrdev_region()
2. unregister_chrdev_region()
3. cdev_init()
4. cdev_add()
5. cdev_del()
6. class_create()
7. device_create()
8. device_destroy()
9. class_destroy()
10. open()
11. release()

------------------------------------------------------------------------

# Interview Questions

1. What is `struct file_operations`?
2. Why do we use `.owner = THIS_MODULE`?
3. Difference between `open()` and `release()`?
4. Why do we need `class_create()`?
5. Why do we need `device_create()`?
6. What happens if `device_create()` is omitted?
7. How does VFS call `pcd_open()`?
8. What is the role of `struct file`?
9. What is the role of `struct inode`?

------------------------------------------------------------------------

# Next Chapters

- read()
- write()
- llseek()
- ioctl()
---------------------------------------------======================
------------------------------------------------------------------------

# Chapter 14 : read()

## Purpose

The `read()` callback transfers data from **kernel space** to **user space**.

Application:

```c
read(fd, buffer, count);
```

Flow:

Application

↓

glibc

↓

System Call

↓

VFS

↓

pcd_read()

------------------------------------------------------------------------

## Prototype

```c
static ssize_t pcd_read(struct file *filp,
                        char __user *buff,
                        size_t count,
                        loff_t *f_pos)
```

------------------------------------------------------------------------

## Parameters

### struct file *filp

Represents the opened file.

Contains:

```c
filp->f_pos
```

Current file position.

------------------------------------------------------------------------

### char __user *buff

Pointer to the user-space buffer.

Data is copied into this buffer.

------------------------------------------------------------------------

### size_t count

Number of bytes requested by the application.

Example:

```c
read(fd, buffer, 20);
```

count = 20

------------------------------------------------------------------------

### loff_t *f_pos

Current file position.

After every successful read:

```c
*f_pos += count;
```

------------------------------------------------------------------------

## Complete Flow

Application

↓

read(fd,buffer,20)

↓

VFS

↓

pcd_read()

↓

Adjust count

↓

copy_to_user()

↓

Update f_pos

↓

Return bytes read

------------------------------------------------------------------------

## Step 1 : Adjust Count

```c
if((*f_pos + count) > DEV_MEM_SIZE)
    count = DEV_MEM_SIZE - *f_pos;
```

Purpose:

Prevent reading beyond device memory.

Example:

Device Size = 512

Current Position = 500

Requested = 20

Available = 12

Driver changes

```text
count = 12
```

------------------------------------------------------------------------

## Step 2 : Copy Data

```c
copy_to_user(buff,
             &device_buffer[*f_pos],
             count);
```

Meaning

Kernel

↓

device_buffer

↓

User Buffer

------------------------------------------------------------------------

If copy fails

```c
return -EFAULT;
```

------------------------------------------------------------------------

## Step 3 : Update Position

```c
*f_pos += count;
```

Example

Current Position = 100

Read = 20 bytes

New Position = 120

------------------------------------------------------------------------

## Step 4 : Return

```c
return count;
```

Returns number of bytes successfully read.

------------------------------------------------------------------------

## copy_to_user()

Prototype

```c
copy_to_user(destination,
             source,
             size);
```

destination

User Space

source

Kernel Space

------------------------------------------------------------------------

Example

```c
copy_to_user(buff,
             &device_buffer[*f_pos],
             count);
```

------------------------------------------------------------------------

## Return Value

Success

```text
Returns number of bytes read
```

Failure

```text
-EFAULT
```

------------------------------------------------------------------------

## Memory Flow

Kernel Memory

device_buffer

↓

copy_to_user()

↓

User Buffer

------------------------------------------------------------------------

## Interview Questions

1. Why do we use copy_to_user()?
2. Why can't memcpy() be used?
3. Why is count adjusted?
4. Why update f_pos?
5. What happens if read crosses device size?
6. Difference between read() and copy_to_user()?
----------------------------------------------------------------------
------------------------------------------------------------------------

# Chapter 15 : write()

## Purpose

The `write()` callback transfers data from **user space** to **kernel space**.

Application:

```c
write(fd, buffer, count);
```

Flow:

Application

↓

glibc

↓

System Call

↓

VFS

↓

pcd_write()

------------------------------------------------------------------------

## Prototype

```c
static ssize_t pcd_write(struct file *filp,
                         const char __user *buff,
                         size_t count,
                         loff_t *f_pos)
```

------------------------------------------------------------------------

## Parameters

### struct file *filp

Represents the opened file.

Contains:

```c
filp->f_pos
```

Current file position.

------------------------------------------------------------------------

### const char __user *buff

Pointer to the user-space buffer.

Data written by the application is stored here.

Example:

```c
write(fd, "Hello", 5);
```

------------------------------------------------------------------------

### size_t count

Number of bytes requested to write.

Example:

```c
write(fd, buffer, 20);
```

count = 20

------------------------------------------------------------------------

### loff_t *f_pos

Current file position.

After every successful write:

```c
*f_pos += count;
```

------------------------------------------------------------------------

## Complete Flow

Application

↓

write(fd,buffer,20)

↓

VFS

↓

pcd_write()

↓

Adjust count

↓

copy_from_user()

↓

Update f_pos

↓

Return bytes written

------------------------------------------------------------------------

## Step 1 : Adjust Count

```c
if((*f_pos + count) > DEV_MEM_SIZE)
    count = DEV_MEM_SIZE - *f_pos;
```

Purpose:

Prevent writing beyond device memory.

Example

Device Size = 512

Current Position = 500

Requested = 20

Available = 12

Driver changes

```text
count = 12
```

------------------------------------------------------------------------

## Step 2 : Check Available Space

```c
if(!count)
    return -ENOMEM;
```

Meaning:

No space left in the pseudo device.

Return

```text
-ENOMEM
```

Meaning:

Out of memory / No space available.

------------------------------------------------------------------------

## Step 3 : Copy Data

```c
copy_from_user(&device_buffer[*f_pos],
               buff,
               count);
```

Meaning

User Buffer

↓

copy_from_user()

↓

Kernel Memory (device_buffer)

------------------------------------------------------------------------

If copy fails

```c
return -EFAULT;
```

------------------------------------------------------------------------

## Step 4 : Update Position

```c
*f_pos += count;
```

Example

Current Position = 100

Written = 20 bytes

New Position = 120

------------------------------------------------------------------------

## Step 5 : Return

```c
return count;
```

Returns number of bytes successfully written.

------------------------------------------------------------------------

## copy_from_user()

Prototype

```c
copy_from_user(destination,
               source,
               size);
```

destination

Kernel Space

source

User Space

------------------------------------------------------------------------

Example

```c
copy_from_user(&device_buffer[*f_pos],
               buff,
               count);
```

------------------------------------------------------------------------

## Memory Flow

User Buffer

↓

copy_from_user()

↓

Kernel Memory

↓

device_buffer

------------------------------------------------------------------------

## Difference Between copy_to_user() and copy_from_user()

| copy_to_user() | copy_from_user() |
|----------------|------------------|
| Kernel → User | User → Kernel |
| Used in read() | Used in write() |

------------------------------------------------------------------------

## read() vs write()

| read() | write() |
|----------|-----------|
| Sends data to user | Receives data from user |
| Uses copy_to_user() | Uses copy_from_user() |
| Kernel → User | User → Kernel |
| Updates f_pos | Updates f_pos |

------------------------------------------------------------------------

## Complete Write Flow

Application

↓

write(fd,buffer,count)

↓

VFS

↓

pcd_write()

↓

Adjust count

↓

copy_from_user()

↓

Update f_pos

↓

Return bytes written

------------------------------------------------------------------------

## Important Error Codes

```text
-EFAULT
```

User buffer is invalid.

```text
-ENOMEM
```

No space left in the pseudo device.

------------------------------------------------------------------------

## Interview Questions

1. Why do we use copy_from_user()?
2. Why can't memcpy() be used?
3. Why is count adjusted before writing?
4. Why do we update f_pos?
5. Difference between read() and write()?
6. Difference between copy_to_user() and copy_from_user()?
7. Why do we return -ENOMEM?
8. What happens when write() reaches the end of device memory?
-----------------------------------------------------------
------------------------------------------------------------------------

# Chapter 16 : llseek()

## Purpose

The `llseek()` callback changes the current file position (`f_pos`).

Application:

```c
lseek(fd, offset, whence);
```

Flow:

Application

↓

glibc

↓

System Call

↓

VFS

↓

pcd_lseek()

------------------------------------------------------------------------

## Prototype

```c
static loff_t pcd_lseek(struct file *filp,
                        loff_t offset,
                        int whence)
```

------------------------------------------------------------------------

## Parameters

### struct file *filp

Represents the opened device.

Contains the current file position.

```c
filp->f_pos
```

------------------------------------------------------------------------

### loff_t offset

Number of bytes to move.

Example

```c
lseek(fd,100,SEEK_SET);
```

offset = 100

------------------------------------------------------------------------

### int whence

Specifies how the offset should be interpreted.

Possible values

- SEEK_SET
- SEEK_CUR
- SEEK_END

------------------------------------------------------------------------

# SEEK_SET

Moves relative to the beginning of the device.

Formula

```text
New Position = offset
```

Example

Current Position

```text
50
```

Application

```c
lseek(fd,20,SEEK_SET);
```

Result

```text
New Position = 20
```

Driver Code

```c
if((offset > DEV_MEM_SIZE) || (offset < 0))
    return -EINVAL;

filp->f_pos = offset;
```

------------------------------------------------------------------------

# SEEK_CUR

Moves relative to the current position.

Formula

```text
New Position = Current Position + offset
```

Example

Current Position

```text
300
```

Application

```c
lseek(fd,100,SEEK_CUR);
```

Calculation

```text
300 + 100 = 400
```

Result

```text
New Position = 400
```

Driver Code

```c
temp = filp->f_pos + offset;

if((temp > DEV_MEM_SIZE) || (temp < 0))
    return -EINVAL;

filp->f_pos = temp;
```

------------------------------------------------------------------------

# SEEK_END

Moves relative to the end of the device.

Formula

```text
New Position = DEV_MEM_SIZE + offset
```

Example

Device Size

```text
512 Bytes
```

Application

```c
lseek(fd,-20,SEEK_END);
```

Calculation

```text
512 + (-20)

= 492
```

Result

```text
New Position = 492
```

Driver Code

```c
temp = DEV_MEM_SIZE + offset;

if((temp > DEV_MEM_SIZE) || (temp < 0))
    return -EINVAL;

filp->f_pos = temp;
```

------------------------------------------------------------------------

# Validation

Always validate the calculated position before updating `f_pos`.

```c
if((temp > DEV_MEM_SIZE) || (temp < 0))
    return -EINVAL;
```

Reason

- Prevent negative file position.
- Prevent accessing memory beyond device size.

------------------------------------------------------------------------

# Return Value

```c
return filp->f_pos;
```

Returns the updated file position.

------------------------------------------------------------------------

# Complete Flow

Application

↓

lseek(fd,offset,whence)

↓

VFS

↓

pcd_lseek()

↓

Calculate New Position

↓

Validate Position

↓

Update f_pos

↓

Return New Position

------------------------------------------------------------------------

# Memory Example

Device Size = 512 Bytes

```text
0 ----------------------------------------------- 511
^                                                 ^
Beginning                                       End
```

### SEEK_SET

```c
lseek(fd,100,SEEK_SET);
```

```text
0 --------100-------------------------------511
          ^
        f_pos
```

### SEEK_CUR

Current Position = 200

```c
lseek(fd,20,SEEK_CUR);
```

```text
200 --------220-----------------------------511
             ^
           f_pos
```

### SEEK_END

```c
lseek(fd,-10,SEEK_END);
```

```text
0--------------------------------------502---511
                                       ^
                                     f_pos
```

------------------------------------------------------------------------

# Summary

| whence | Formula |
|---------|---------|
| SEEK_SET | f_pos = offset |
| SEEK_CUR | f_pos = current position + offset |
| SEEK_END | f_pos = DEV_MEM_SIZE + offset |

------------------------------------------------------------------------

# Important Error

Return

```text
-EINVAL
```

When

- Position < 0
- Position > DEV_MEM_SIZE
- Invalid whence value

------------------------------------------------------------------------

# Interview Questions

1. What is llseek()?
2. What is f_pos?
3. Difference between SEEK_SET, SEEK_CUR and SEEK_END?
4. Why do we validate the file position?
5. Why do we use a temporary variable (temp)?
6. Why do we return -EINVAL?
7. What is the return value of llseek()?

------------------------------------------------------------------------

# APIs Learned So Far

1. module_init()
2. module_exit()
3. alloc_chrdev_region()
4. unregister_chrdev_region()
5. cdev_init()
6. cdev_add()
7. cdev_del()
8. class_create()
9. device_create()
10. device_destroy()
11. class_destroy()
12. open()
13. release()
14. read()
15. write()
16. llseek()

------------------------------------------------------------------------

# Next Chapter

- ioctl() / unlocked_ioctl()
- Why ioctl() is needed
- _IO, _IOR, _IOW, _IOWR macros
- User Space ↔ Driver communication using ioctl()
---------------------------------------------------# Chapter 18: ioctl() Macros (_IO, _IOR, _IOW, _IOWR)

## 1. What is ioctl()?

`ioctl()` (Input/Output Control) is a system call used to perform device-specific operations that cannot be done using `read()` or `write()`.

Prototype:

```c
int ioctl(int fd, unsigned long cmd, ...);
fd : File descriptor returned by open()
cmd : Encoded ioctl command
arg : Optional argument
2. Why do we need ioctl macros?

Suppose we define commands like:

#define LED_ON     1
#define LED_OFF    2

Another driver may also define:

#define MOTOR_ON   1
#define MOTOR_OFF  2

Now command numbers conflict.

Linux solves this by encoding additional information into every ioctl command.

3. ioctl Macros

Linux provides four standard macros.

_IO()

Used when no data is transferred.

Syntax:

_IO(type, nr)

Example:

#define LED_ON _IO('P',1)

Meaning:

Type (Magic Number) = 'P'
Command Number = 1
Direction = None
Size = 0

Application:

ioctl(fd, LED_ON);
_IOR()

Used when the driver sends data to user space.

Syntax:

_IOR(type, nr, datatype)

Example:

#define GET_LED_STATUS _IOR('P',2,int)

Application:

int status;

ioctl(fd, GET_LED_STATUS, &status);

Driver:

copy_to_user((int __user *)arg,
             &led_status,
             sizeof(int));

Direction:

Kernel
   │
   ▼
User
_IOW()

Used when user space sends data to the driver.

Syntax:

_IOW(type, nr, datatype)

Example:

#define SET_BRIGHTNESS _IOW('P',3,int)

Application:

int brightness = 80;

ioctl(fd, SET_BRIGHTNESS, &brightness);

Driver:

copy_from_user(&kernel_brightness,
               (int __user *)arg,
               sizeof(int));

Direction:

User
  │
  ▼
Kernel
_IOWR()

Used when both user and kernel exchange data.

Syntax:

_IOWR(type, nr, datatype)

Example:

#define UPDATE_STATUS _IOWR('P',4,int)

Application:

int value = 10;

ioctl(fd, UPDATE_STATUS, &value);

Driver:

copy_from_user(...);

process_data();

copy_to_user(...);

Direction:

User
   │
   ▼
Kernel

Kernel
   │
   ▼
User
4. Meaning of Each Parameter

Example:

_IO('P',1)
Type

Also called the Magic Number.

'P'

Used to identify the driver.

Examples:

'P' -> Pseudo Driver
'U' -> UART Driver
'S' -> SPI Driver
Number (nr)

Identifies a particular command.

Example:

1 -> LED_ON
2 -> LED_OFF
3 -> GET_STATUS
5. Why same magic number?

All commands belonging to one driver should use the same magic number.

Example:

#define LED_ON          _IO('P',1)
#define LED_OFF         _IO('P',2)
#define GET_STATUS      _IOR('P',3,int)
#define SET_BRIGHTNESS  _IOW('P',4,int)

Only the command number changes.

6. Why different magic numbers?

Different drivers use different magic numbers.

Example:

#define LED_ON      _IO('P',1)

#define UART_RESET  _IO('U',1)

Although both commands use command number 1, they never conflict because their magic numbers are different.

7. Internal Expansion

Linux defines:

#define _IO(type,nr) \
        _IOC(_IOC_NONE,(type),(nr),0)

Example:

_IO('P',1)

Expands to:

_IOC(_IOC_NONE,'P',1,0)
8. _IOR Expansion
_IOR('P',2,int)

Expands to

_IOC(_IOC_READ,'P',2,sizeof(int))
9. _IOW Expansion
_IOW('P',3,int)

Expands to

_IOC(_IOC_WRITE,'P',3,sizeof(int))
10. _IOWR Expansion
_IOWR('P',4,int)

Expands to

_IOC(_IOC_READ | _IOC_WRITE,
     'P',
     4,
     sizeof(int))
11. Direction Meaning
_IO

No Data Transfer

Application
      │
      ▼
Driver performs action
_IOR

Kernel
   │
   ▼
User
_IOW

User
   │
   ▼
Kernel
_IOWR

User
   │
   ▼
Kernel
   │
   ▼
User
12. Summary Table
Macro	Data Flow	Driver API
_IO	No data	Perform operation
_IOR	Kernel → User	copy_to_user()
_IOW	User → Kernel	copy_from_user()
_IOWR	Both directions	copy_from_user() + copy_to_user()
Interview Questions
Q1. Why do we use _IO() instead of simply writing #define LED_ON 1?

Answer:
To generate a unique ioctl command number by encoding the magic number, command number, direction, and size, avoiding conflicts between different drivers.

Q2. What is the purpose of the magic number?

Answer:
The magic number identifies the driver. All ioctl commands for a particular driver should use the same magic number.

Q3. What is the difference between _IOR() and _IOW()?

Answer:

_IOR() → Driver sends data to user space using copy_to_user().
_IOW() → User space sends data to the driver using copy_from_user().
Q4. When do we use _IOWR()?

Answer:
When both user space and the driver exchange data in the same ioctl() call.

Key Points to Remember
type = Magic Number (Driver Identifier)
nr = Command Number
_IO = No data transfer
_IOR = Kernel → User
_IOW = User → Kernel
_IOWR = Both directions
Linux internally expands these macros using _IOC().
This Chapter 18 continues naturally from your previous notes. In **Chapter 19**, we'll go one level deeper into the Linux kernel and study the actual implementation of `_IOC()`, including `_IOC_DIRSHIFT`, `_IOC_TYPESHIFT`, `_IOC_NRSHIFT`, and `_IOC_SIZESHIFT`, to understand exactly how the 32-bit `ioctl` command number is constructed.
-------------------------------------------------------------------
# Chapter 19: Understanding _IOC() Internals

## 1. Introduction

In the previous chapter, we learned that:

```c
#define LED_ON _IO('P',1)
```

Internally, Linux expands it as:

```c
_IO('P',1)

↓

_IOC(_IOC_NONE,'P',1,0)
```

`_IOC()` is the core Linux macro responsible for generating a unique 32-bit ioctl command number.

It combines four fields:

- Direction
- Magic Number (Type)
- Command Number (Number)
- Data Size

---

## 2. Linux Definition of _IOC()

Simplified Linux definition:

```c
#define _IOC(dir,type,nr,size) \
        ((dir << _IOC_DIRSHIFT) | \
         (type << _IOC_TYPESHIFT) | \
         (nr << _IOC_NRSHIFT) | \
         (size << _IOC_SIZESHIFT))
```

Every field is shifted to a different bit position and then combined using the bitwise OR (`|`) operator.

---

## 3. Parameters of _IOC()

```c
_IOC(dir,type,nr,size)
```

| Parameter | Meaning |
|-----------|---------|
| dir | Data transfer direction |
| type | Magic Number (Driver Identifier) |
| nr | Command Number |
| size | Size of the data transferred |

---

## 4. Why is _IOC() Needed?

Suppose Linux simply added all values.

Example 1:

```text
Direction = 0
Magic = 80
Number = 1
Size = 0

Total = 81
```

Example 2:

```text
Direction = 1
Magic = 79
Number = 1
Size = 0

Total = 81
```

Different commands produce the same result.

This is called a **collision**.

To avoid collisions, Linux stores every field in a separate bit position.

---

## 5. ioctl Command Layout

An ioctl command is a 32-bit integer.

```text
 -------------------------------------------------------------
| Direction | Data Size | Magic(Type) | Command Number (nr) |
 -------------------------------------------------------------
```

Typical bit layout:

```text
31........30 29............16 15........8 7........0

 Direction      Size          Type         Number
```

---

## 6. Direction Field

Example:

```c
_IO('P',1)
```

Expands to

```c
_IOC(_IOC_NONE,'P',1,0)
```

Direction is

```text
_IOC_NONE
```

Meaning

```text
No data transfer
```

Other possible directions:

| Macro | Meaning |
|--------|---------|
| _IOC_NONE | No data transfer |
| _IOC_READ | Kernel → User |
| _IOC_WRITE | User → Kernel |
| _IOC_READ \| _IOC_WRITE | Both directions |

---

## 7. Magic Number (Type)

Example

```c
_IO('P',1)
```

Magic Number

```text
'P'
```

ASCII value

```text
80
```

Purpose:

Identifies the driver.

Examples

```text
'P' → Pseudo Driver

'U' → UART Driver

'S' → SPI Driver

'I' → I2C Driver
```

All ioctl commands belonging to one driver should use the same magic number.

---

## 8. Command Number (nr)

Example

```c
_IO('P',1)
```

Command Number

```text
1
```

Examples

```text
1 → LED_ON

2 → LED_OFF

3 → LED_TOGGLE

4 → GET_STATUS
```

The command number identifies a specific operation within the driver.

---

## 9. Data Size

Example

```c
_IOR('P',4,int)
```

Size field

```text
sizeof(int)

=

4 bytes
```

Linux stores the size of the transferred data in the ioctl command.

---

## 10. How Bit Shifting Works

```c
dir << _IOC_DIRSHIFT

type << _IOC_TYPESHIFT

nr << _IOC_NRSHIFT

size << _IOC_SIZESHIFT
```

Each value is shifted to its own position.

Then Linux combines them:

```c
encoded_command =
(dir << _IOC_DIRSHIFT) |
(type << _IOC_TYPESHIFT) |
(nr << _IOC_NRSHIFT) |
(size << _IOC_SIZESHIFT);
```

---

## 11. Complete Example

Application

```c
#define LED_ON _IO('P',1)

ioctl(fd, LED_ON);
```

Macro Expansion

```c
_IO('P',1)

↓

_IOC(_IOC_NONE,'P',1,0)
```

Packed Fields

```text
Direction = NONE

Magic = 'P'

Number = 1

Size = 0
```

↓

Linux creates

```text
One Unique 32-bit Integer
```

↓

Driver receives

```c
unsigned int cmd
```

---

## 12. Driver Comparison

Driver:

```c
switch(cmd)
{
    case LED_ON:

        pr_info("LED ON");

        break;

    case LED_OFF:

        pr_info("LED OFF");

        break;
}
```

Both `cmd` and `LED_ON` contain the same encoded value, so the comparison succeeds.

---

## 13. Complete Flow

```text
Application

        │

        ▼

_IO('P',1)

        │

        ▼

_IOC(_IOC_NONE,'P',1,0)

        │

        ▼

Shift Direction

        │

Shift Magic

        │

Shift Number

        │

Shift Size

        │

Bitwise OR

        │

One Encoded 32-bit Command

        │

Kernel

        │

pcd_ioctl()

        │

switch(cmd)

        │

case LED_ON
```

---

## 14. Why Doesn't the Driver Decode cmd?

Normally, the driver never manually extracts:

- Direction
- Type
- Number
- Size

Instead, it simply compares:

```c
case LED_ON:

case LED_OFF:

case GET_STATUS:
```

because the macros generate identical encoded values during compilation.

---

## Interview Questions

### Q1. What is `_IOC()`?

**Answer:**

`_IOC()` is the internal Linux macro that combines direction, magic number, command number, and data size into a unique 32-bit ioctl command.

---

### Q2. Why does Linux use bit shifting?

**Answer:**

Bit shifting places each field in a different bit position, preventing collisions between ioctl commands.

---

### Q3. Why is bitwise OR (`|`) used?

**Answer:**

Bitwise OR combines the shifted fields into one 32-bit command without overlapping the fields.

---

### Q4. What are the four fields encoded by `_IOC()`?

**Answer:**

- Direction
- Magic Number (Type)
- Command Number
- Data Size

---

## Key Points

- `_IO()`, `_IOR()`, `_IOW()`, and `_IOWR()` all expand to `_IOC()`.
- `_IOC()` creates a unique 32-bit ioctl command.
- Bit shifting assigns each field its own position.
- Bitwise OR combines all fields.
- The driver receives the encoded value in the `cmd` parameter.
- Drivers typically compare `cmd` using a `switch` statement rather than decoding it manually.
--------------------------------------------------------------------------------
