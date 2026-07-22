                                  Introduction
============================================================================
Good morning. My name is Sravani, and I am from Andhra Pradesh. I completed my M.Tech and have over three years of experience in embedded software development.

During my career, I have worked in the industrial automation, semiconductor, and home appliance domains. My experience includes Embedded C programming, firmware development, and communication protocols such as UART, I2C, and SPI. I have worked extensively with STM32 microcontrollers based on the ARM Cortex-M4 architecture, where I was responsible for firmware development, hardware-software interfacing, debugging, and implementing embedded features.

In addition to bare-metal embedded development, I have recently been enhancing my knowledge of Embedded Linux, including Linux kernel concepts and Linux character device drivers. I also have working knowledge of Python, which I use for automation and scripting tasks.

I enjoy learning new technologies and solving challenging technical problems. I am now looking for an opportunity where I can apply my embedded C and Linux skills, contribute to real-world embedded products, and continue growing as an embedded software engineer. I believe this role at Alten aligns well with my technical background and career goals.
-------------------------------------------------------------------------------








One-line memory formula
temp & 1 → Check the last bit
temp >>= 1 → Move to the next bit
temp & (temp - 1) → Delete one set bit
----------------------------------------------
| Expression | Meaning                                                          |
| ---------- | ---------------------------------------------------------------- |
| `*p++`     | Move the **pointer** after reading the current value             |
| `*++p`     | Move the **pointer first**, then read the value                  |
| `(*p)++`   | Increment the **value**, then use the old value (post-increment) |
| `++*p`     | Increment the **value first**, then use the new value            |
-------------------------------------------------------------------

Difference between char array and pointer
========================================
"char *p = "Hello"; declares a pointer that points to a string literal stored in the read-only data section. The pointer variable itself is stored on the stack if it is a local variable. Modifying the string literal results in undefined behavior."


"char s[] = "Hello"; creates a character array and copies the string into the array. If declared inside a function, the array is stored on the stack and can be modified safely."
-----------------------------------------------------------------------------------
Differenc ebetween static,extern
=================================================================================
global static variable has internal linkage, so it can only be accessed within the same source file, although its lifetime is the entire program. extern is used to declare a global variable that is defined elsewhere, allowing multiple source files to refer to the same variable. So, if I need to share x between main.c and driver.c, I would define x in one .c file and declare it using extern in a header or the other source file.”
----------------------------------------------------------------------------------
| Declaration          | Change `*p`? | Change `p`? |
| -------------------- | -----------: | ----------: |
| `const int *p`       |            ❌ |           ✅ |
| `int *const p`       |            ✅ |           ❌ |
| `const int *const p` |            ❌ |           ❌ |
-------------------------------------------------------------------------------
| Expression | Meaning                    | Example Address |
| ---------- | -------------------------- | --------------- |
| `arr`      | Pointer to first element   | `1000`          |
| `&arr`     | Pointer to the whole array | `1000`          |
| `arr + 1`  | Next integer               | `1004`          |
| `&arr + 1` | Next whole array           | `1020`          |
--------------------------------------------------------------------------
             High Memory Address
        +-------------------------+
        |        Stack            |  ← Local variables,
        |                         |     Function calls
        |      (Grows Down) ↓     |
        +-------------------------+
        |        Heap             |  ← malloc(), calloc()
        |                         |
        |      (Grows Up) ↑       |
        +-------------------------+
        |      .bss Section       |  ← Uninitialized Global &
        |                         |     Static Variables
        +-------------------------+
        |      .data Section      |  ← Initialized Global &
        |                         |     Static Variables
        +-------------------------+
        |     .rodata Section     |  ← String Literals,
        |                         |     const Data
        +-------------------------+
        |     .text Section       |  ← Program Instructions
        +-------------------------+
             Low Memory Address
===================================================
Remember these rules:

Code → .text
String literals & constants → .rodata
Initialized global/static variables → .data
Uninitialized global/static variables → .bss
Dynamic memory (malloc) → Heap
Local variables, function parameters, local arrays → Stack
-----------------------------------------------------
| Variable                          | Memory Section                                |
| --------------------------------- | --------------------------------------------- |
| `int g = 10;`                     | `.data`                                       |
| `static int s = 20;`              | `.data`                                       |
| `static int z;`                   | `.bss`                                        |
| `const char *p = "Hello";`        | `p` → Stack (if local), `"Hello"` → `.rodata` |
| `int x = 5;`                      | Stack                                         |
| `char str[] = "Hi";`              | Stack                                         |
| `int *ptr = malloc(sizeof(int));` | `ptr` → Stack, allocated memory → Heap        |
| Function code                     | `.text`                                       |
------------------------------------------------------------------
Interview Tip: A common trick question is: "Where is const char *p = "Hello"; stored?" Always distinguish between the pointer variable and the string literal. The pointer and the string are stored in different memory regions.
-----------------------------------------------------------------
Why do we use macros in Embedded C?
Answer

Macros are used in Embedded C because they are expanded by the preprocessor before compilation, so they avoid function call overhead. They make the code faster, more readable, reusable, and easier to maintain, which is important in resource-constrained embedded systems.
| Application             | Example                                                |
| ----------------------- | ------------------------------------------------------ |
| Register Access         | `#define GPIOA_ODR (*(volatile uint32_t *)0x40020014)` |
| Bit Manipulation        | `SET_BIT()`, `CLEAR_BIT()`                             |
| Conditional Compilation | `#ifdef DEBUG`                                         |
| Header Guards           | `#ifndef UART_H`                                       |
| Constants               | `#define PI 3.14`                                      |
| Function-like Macros    | `MAX()`, `MIN()`, `ABS()`                              |
----------------------------------------------------------------------
| Feature       | `#define PI 3.14`         | `const float PI = 3.14` |
| ------------- | ------------------------- | ----------------------- |
| Processed by  | Preprocessor              | Compiler                |
| Memory        | ❌ No                      | ✅ Yes                   |
| Type checking | ❌ No                      | ✅ Yes                   |
| Scope         | Preprocessor substitution | Normal C scope          |
| Debugging     | Difficult                 | Easy                    |
--------------------------------------------------------
volatile int counter = 0;

counter++;
Question

Is counter++ an atomic operation?

Can two interrupts or two threads safely execute it at the same time?
Does volatile solve this?

❌ No.

volatile only tells the compiler:

"Don't optimize access to this variable."

It does not:

Make operations atomic.
Prevent interrupts.
Protect against race conditions.
Synchronize multiple threads.
How do we make it safe?

In embedded systems, common approaches are:

Disable interrupts (for very short critical sections)
__disable_irq();
counter++;
__enable_irq();
Or use
Mutexes (RTOS)
Spinlocks (Linux kernel)
Atomic operations (stdatomic.h in C11 or platform-specific atomics)
⭐ Interview Answer (30 seconds)

"counter++ is not an atomic operation because it consists of three steps: read, modify, and write. If an interrupt or another thread accesses the variable between these steps, a race condition can occur and updates may be lost. The volatile keyword prevents compiler optimization but does not make the operation atomic or thread-safe. To make it safe, we need synchronization mechanisms such as disabling interrupts, mutexes, or atomic operations."
-----------------------------------------------------
function pointer
=================
"A function pointer is a pointer variable that stores the address of a function. Using the function pointer, we can call the function indirectly."

Why do we use Function Pointers?

You mentioned:

✅ Callback functions
✅ Linux device drivers (file_operations)

Excellent.

You can also add:

State machines
Interrupt vector tables (depending on the microcontroller)
Menu-driven applications
Interview-Ready Answer (1 Minute)

"A function pointer is a pointer variable that stores the address of a function. It allows us to invoke a function indirectly through the pointer. Function pointers improve flexibility because the function to be executed can be decided at runtime. In embedded systems, they are commonly used in callback functions, interrupt vector tables, Linux device drivers through the file_operations structure, and state machine implementations."
Interview Tip
=================
If the interviewer asks:

"Why not call the function directly?"

A good answer is:

"Direct function calls are fixed at compile time. Function pointers allow the function to be selected dynamically at runtime, making the code more flexible and reusable."
-------------------------------------------------------
One small terminology correction

You said:

"Function name represents the base address."

For arrays, we often say base address.

For functions, it's better to say:

"The function name represents the address of the function."

That's the wording interviewers expect.

Interview-Ready Answer

"The output is 30. First, a function pointer fp is declared that can point to a function taking two integers and returning an integer. Next, fp = add; stores the address of the add function in the function pointer. When fp(10,20) is executed, control transfers to the add function, which calculates 10 + 20 and returns 30. Finally, printf prints 30. We can write either fp = add; or fp = &add; because, in C, the function name decays to its address in this context."
--------------------------------------------------------------
Memory Trick
Function Pointer
int (*fp)(int, int);

Read it inside-out:

fp
 ↓
pointer
 ↓
to function
 ↓
(int, int)
 ↓
returns int
Function Returning Pointer
int *fp(int, int);

Read it inside-out:

fp
 ↓
function
 ↓
(int, int)
 ↓
returns pointer to int
Interview Tip

Many interviewers ask:

"How do you read int (*fp)(int, int);?"

A good answer is:

"fp is a pointer to a function that takes two integer arguments and returns an integer."
--------------------------------------
Question 4 (Most Important)
==========================
This is the question that interviewers ask candidates with 3+ years of embedded experience.

Where have you used function pointers in Embedded Systems?

Give at least three practical examples and explain each briefly.

Examples could include:

Interrupt Service Routines (ISR)
Callback functions
Linux Device Drivers (file_operations)
State machines--

I have primarily used function pointers in callback functions and Linux device drivers. In callback functions, the function pointer is passed as an argument so that the called function can invoke it when required. In Linux device drivers, the file_operations structure contains function pointers for operations such as open, read, write, and release, allowing the kernel to invoke the appropriate driver function. I also know that function pointers are used in interrupt vector tables and state machine implementations.
--------------------------------------------------------
If the interviewer asks:

Why can we write fp() instead of (*fp)()?

You can answer:

"Both are equivalent in C. fp is a function pointer. When a function pointer is followed by (), the compiler automatically dereferences it. Therefore, fp() and (*fp)() both invoke the function whose address is stored in fp."
----------------------------------------------------------
Reading complex declarations (using the "inside-out" rule)----int (*fp)(int,int)---pointer to a function with two intereger arguments with return type integer.
Explaining why parentheses are required (operator precedence).ex:int *fp(int,int),int(*fp)(int,int) both are valid ,1st one is normal function with two aruments with return type is integer pointer,2nd one is function pointer
----------------------------------
                LINUX DEVICE DRIVERS 
=======================================================================
Can you explain what happens when you insert your Linux device driver using insmod?

Interview-Ready Answer (2 Minutes)

"When we execute the insmod command, the Linux kernel loads the driver module into kernel memory. After the module is loaded, the kernel invokes the module initialization function, which is specified using the module_init() macro.

Inside the initialization function, the first step is to allocate a device number using alloc_chrdev_region(). This function allocates a major number dynamically and a minor number for the character device.

Next, we initialize the character device structure using cdev_init(). This function associates our file_operations structure with the cdev. The file_operations structure contains function pointers such as open, read, write, and release.

After initialization, we register the character device with the kernel using cdev_add(). From this point onward, the kernel knows about our character device and its associated operations.

Then, we create a device class using class_create(). This class is mainly used by the sysfs subsystem and helps organize devices under /sys/class.

Finally, we create the device node using device_create(). This creates an entry such as /dev/mydevice, allowing user-space applications to access the driver using standard system calls like open(), read(), write(), and ioctl().

Once these initialization steps are complete, the driver is ready to communicate with user-space applications."

Short Interview Version (1 Minute)

"insmod loads the kernel module into memory and invokes the driver's initialization function. The driver first allocates a device number using alloc_chrdev_region(), initializes the character device with cdev_init(), and registers it with the kernel using cdev_add(). It then creates a device class using class_create() and finally creates the device node using device_create(). After these steps, the device file is available under /dev, and user applications can access the driver through system calls."

Flow Diagram (Remember This)
User
  |
  | insmod driver.ko
  |
  v
Kernel loads module
  |
  v
module_init(driver_init)
  |
  +----------------------------+
  |                            |
  | alloc_chrdev_region()      | --> Allocate Major & Minor number
  |                            |
  | cdev_init()                | --> Initialize cdev with file_operations
  |                            |
  | cdev_add()                 | --> Register cdev with kernel
  |                            |
  | class_create()             | --> Create device class (/sys/class)
  |                            |
  | device_create()            | --> Create /dev/mydevice
  +----------------------------+
               |
               v
Driver Ready
               |
               v
User can call
open()
read()
write()
ioctl()
close()
-------------------------------------------------------------------------------

Explain the flow of open("/dev/mydevice", O_RDWR);

You don't need to explain how the driver is registered unless they ask.

Driver registration happens earlier, when the module is loaded:

insmod
   ↓
driver_init()
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

After this setup, the device file /dev/mydevice already exists.

Only then does the application execute:

open("/dev/mydevice", O_RDWR);
Interview-Ready Answer

"When the application calls open("/dev/mydevice", O_RDWR), the open() library function invokes the open system call, which switches execution from user space to kernel space. The kernel passes the request to the Virtual File System (VFS). The VFS looks up the /dev/mydevice entry and obtains its inode. From the inode, it determines the major and minor numbers. Using the major number, the kernel identifies the registered character device and retrieves the associated cdev structure. The cdev contains a pointer to the file_operations structure. The VFS then invokes the .open function pointer from file_operations, which calls my driver's my_open() function. After my_open() returns, control returns through the VFS back to user space, and open() returns a file descriptor if successful."

Complete Flow Diagram
User Application
      |
      | open("/dev/mydevice")
      |
      v
glibc open()
      |
      v
System Call
      |
      v
Kernel
      |
      v
VFS
      |
      v
Lookup /dev/mydevice
      |
      v
inode
      |
      v
Major / Minor Number
      |
      v
cdev
      |
      v
file_operations
      |
      v
.open ---> my_open()
      |
      v
Return File Descriptor
      |
      v
User Application
----------------------------------------------------------------
Interviewer's Follow-up Question

Suppose you have:

static const struct file_operations fops = {
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release,
};
Question:3

Who initializes this file_operations structure?

And how does the kernel know that my_open() should be called when the application executes open()?-
Annswer:
==========
"The driver initializes the file_operations structure by assigning function pointers such as .open = my_open and .read = my_read. During initialization, cdev_init() associates this file_operations structure with the character device, and cdev_add() registers it with the kernel. Later, when the application calls open(), the VFS locates the corresponding cdev, retrieves its file_operations structure, and invokes the .open function pointer. Since .open points to my_open, the kernel executes my_open(inode, file)."

--------------------------------------------------------------------------------------------------------------------------------------
Next Interview Question (Frequently Asked):5
static int my_open(struct inode *inode, struct file *file)
{
    return 0;
}
Questions
a.Why does my_open() receive two arguments?
b.What information is available in struct inode?
c.What information is available in struct file?


a.my_open() receives:

static int my_open(struct inode *inode, struct file *file)

because the kernel needs to provide:

inode → information about the device (or file) itself.
file → information about the current open instance.


b."struct inode represents the device file in the filesystem. It contains metadata such as the major and minor device numbers, file permissions, ownership, and other filesystem-related information."

In character drivers, we commonly use it to obtain:

iminor(inode);
imajor(inode);

c."Every time we are creating the new file object."

✅ This is correct.

Every successful open() creates a new struct file.

It represents that particular open instance.

It contains information such as:

Current file position (f_pos)
File access mode
Pointer to file_operations
Private driver data (private_data)

For example:

file->private_data = my_device;

This is commonly done in my_open() and then used later in read(), write(), and ioctl().



Memory Trick (Very Important)

Think of it like this:

inode

Information about the device itself

Created once.

Contains:

Major number
Minor number
File metadata
file

Information about one open session

Created every time someone executes:

open("/dev/mydevice");

If three applications open the device:

Application A ---> file object A

Application B ---> file object B

Application C ---> file object C

All three may point to the same inode, but each has its own struct file.

Interview-Ready Answer

"struct inode contains information about the device file, including the major and minor device numbers and other filesystem metadata. It represents the device itself. struct file represents a specific open instance of the device. A new struct file object is created on every successful open() call and stores information such as the current file position, access mode, and private_data. Therefore, my_open() receives both pointers so the driver has access to the device information and the current open session."
------------------------------------------------------------------------------------------------
Why do we need both cdev_init() and cdev_add()? Why can't we call only cdev_add()?

Interview-Ready Answer

"cdev_init() and cdev_add() perform two different tasks. cdev_init() initializes the cdev structure and associates it with the driver's file_operations structure. At this stage, the character device exists only in memory and is not known to the kernel.

cdev_add() registers the initialized cdev with the kernel using the allocated device number. After this step, the kernel can locate the driver and invoke the appropriate functions, such as my_open(), my_read(), and my_write().

Therefore, we cannot call cdev_add() without first calling cdev_init(), because the cdev structure must be initialized before it can be registered."
--------------------------------------------------------------------------------
One-Line Difference (Very Important)
Function	Purpose
cdev_init()	Initializes the cdev structure and links it to file_operations.
cdev_add()	Registers the initialized cdev with the kernel.
Interview Tip

Many interviewers ask:

"Does cdev_init() register the driver?"

The correct answer is:

"No. cdev_init() only initializes the cdev structure. Registration with the kernel happens in cdev_add()."

This distinction is asked very frequently in Linux Device Driver interviews.
---------------------------------------------------------------------------------------------------------
                 cdev
+------------------------------------+
| kobject                            |
| owner = THIS_MODULE                |
| ops ---------------------------+   |
| dev = Major:240 Minor:0        |   |
| count = 1                      |   |
+--------------------------------+---+
                                 |
                                 v
                    file_operations
              +--------------------------+
              | open    -> my_open()     |
              | read    -> my_read()     |
              | write   -> my_write()    |
              | release -> my_release()  |
              +--------------------------+
| Member  | Purpose                        |
| ------- | ------------------------------ |
| `ops`   | Pointer to `file_operations` ⭐ |
| `owner` | Module owner (`THIS_MODULE`)   |
| `dev`   | Major and minor device number  |
| `count` | Number of devices              |
nterviewer:

Which member of struct cdev is the most important?

Answer:

"ops is the most important member because it points to the file_operations structure. Through this pointer, the kernel invokes driver functions such as my_open(), my_read(), my_write(), and my_release() when user-space applications perform file operations."
-----------------------------------------------------------------------------------------------------------------------------------
What is a Device Tree? Why do we need it?

Interview-Ready Answer

"A Device Tree is a data structure that describes the hardware of an embedded board to the Linux kernel. It contains information about hardware components such as CPUs, memory, GPIOs, I2C devices, SPI devices, UARTs, interrupt lines, clocks, and other peripherals. This allows the kernel to understand the hardware configuration without hardcoding board-specific information into the kernel source."

Why is Device Tree Needed?

Without a Device Tree:

The kernel source code would need to be modified for every new hardware board.

With a Device Tree:

The same kernel image can support different boards.
Only the Device Tree Blob (.dtb) changes.
Device Tree Flow
Bootloader (U-Boot)
        |
        | Loads
        |
        +---- Linux Kernel
        |
        +---- Device Tree Blob (.dtb)
                     |
                     v
           Kernel parses Device Tree
                     |
                     v
           Driver gets hardware information
Example
uart0 {
    compatible = "ti,am3352-uart";
    reg = <0x44e09000 0x2000>;
    interrupts = <72>;
    status = "okay";
};

This tells the kernel:

UART controller type
Register address
Interrupt number
Device enabled
Interview Question

How does a driver use the Device Tree?

Answer:

"The driver matches the compatible string in the Device Tree with the driver's of_device_id table. If they match, the kernel probes the driver and passes the Device Tree information to it."

Example:

static const struct of_device_id my_driver_ids[] = {
    { .compatible = "ti,am3352-uart" },
    { }
};
----------------------------------------------------------------------------------------------------
2. Top Half and Bottom Half of Interrupt

This is another favorite interview topic.

Why do we divide interrupt handling?

When an interrupt occurs:

The CPU should respond immediately.
But lengthy processing inside the interrupt handler blocks other interrupts and reduces system responsiveness.

So Linux splits interrupt handling into:

Top Half
Bottom Half
Top Half

Runs immediately after the interrupt.

Responsibilities:

Acknowledge the interrupt.
Read the hardware status.
Clear the interrupt.
Schedule the bottom half.

Must execute quickly.

Example:

irqreturn_t my_irq_handler(int irq, void *dev)
{
    /* Read interrupt status */

    /* Clear interrupt */

    /* Schedule bottom half */

    return IRQ_HANDLED;
}
Bottom Half

Runs later.

Responsibilities:

Long processing
Copy data
Network processing
Data parsing
Buffer management

Since it is deferred, it allows the CPU to handle new interrupts promptly.

Example

Suppose a UART receives data.

Top Half
UART Interrupt
      |
      |
Read received byte
Clear interrupt
Schedule bottom half
Return immediately

Execution time:

Very Fast
Bottom Half
Copy data to buffer

Check packet

Wake up application

Process protocol

Execution time:

Long
Why is this useful?

Imagine:

A UART receives 1000 interrupts per second.

If the ISR processes every packet completely:

Interrupt
↓

Copy data

Checksum

Protocol parsing

Printing

Memory allocation

Return

The CPU spends too much time in the interrupt handler, delaying other interrupts.

Instead:

Interrupt

↓

Read data

Clear interrupt

Schedule bottom half

Return

The lengthy work happens later, improving responsiveness.

Bottom Half Mechanisms

Linux provides several ways to implement the bottom half:

SoftIRQ – High-performance, used internally by the kernel.
Tasklet – Built on SoftIRQ, commonly used in older drivers.
Workqueue – Runs in process context and can sleep.
Interview Difference Table
Top Half	Bottom Half
Runs immediately	Runs later
Interrupt context	Deferred execution
Must be fast	Can perform longer processing
Cannot sleep	Workqueues can sleep
Acknowledge and clear interrupt	Process data and perform lengthy work
Interview Answer (1 Minute)

"Linux divides interrupt handling into two parts. The top half executes immediately when the interrupt occurs. Its job is to acknowledge the interrupt, read or clear the hardware status, and schedule the bottom half. It should finish as quickly as possible. The bottom half performs the longer processing, such as copying data, protocol handling, or waking up applications. This design reduces interrupt latency and improves overall system responsiveness."

------------------------------------------------------
Explain how a character driver works from loading to user access."

You can answer:

"When the module is loaded using insmod, the driver's initialization function is executed through module_init(). First, alloc_chrdev_region() allocates a unique major and minor number. Next, cdev_init() initializes the cdev structure and associates it with the file_operations callbacks. Then cdev_add() registers the character device with the kernel. After that, class_create() creates a device class in sysfs, and device_create() creates the /dev device node, allowing user applications to access the driver. When an application calls open(), the request goes through the system call interface and the VFS. The VFS identifies the driver using the inode's major number, finds the associated cdev, and invokes the appropriate callback from file_operations, such as my_open(), my_read(), my_write(), ioctl(), or release(). This is the complete flow from driver registration to user-space access."
---------------------------------------------------------
What is Device Tree Matching?

Device Tree Matching is the process by which the Linux kernel matches a Device Tree node with the appropriate device driver.

The matching is based on the compatible property.

Step 1: Device Tree Node

Suppose your board has an LED controller.

led@1000 {
    compatible = "mycompany,my-led";
    reg = <0x1000 0x100>;
    interrupts = <10>;
    status = "okay";
};
Explanation
compatible → Device identifier
reg → Hardware register address
interrupts → Interrupt number
status = "okay" → Device is enabled
Step 2: Driver

The driver provides an of_device_id table.

static const struct of_device_id led_of_match[] = {
    { .compatible = "mycompany,my-led" },
    { }
};

MODULE_DEVICE_TABLE(of, led_of_match);

This table tells the kernel:

"My driver supports any Device Tree node whose compatible string is mycompany,my-led."

Step 3: Platform Driver Registration
static struct platform_driver led_driver = {
    .probe  = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "my_led_driver",
        .of_match_table = led_of_match,
    },
};

module_platform_driver(led_driver);

Here,

.of_match_table = led_of_match;

connects the driver to the Device Tree matching table.

Step 4: Kernel Matching Process

When Linux boots:

Bootloader

↓

Kernel Starts

↓

Kernel Reads Device Tree (.dtb)

↓

Finds:

compatible = "mycompany,my-led"

↓

Looks at all registered drivers

↓

Checks each driver's of_match_table

↓

Match Found

↓

Calls probe()
Complete Flow
Device Tree

compatible = "mycompany,my-led"

        │
        ▼
Linux Kernel Reads DT

        │
        ▼
Platform Bus

        │
        ▼
Driver's of_match_table

        │
        ▼
Compare Compatible String

        │
        ▼
Match Found

        │
        ▼
probe() Called

        │
        ▼
Driver Initializes Device
What Happens Inside probe()?

Once a match occurs, the kernel calls:

static int led_probe(struct platform_device *pdev)
{
    printk("LED Driver Probe Called\n");

    return 0;
}

In a real driver, probe() typically:

Reads register addresses
Maps hardware registers
Requests IRQs
Initializes GPIOs
Registers the character device (if needed)

Example:

struct resource *res;

res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

base = devm_ioremap_resource(&pdev->dev, res);

Here:

platform_get_resource() reads the reg property from the Device Tree.
devm_ioremap_resource() maps that physical address into kernel virtual memory.
Real Example (UART)
Device Tree
uart0 {
    compatible = "ti,omap4-uart";
    reg = <0x44e09000 0x1000>;
    interrupts = <72>;
};
UART Driver
static const struct of_device_id uart_of_match[] = {
    { .compatible = "ti,omap4-uart" },
    { }
};
Boot Flow
Device Tree

compatible = "ti,omap4-uart"

↓

Kernel

↓

UART Driver

↓

Compatible Matches

↓

uart_probe()

↓

UART Ready
Why Do We Use compatible?

Imagine there are multiple drivers:

GPIO Driver

UART Driver

SPI Driver

I2C Driver

LED Driver

The kernel needs a way to know which driver belongs to which hardware.

The compatible string provides that identification.

Interview Questions
Q1. How does the Linux kernel match a driver with a device?

Answer:

"The kernel reads the Device Tree during boot and checks the compatible property of each node. It compares this string with the driver's of_match_table. If they match, the kernel invokes the driver's probe() function."

Q2. What is of_match_table?

Answer:

"of_match_table is an array of of_device_id structures containing the compatible strings supported by the driver. It is used by the kernel for Device Tree matching."

Q3. When is probe() called?

Answer:

"probe() is called automatically after the kernel successfully matches a Device Tree node with a driver's of_match_table."

One-Minute Interview Answer

"During Linux boot, the kernel parses the Device Tree Blob (DTB). Each device node contains a compatible property that identifies the hardware. Every platform driver provides an of_match_table with the compatible strings it supports. The kernel compares the Device Tree's compatible string with each driver's of_match_table. If a match is found, it creates a platform device and calls the driver's probe() function. Inside probe(), the driver obtains resources such as register addresses and interrupts from the Device Tree and initializes the hardware."
