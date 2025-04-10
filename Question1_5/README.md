
# Operating Systems Questions 1.5


-----------------------------------
Question 1.5.1: Kernel vs User Mode
-----------------------------------

### Program try to read infomation from disk:

Kernel = needed.

Why? System privileges are needed for the programme to perform input and output to the disk. That is, user mode programmes have to make system calls to the kernel which is careful about anything requiring physical access like disks.

### Reading the current time from the hardware clock:

Kernel = needed.

Why? The hardware clock is controlled to be accessed with only privileged commands which can be executed in kernel mode only. These are done for protective measures.

------------------------------
Question 1.5.2: System Calls
------------------------------

System calls are the gateway for application programmes to communicate with the operating system. App-level software modules cannot execute file and directory functions directly due to permission restrictions, but they can access them through system calls.

Types and Examples of System Calls:

1. **Process Control**
   - `fork()` : Starts a new child process.
   - `exit()` : Stops the currently running process.

2. **File Management**
   - `open()` : Opens a file for reading/writing.
   - `read()` : Reads data from a file.

3. **Device Management**
   - `close()` : Closes a file or device descriptor.

4. **Information Maintenance**
   - `getpid()` : Returns the process ID of the current process.
   - `times()` : Returns CPU time used or consumed.

5. **Communication**
   - `pipe()` : Generates a one-way communication pathway.
   - `msgsnd()` : Sends a message to a message queue.

---------------------------------------
Question 1.5.3: Fork + Exec + Wait Case
---------------------------------------

C Code:
 ```bash
int ret = fork();

if (ret == 0) {
    printf("Hello1\n");
    exec("some_executable");
    printf("Hello2\n");
}
else if (ret > 0) {
    wait();
    printf("Hello3\n");
}
else {
    printf("Hello4\n");
}
  ```

**Explanation of Possible Outputs:**

### Program Outline:

The child process is created using fork().

The first child process will print ‘Hello1’ and after that will attempt to execute a new programme with exec(). The parent process will execute the second block and are now waiting for the first child process to complete (prints ‘Hello3’).

If fork() fails, ‘Hello4’ is printed.

Potential Scenarios:

### Scenario 1: fork() succeeds and exec() succeeds:

Output:
 ```bash
Hello1

Hello3
 ```
The child process prints "Hello1", and subsequently calls exec() that replaces the child process’ memory with a new process therefore the child will not see ‘Hello2’ printed. The parent process will be printing ‘Hello3’ after waiting on the child process.

### Scenario 2: fork() succeeds and exec() fails:

Output:
 ```bash
Hello1

Hello2

Hello3
 ```
Now the child process prints ‘Hello1’, after this will attempt exec() but since exec doesn’t find an executable may (potentially) lead to the child process executing ‘Hello2’ next. Parent prints ‘Hello3’ upon completion of the Children process.

### Scenario 3: fork() fails:

Output:
 ```bash
Hello4
 ```
If fork() fails it returns -1 and leads into the else block where ‘Hello4’ is printed. No child process is created and action ceases from that point.

