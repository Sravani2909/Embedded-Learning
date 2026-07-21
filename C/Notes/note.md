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
-----------------------------------
