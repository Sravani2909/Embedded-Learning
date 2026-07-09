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
