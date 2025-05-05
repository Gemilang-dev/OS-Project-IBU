### Task 2.1

**📌 mmap**
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

**📌 munmap**
Function: Frees (unmaps) memory allocated by mmap.

```
munmap(ptr, 4096);
```

Parameter Explanation:


| Parameter | Description                                                                              |
| --------- | ---------------------------------------------------------------------------------------- |
| ptr       | The starting address of the memory to be freed (must be the same as the result of mmap). |
| 4096      | The size of the memory to be freed (must be the same as when allocated).                 |

✅ Important Notes

* mmap is useful when you want to manage memory manually, for example creating a replacement for malloc.
* After finishing using memory, always use munmap to prevent memory leaks.

### 2.2 Implemtation C Program (mmap\_demo.c)

**📌 Compile Program**

```
gcc mmap_demo.c -o mmap_demo
```

**📌 Run Program**

```
./mmap_demo
```

**📌 Result**

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

### Task 2.3: Memory Usage Analysis

in the code in the

In code section:

```
void print_memory_usage(const char *stage) {
char command[256];   
snprintf(command, sizeof(command),
"echo \"\\n[%s]\" && ps -o pid,vsz,rss,comm -p %d", // HERE
stage, getpid());  
system(command);        
}
```

`ps` is used to monitor memory usage during three stages of the `mmap_demo` program. So we get this result:

| Stage                  | VSZ (KB) | RSS (KB) |
|------------------------|----------|----------|
| Initial                | 2552     | 1688     |
| After mmap             | 2556     | 1688     |
| After writing to memory| 2556     | 1692     |

From the information about the experiment it can be seen that:

- VSZ has been increased by 4 KB simultaneously with calling mmap. This shows a new memory mapping was added to the process’s address space.
- The Physical memory usage (RSS) is however the same, which indicates the memory is not physically allocated yet.
- After writing to the allocated memory, there is an increase in RSS by 4 KB. This shows that Linux indeed does “lazy allocation”, providing actual physical pages only when they are accessed.
- Increase in RSS indicates a “page fault” that happened causing the OS to allocate a real physical page.
- Such behavior is observant in Linux anonymous memory mapping and shows further deferment to allocation until needed. Linux's free allocation techniques showcase inefficient memory management.


### Task 2.4: Explanation of Observations

#### a. Memory Usage (VSZ and RSS)

Below is a summary of memory usage in the three programming stages :

| Stage                  | VSZ (KB) | RSS (KB) |
|------------------------|----------|----------|
| Initial                | 2552     | 1688     |
| After mmap             | 2556     | 1688     |
| After writing to memory| 2556     | 1692     |

These results confirm that the call to `mmap()` added a new memory mapping to the process’s virtual address space, increasing VSZ by 4 KB. However, no additional physical memory (RSS) was used until the memory was accessed via writing.

#### b. /proc/<PID>/maps Explanation

Although the `/proc/<PID>/maps` file was not captured during execution, we can infer that after calling `mmap()`, a new entry appeared in this file. This entry would represent an **anonymous private memory mapping**, typically labeled as:

