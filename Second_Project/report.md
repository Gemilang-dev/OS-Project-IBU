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
