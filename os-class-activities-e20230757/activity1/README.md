# Class Activity 1 — System Calls in Practice

- **Student Name:** PEN Bunnaka
- **Student ID:** e20230757
- **Date:** 23 October 2026


## Task 1: File Creator & Reader

### Part A — File Creator

**Describe your implementation:**

- **Library Version (`fopen`, `fprintf`, `fclose`):** Uses standard C library abstractions that operate on buffered `FILE *` streams.
- **System Call Version (`open`, `write`, `close`):** Interfaces directly with the Linux kernel using integer file descriptors (`fd`) with no user-space buffering.

**Version A — Library Functions (`file_creator_lib.c`):**

<!-- Screenshot: gcc -o file_creator_lib file_creator_lib.c && ./file_creator_lib && cat output.txt -->
![Task 1A - Library](screenshots/task1_creator_lib.png)

**Version B — POSIX System Calls (`file_creator_sys.c`):**

<!-- Screenshot: gcc -o file_creator_sys file_creator_sys.c && ./file_creator_sys && cat output.txt -->
![Task 1A - Syscall](screenshots/task1_creator_sys.png)

**Questions:**

1. **What flags did you pass to `open()`? What does each flag mean?**

   > - `O_WRONLY`: Open file for write-only access.
   > - `O_CREAT`: Create the file if it does not already exist.
   > - `O_TRUNC`: Truncate (overwrite) the file to zero length if it already exists.

2. **What is `0644`? What does each digit represent?**

   > `0644` defines file permissions in octal format:
   > - `0`: Specifies octal notation.
   > - `6` (4+2): Owner has Read + Write permissions.
   > - `4`: Group has Read-only permission.
   > - `4`: Others have Read-only permission.

3. **What does `fopen("output.txt", "w")` do internally that you had to do manually?**

   > Internally, `fopen("output.txt", "w")` sets equivalent flags (`O_WRONLY | O_CREAT | O_TRUNC`), applies default creation permissions, and allocates a user-space `FILE` structure with internal I/O buffering. In system calls, we manually pass flags, define octal permissions (`0644`), and manage the raw integer file descriptor.

### Part B — File Reader & Display

**Describe your implementation:**

- **Library Version (`fopen`, `fgets`, `fclose`):** Reads text line-by-line into a buffer and prints using `printf()`.
- **System Call Version (`open`, `read`, `write`, `close`):** Directly reads chunks of bytes into a 256-byte buffer via `read()` and writes them directly to `STDOUT_FILENO` (fd `1`) using `write()`.

**Version A — Library Functions (`file_reader_lib.c`):**

![Task 1B - Library](screenshots/task1_reader_lib.png)

**Version B — POSIX System Calls (`file_reader_sys.c`):**

![Task 1B - Syscall](screenshots/task1_reader_sys.png)

**Questions:**

1. **What does `read()` return? How is this different from `fgets()`?**

   > - `read()` returns the number of raw bytes actually read (`ssize_t`), `0` on EOF, or `-1` on error.
   > - `fgets()` reads line-by-line, appends a null terminator (`\0`), and returns a pointer to the buffer (`char*`), or `NULL` on EOF/error.

2. **Why do you need a loop when using `read()`? When does it stop?**

   > - A loop is required because `read()` processes data in chunks up to the buffer size; larger files require multiple reads.
   > - The loop stops when `read()` returns `0`, which signals End-of-File (EOF).

---

## Task 2: Directory Listing & File Info

**Describe your implementation:**

- **Library Version (`dir_list_lib.c`):** Uses `opendir()`, `readdir()`, and `stat()` with standard `printf()` to output formatted file names and sizes.
- **System Call Version (`dir_list_sys.c`):** Uses `snprintf()` to format directory entry metadata into a string buffer, then writes directly to stdout (`fd 1`) with `write()`.

### Version A — Library Functions (`dir_list_lib.c`)

![Task 2 - Version A](screenshots/task2_lib.png)

### Version B — System Calls (`dir_list_sys.c`)

![Task 2 - Version B](screenshots/task2_sys.png)

### Questions

1. **What struct does `readdir()` return? What fields does it contain?**

   > Returns a pointer to `struct dirent`. Key fields include:
   > - `d_name`: Name of the file/directory.
   > - `d_ino`: Inode number.
   > - `d_type`: Entry type (e.g., regular file `DT_REG`, directory `DT_DIR`).
   > - `d_reclen`: Record length.

2. **What information does `stat()` provide beyond file size?**

   > `struct stat` provides comprehensive file metadata, including file type/permissions (`st_mode`), user ID (`st_uid`), group ID (`st_gid`), hard link count (`st_nlink`), and timestamps for access (`st_atime`), modification (`st_mtime`), and inode status change (`st_ctime`).

3. **Why can't you `write()` a number directly — why do you need `snprintf()` first?**

   > `write()` expects a buffer containing ASCII/UTF-8 character bytes to display on the terminal. An integer in memory is stored in binary representation; `snprintf()` converts binary numeric values into printable ASCII text before writing to stdout.

---

## Optional Bonus: Windows API (`file_creator_win.c`)

Screenshot of running on Windows:

![Task 1 - Windows](screenshots/task1_win.png)

### Bonus Questions

1. **Why does Windows use `HANDLE` instead of integer file descriptors?**

   > A `HANDLE` is an opaque pointer/reference to kernel executive objects. Unlike simple integer indices in a process file table, `HANDLE` provides access to diverse kernel resources (files, processes, events, mutexes) with built-in security, access rights, and object typing.

2. **What is the Windows equivalent of POSIX `fork()`? Why is it different?**

   > The equivalent is `CreateProcess()`. POSIX `fork()` duplicates the parent's entire address space and execution state to create a child process. In contrast, `CreateProcess()` starts a new process from scratch by loading an executable file into a fresh address space.

3. **Can you use POSIX calls on Windows?**

   > Yes, through compatibility layers like Cygwin, MSYS2, or virtualization environments like WSL (Windows Subsystem for Linux).

### Cross-Platform Comparison Table

| Operation | C Library | POSIX System Call (Linux) | Windows API |
|-----------|-----------|--------------------------|-------------|
| Create/open file | `fopen()` | `open()` | `CreateFile()` |
| Write to file | `fprintf()` | `write()` | `WriteFile()` |
| Read from file | `fread()` | `read()` | `ReadFile()` |
| Close file | `fclose()` | `close()` | `CloseHandle()` |
| Delete file | `remove()` | `unlink()` | `DeleteFile()` |
| Print to console | `printf()` | `write(1, ...)` | `WriteFile(GetStdHandle(...), ...)` |

---

## Task 3: strace Analysis

**Describe what you observed:**
The library version made significantly more system calls (38 calls vs 33 calls in `strace -c`) due to standard C runtime overhead, memory allocation (`brk`), and file status checks (`fstat`). The system call version executed direct, unbuffered operations with fewer total syscalls.

### strace Output — Library Version (File Creator)

<!-- Screenshot: strace -e trace=openat,read,write,close ./file_creator_lib -->
<!-- IMPORTANT: Highlight/annotate the key system calls in your screenshot -->
![strace - Library version File Creator](screenshots/strace_lib_creator.png)

### strace Output — System Call Version (File Creator)

<!-- Screenshot: strace -e trace=openat,read,write,close ./file_creator_sys -->
<!-- IMPORTANT: Highlight/annotate the key system calls in your screenshot -->
![strace - System call version File Creator](screenshots/strace_sys_creator.png)

### strace Output — Library Version (File Reader or Dir Listing)

![strace - Library version](screenshots/strace_lib_reader.png)

### strace Output — System Call Version (File Reader or Dir Listing)

![strace - System call version](screenshots/strace_sys_reader.png)

### strace -c Summary Comparison

<!-- Screenshot of `strace -c` output for both versions -->
![strace summary - Library](screenshots/strace_summary_lib.png)
![strace summary - Syscall](screenshots/strace_summary_sys.png)

### Questions

1. **How many system calls does the library version make compared to the system call version?**

   > Based on `strace -c`:
   > - **Library Version (`file_creator_lib`):** 38 system calls total.
   > - **System Call Version (`file_creator_sys`):** 33 system calls total (5 fewer calls).

2. **What extra system calls appear in the library version? What do they do?**

   > - `fstat` (4 calls in lib vs 2 in sys): Queries file descriptors to inspect device type and determine buffer size/strategy.
   > - `brk` (3 calls in lib vs 1 in sys): Dynamically adjusts the process heap boundary for standard library I/O buffer allocation.

3. **How many `write()` calls does `fprintf()` actually produce?**

   > `fprintf()` produced exactly **1** `write()` system call (`write(3, "Hello from Operating Systems cla"..., 36) = 36`). This is because `stdio` buffers output in user space and only triggers a single flush to kernel space when `fclose()` is called.

4. **In your own words, what is the real difference between a library function and a system call?**

   > A **library function** (e.g., `printf`, `fopen`) executes in user mode within the application's address space, providing buffering, data formatting, and abstraction. A **system call** (e.g., `write`, `open`) transitions execution from user mode to kernel mode via a software interrupt/trap to request privileged OS kernel services.

---

## Task 4: Exploring OS Structure

### System Information

> 📸 Screenshot of `uname -a`, `/proc/cpuinfo`, `/proc/meminfo`, `/proc/version`, `/proc/uptime`:

![System Info](screenshots/task4_system_info.png)

### Process Information

> 📸 Screenshot of `/proc/self/status`, `/proc/self/maps`, `ps aux`:

![Process Info](screenshots/task4_process_info.png)

### Kernel Modules

> 📸 Screenshot of `lsmod` and `modinfo`:

![Kernel Modules](screenshots/task4_modules.png)

### OS Layers Diagram

> The diagram of the OS layers, labeled with real data from your system:

![OS Diagram](screenshots/task4_os_layers_diagram.png)


## Questions

### 1. What is `/proc`? Is it a real filesystem on disk?

`/proc` is a virtual filesystem provided by the Linux kernel. It is not a normal filesystem stored on a physical disk. Instead, its files are generated dynamically by the kernel to provide information about running processes, memory usage, CPU details, and other system information.

### 2. Monolithic kernel vs. microkernel — which type does Linux use?

- **Monolithic kernel:** Most core operating system services, including process scheduling, memory management, filesystems, and device drivers, run in kernel space.
- **Microkernel:** Only essential functions run in kernel space, while many other services operate in user space.
- **Linux:** Linux uses a monolithic kernel design. However, it also supports loadable kernel modules, which allow certain kernel components to be loaded and unloaded when needed.

### 3. What memory regions do you see in `/proc/self/maps`?

The `/proc/self/maps` file displays the memory regions used by a process. Common regions include:

- **Executable code:** Contains the program's machine instructions.
- **Data segments:** Store global and static variables with different access permissions.
- **Heap:** Used for dynamic memory allocation, such as with `malloc()`.
- **Shared libraries:** Contains libraries loaded by the program, such as `libc.so.6`.
- **Stack:** Stores function call information, local variables, and other temporary data.
- **`[vdso]` and `[vvar]`:** Special memory regions used to provide certain kernel-related information and allow some operations to be performed efficiently.

The exact regions may vary depending on the program and the system.

### 4. Break down the kernel version string from `uname -a`.

Based on my terminal output, the kernel version is `7.0.0-38-generic`.

- **`7`:** Major kernel version.
- **`0`:** Minor version.
- **`0`:** Additional version component.
- **`38`:** Ubuntu kernel build revision.
- **`generic`:** Indicates the general-purpose Ubuntu kernel flavor.
- **`#38-Ubuntu`:** Identifies the kernel build.
- **`SMP`:** Indicates support for symmetric multiprocessing.
- **`PREEMPT_DYNAMIC`:** Indicates that the kernel supports dynamically configurable preemption behavior.
- **`x86_64`:** Indicates a 64-bit x86 architecture.

### 5. How does `/proc` show that the OS is an intermediary between programs and hardware?

The `/proc` filesystem demonstrates how the operating system provides programs with an interface to system information. For example, `/proc/cpuinfo` provides CPU details, while `/proc/meminfo` shows memory information. The kernel gathers and presents this information in a readable format, allowing user programs to inspect the system without directly accessing hardware.

## Reflection

Through this activity, I learned more about how the Linux operating system works and how programs interact with the kernel. I discovered that library functions, such as `printf()` and `fopen()`, provide convenient interfaces for programmers, while system calls, such as `write()`, `open()`, and `read()`, allow programs to request services from the kernel. I also learned that library functions may use buffering to reduce the number of system calls. Using tools such as `strace` helped me understand the interaction between user space and kernel space. Finally, exploring `/proc` helped me understand how Linux provides information about processes, memory, and hardware.