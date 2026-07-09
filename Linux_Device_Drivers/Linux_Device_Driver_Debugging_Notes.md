# Linux Device Driver Debugging Notes

## Issue 1: Unable to Insert Kernel Module

### Problem

``` bash
sudo insmod hello.ko
```

**Error:**

``` text
insmod: ERROR: could not insert module hello.ko: Key was rejected by service
```

## Step 1: Identify the Stage

-   `make` completed successfully.
-   `hello.ko` was generated.
-   Therefore, the problem occurred during **module loading**, not
    compilation.

## Step 2: Read the Error

The message **"Key was rejected by service"** indicates a
security-related issue rather than a coding error.

## Step 3: Check Kernel Messages

``` bash
sudo dmesg | tail -20
```

Purpose: - Check why the kernel rejected the module.

## Step 4: Check Secure Boot

``` bash
mokutil --sb-state
```

Output:

``` text
SecureBoot enabled
```

## Root Cause

UEFI Secure Boot was enabled.

When Secure Boot is enabled, Linux allows only trusted, digitally signed
kernel modules to be loaded.

Your module `hello.ko` was unsigned, so the kernel rejected it.

## Solution

1.  Reboot into BIOS/UEFI.
2.  Go to **Security → Secure Boot**.
3.  Set **Secure Boot = Disabled**.
4.  Save changes using **F10**.
5.  Boot into Ubuntu.

Verify:

``` bash
mokutil --sb-state
```

Expected:

``` text
SecureBoot disabled
```

## Verification

``` bash
sudo insmod hello.ko
lsmod | grep hello
sudo dmesg | tail -5
```

Expected: - Module loads successfully. - `lsmod` shows `hello`. -
`dmesg` displays **Hello Linux Kernel!**

## Commands Used

``` bash
make
ls
sudo insmod hello.ko
sudo dmesg | tail -20
mokutil --sb-state
lsmod | grep hello
```

## Interview Question

**Q:** Why did `insmod` fail with **"Key was rejected by service"**?

**Answer:**

Because Secure Boot was enabled. The kernel accepts only trusted,
digitally signed kernel modules. Since `hello.ko` was unsigned, it was
rejected. Disabling Secure Boot (or signing the module) resolves the
issue.

# Linux Device Driver Debugging Framework

1.  Identify whether the issue is:
    -   Compilation
    -   Module loading
    -   Runtime
2.  Read the exact error message.
3.  Check `dmesg`.
4.  Verify the environment:
    -   Kernel version
    -   Kernel headers
    -   Secure Boot
    -   Dependencies
5.  Identify the root cause.
6.  Apply the fix.
7.  Verify the solution.
/////////////////////////////////////////////


| File             | Purpose                                         |
| ---------------- | ----------------------------------------------- |
| `hello.c`        | Your source code                                |
| `hello.o`        | Compiled object file                            |
| `hello.mod.c`    | Auto-generated module metadata                  |
| `hello.mod.o`    | Compiled metadata object                        |
| `hello.ko`       | Final kernel module                             |

| `Module.symvers` | Exported kernel symbols and version information |
| `modules.order`  | Order of built modules                          |





/////////////////////////

| Error                         | Root Cause              | Solution                               |
| ----------------------------- | ----------------------- | -------------------------------------- |
| `missing separator`           | TAB missing in Makefile | Use a TAB instead of spaces            |
| `Key was rejected by service` | Secure Boot enabled     | Disable Secure Boot or sign the module |
| `Unknown symbol`              | Missing exported symbol | Check dependencies/exports             |
| `Invalid module format`       | Kernel version mismatch | Rebuild against the running kernel     |
| `Exec format error`           | Wrong architecture      | Compile for the correct target         |
| `Permission denied`           | Insufficient privileges | Use `sudo` or fix permissions          |




