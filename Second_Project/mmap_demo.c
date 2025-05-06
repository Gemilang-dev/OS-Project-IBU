#include <stdio.h>              // For printf and perror functions
#include <stdlib.h>             // For EXIT_SUCCESS and EXIT_FAILURE
#include <sys/mman.h>           // For mmap() and munmap() system calls
#include <unistd.h>             // For getpid() function
#include <string.h>             // For memset() function

#define SIZE 4096               // Define memory size to allocate (4KB)

// Function to display memory usage using `ps` command
void print_memory_usage(const char *stage) {
    char command[256];          // Buffer to store shell command
    snprintf(command, sizeof(command),
             "echo \"\\n[%s]\" && ps -o pid,vsz,rss,comm -p %d", // Format output
             stage, getpid());  // Insert current process ID
    system(command);            // Execute the shell command
}

int main() {
    void *addr;                 // Pointer to store address of allocated memory

    // Step 1: Print memory usage before any allocation
    print_memory_usage("Initial");

    // Step 2: Allocate 4KB of anonymous private memory using mmap
    addr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {  // Check if mmap failed
        perror("nah man. this mmap failed"); // Print error message
        return EXIT_FAILURE;
    }

    // Step 3: Print memory usage after mmap allocation
    print_memory_usage("After mmap");

    // ------------------- Additional code for 2.3 (Pause: Check /proc/<PID>/maps in another terminal now)
    printf("\n[Pause] Hold up, check the /proc/%d/maps one more time — then smash that Enter like you mean it, yo..\n", getpid());
    getchar();


    // Step 4: Write to the allocated memory to trigger actual physical allocation
    memset(addr, 42, SIZE);    // Fill memory with the value 42
    print_memory_usage("After writing to memory");

    // Pause again for another check
    printf("\n[Pause] Hold up, check the /proc/%d/maps one more time — then smash that Enter like you mean it, yo\n", getpid());
    getchar();

    // Step 5: Unmap the memory using munmap
    if (munmap(addr, SIZE) == -1) {   // Check if munmap failed
        perror("munmap failed bro");     // Print error message
        return EXIT_FAILURE;
    }

    // Confirm successful unmapping
    printf("\nMemory successfully unmapped shesssssssss!!!!.\n");

    return EXIT_SUCCESS;       // Exit program successfully
}
