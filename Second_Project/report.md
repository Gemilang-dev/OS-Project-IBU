# Project 2: Memory Management in Linux using `mmap()` and `munmap()`

- Rayi Aqli Gemilang
- Isa Selimovic

## Task 2.1

### **📌 mmap**

Function: Allocates memory (usually in page size, 4KB) from the operating system into the program memory space.

```
void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
```

Parameter Explanation:


| Parameter                      | Descriptioncol2                                                                                        |
| ------------------------------ | ------------------------------------------------------------------------------------------------------ |
| NULL                           | Let the OS choose the starting address of the allocated memory.                                        |
| 4096                           | The requested memory size (in bytes), for example 4096 = 1 page (4KB).                                 |
| PROT\_READ I PROT\_WRITE      | Memory access rights: can be read and written.                                                         |
| MAP\_PRIVATE I MAP\_ANONYMOUS | MAP_PRIVATE: changes are not shared with other processes. MAP_ANONYMOUS: not associated with any file. |
| -1                             | Because it does not use a file, the file descriptor is filled with -1.                                 |
| 0                              | Initial offset in the file (if using a file), filled with 0 for anonymous.                             |

PROT_READ | PROT_WRITE Memory access rights: can be read and written.
MAP_PRIVATE | MAP_ANONYMOUS MAP_PRIVATE: changes are not shared with other processes.
MAP_ANONYMOUS: not associated with any file.
-1 Because it does not use a file, the file descriptor is filled with -1.
0 Initial offset in the file (if using a file), filled with 0 for anonymous.

Result: Pointer (ptr) to an empty block of memory that can be used like an array or regular buffer.

### **📌 munmap**

Function: Frees (unmaps) memory allocated by mmap.

```
munmap(ptr, 4096);
```

Parameter Explanation:


| Parameter | Description                                                                              |
| --------- | ---------------------------------------------------------------------------------------- |
| ptr       | The starting address of the memory to be freed (must be the same as the result of mmap). |
| 4096      | The size of the memory to be freed (must be the same as when allocated).                 |

Notes :

* mmap is useful when you want to manage memory manually, for example creating a replacement for malloc.
* After finishing using memory, always use munmap to prevent memory leaks.

## 2.2 Implemtation C Program (mmap\_demo.c)

### 📌 Compile Program

```
gcc mmap_demo.c -o mmap_demo
```

### 📌 Run Program

```
./mmap_demo
```

### 📌 Result

```
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/Second_Project$ ./mmap_demo[Initial]
PID    VSZ   RSS COMMAND
2215   2552  1688 mmap_demo[After mmap]
PID    VSZ   RSS COMMAND
2215   2556  1688 mmap_demo[After writing to memory]
PID    VSZ   RSS COMMAND
2215   2556  1692 mmap_demo

Memory successfully unmapped.
```

## Task 2.3: Memory Usage Analysis

Result after update code for 2.3

```
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/Second_Project$ ./mmap_demo[Initial]
PID    VSZ   RSS COMMAND
10175   2552  1708 mmap_demo
[After mmap]
PID    VSZ   RSS COMMAND
10175   2556  1708 mmap_demo
[Pause] Press Enter to continue after checking /proc/10175/maps...
[After writing to memory]
PID    VSZ   RSS COMMAND
10175   2688  1776 mmap_demo
[Pause] Press Enter to continue after checking /proc/10175/maps again...
Memory successfully unmapped.
```

### 📌 1. VSZ and RSS recording using `ps` at each stage

The following command was used to check memory usage:

```
void print_memory_usage(const char *stage) {
char command[256];   
snprintf(command, sizeof(command),
"echo \"\\n[%s]\" && ps -o pid,vsz,rss,comm -p %d", // HERE
stage, getpid());  
system(command);  
}
```

Results observed from your output:


| Stage                       | VSZ (KB) | RSS (KB) |
| --------------------------- | -------- | -------- |
| Initial                     | 2552     | 1708     |
| After`mmap()`               | 2556     | 1708     |
| After writing to memory**** | 2688     | 1776     |

Base on the table we can draw conclution as below:

- Since the process acquires a new virtual memory zone after calling mmap(), the Virtual Set Size (VSZ) somewhat grows.
- Linux's lazy allocation (which means no physical memory is allocated until the page is requested) means that RSS (Resident Set Size) stays intact after mapping.
- Writing to memory causes a page fault, which in turn causes the system to allocate real physical memory, therefore RSS increases only after that.

### 📌 2. /proc/pid/maps observation

#### a. After `mmap()` (before `memset()`)

When [pause] appear run this code

```
cat /proc/$(pgrep mmap_demo)/maps > maps_after_mmap.txt
```

The results of the many results from `/proc/<PID>/maps` These are the results that we can process :

```
7f70be6ae000-7f70be6b1000 rw-p 00000000 00:00 0
```

**Interpretation:**

* This is the memory region created by `mmap()`.
* Permissions: `rw-p` → Read/Write and Private.
* No file mapping → It’s anonymous memory.
* Size: 4 KB (from `...ae000` to `...b1000`).

From these data it can be concluded that :

> Although the memory region is visible in the maps file, it has not yet used any physical memory because no access/write has occurred

#### b. After writing to memorysss

after seeing the results of the first code on the terminal that runs `cat /proc/$(pgrep mmap_demo)/maps > maps_after_mmap.txt` then. rerun the terminal from the `mmap_demo.c` program and pause will occur again then run this on another terminal

```
cat /proc/$(pgrep mmap_demo)/maps > maps_after_write.txtSeta
```

Following the writing of the memory map to it by memset(), we re-examined it using `/proc//maps. `  The mmap()-generated anonymous memory area's virtual address range and permissions remained unchanged and visible.

For example:

```
7f70be6ae000-7f70be6b1000 rw-p 00000000 00:00 0
```

Thus, it is confirmed that the identically mapped memory region remained operational.

While there was no outward change to the address range, we did see that the process data's RSS (Resident Set Size) increased from 1708 KB to 1776 KB.   This indicates that the memory was accessed (written to) after the kernel has already allocated physical memory to support that location.

Because Linux uses a lazy allocation method, which typically results in a page fault upon first access, this conduct is consistent with that method.   Operating systems increase RSS when they map a physical frame to satisfy an access.

### Interpretation:

* In `/proc/pid/maps`, you can see that the anonymous memory was still allocated after writing.
* It has been confirmed by the increase in RSS that physical memory was allocated only after access occurred.  Physical memory was allocated immediately upon access, as confirmed by the increase in RSS.
* Lazy memory allocation and the triggering of a page fault on first write are compatible with this.  Lazy memory allocation and the triggering of a page fault on first write are compatible with this.

## Task 2.4 – Explain Observations

This section summaries and elucidates the memory use behaviour of the mmap\_demo program using the `/proc//maps` file, the results of ps, and our understanding of Linux memory management algorithms.

### a. Memory Usage Observations

The memory usage at each stage of the program execution is as follows :


| Stage                    | VSZ (KB) | RSS (KB) | Description                                      |
| ------------------------ | -------- | -------- | ------------------------------------------------ |
| **Initial**              | 2552     | 1708     | Before any memory mapping                        |
| **After`mmap()`**        | 2556     | 1708     | Anonymous page mapped, but not yet used          |
| **After writing memory** | 2688     | 1776     | Page accessed (written to), physical memory used |

* The Virtual Set Size (VSZ) increased slightly after mmap() due to the expansion of the program's virtual address space.
* Maps had no effect on Resident Set Size (RSS) because the RAM wasn't used yet.
* The jump in RSS after writing to memory was an indication that the page was now being supported by actual physical memory.

### b. Excerpts from `/proc/<PID>/maps`

We observed the following relevant memory segment after mapping and writing to the memory:

```
7f70be6ae000-7f70be6b1000 rw-p 00000000 00:00 0
```

Here we have an anonymous private mapping that was created with MAP_ANONYMOUS | MAP_PRIVATE.  Throughout the entirety of the show, the region stayed.

As soon as the first write operation was performed, the RSS data became backed by physical memory; the memory range stayed constant throughout the mapping and writing steps.

### c. Analysis & Explanation

These observations confirm key concepts of **Linux memory management**:

* **Lazy Allocation**: When a process uses `mmap()` to map memory, Linux does not immediately allocate physical pages. Instead, it reserves the address space in the virtual memory layout.
* **Page Faults**: Physical memory is only allocated when the process accesses the page (e.g., by writing to it). This triggers a **page fault**, which causes the kernel to allocate a page frame and map it to the corresponding virtual address.
* **Impact on RSS**: Since RSS measures the portion of memory that is physically resident, it only increases after the memory is accessed. This explains why VSZ increased slightly after mapping, but RSS did not rise until the write occurred.

### Conclusion

Through this experiment, we clearly demonstrated how **memory mappings behave differently before and after access**, and how tools like `ps` and `/proc/<PID>/maps` help visualize these changes. The results matched expectations and provided hands-on insight into **virtual vs physical memory**, **lazy allocation**, and **demand paging** in Linux.

## Task 2.5 – `analyse.sh` (Automated Memory Analysis Script)

### How to Use:

This script helps you analyze the memory usage of a running process, especially for your `mmap_demo` program.

#### **1. Go to the folder where `analyse.sh` is saved**

Make sure your terminal is in the directory where `analyse.sh` is located.

```
cd /path/to/your/folder
```

#### **2. Make the script executable**

You only need to do this **once**:

```
chmod +x analyse.sh
```

#### **3. Run your mmap\_demo program in a separate terminal**

In **Terminal 1**, run your `mmap_demo` and **wait at the [Pause] prompt**. For example:

```
./mmap_demo
```

When you see something like:

```
[Pause] Hold up, check the /proc/%d/maps one more time — then smash that Enter like you mean it, yo..
```

**Do not press Enter yet.** Keep this terminal open.

#### **4. In another terminal (Terminal 2), run the script**

Use the command below to run the analysis script while `mmap_demo` is paused:

```
./analyse.sh $(pgrep mmap_demo)
```

This will:

* Display the PID, VSZ, RSS, and command of the running process.
* Show the memory mappings from `/proc/<PID>/maps`.

#### **5. Return to Terminal 1 and continue**

Go back to Terminal 1 and press Enter to proceed with the program when the script finishes showing the memory analysis.

At several moments in mmap\_demo, you can call this procedure to see memory changes, such as after `mmap `or after writing to memory.
