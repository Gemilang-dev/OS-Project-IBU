# OS Shell Project (Project Topic 2)

### Rayi Aqli Gemilang

### Isa Selimovic

## Project Stucture

MyShellProject/           # Main folder for the entire project
├── README.md             # Project documentation (usage instructions)
└── First Project/
    ├── bin/                  # Folder for executable binary files
    │   └── shell             # Compiled binary file of the shell
    ├── include/              # Folder for header files (.h)
    │   └── shell.h           # Header file containing function declarations
    ├── obj/                  # Folder for compiled object files
    │   ├── shell.o           # Compiled object file from shell.c
    │   ├── commands.o        # Compiled object file from commands.c
    │   └── utils.o           # Compiled object file from utils.c
    ├── src/                  # Folder for C source code files (.c)
    │   ├── shell.c           # Main implementation of the shell
    │   ├── commands.c        # Implementation of shell commands
    │   └── utils.c           # Utility functions for the shell
    ├── tests/                # Folder for test files
    │   ├── test1_1/          # Test folder for Task
    └── Makefile              # File for the build process (using make)

## Task 1.1: Shell Interface

### Objective:
The goal of **Task 1.1** is to create a basic shell interface that continuously prompts the user for input, executes commands, and returns to the prompt to ask for new input. The shell should display a prompt that indicates readiness to receive commands and execute them. The prompt initially shows in a **Basic format** (`prompt$`), and we later updated it to display in an **Advanced format** (`machinename@username:~$`).

### **Basic Prompt and Advanced Prompt**

- **Basic Prompt**: Displays only `prompt$` when waiting for user input.
- **Advanced Prompt**: Displays `machinename@username:~$`, where `machinename` is the name of the system and `username` is the active user’s name.

### **Functionality**
- The shell is designed to display a prompt and accept commands from the user.
- Users can switch between the **Basic Prompt** (`prompt$`) and **Advanced Prompt** (`machinename@username:~$`) by typing the command **`switch`**.

---

## How to Run the Shell

### Step 1: Compile the Program

To compile the shell, navigate to the project directory **`FirstProject/`** and run the following command:

```bash
make
```
This will compile the program and create the binary executable ./bin/shell.

Step 2: Run the Shell
Once compiled, you can run the shell with the following command:

```bash
./bin/shell
```
This will start the shell with the Advanced Prompt format by default.

## Switching Between Basic and Advanced Prompts

### Changing from Basic to Advanced Prompt and Vice Versa

- **Default Format**: When the shell starts, it displays the prompt in **Advanced format** by default. The prompt will look like:
```bash
machinename@username:~$
```
- **Switching to Basic Format**: To switch from **Advanced Prompt** to **Basic Prompt**, type the command:
```bash
switch
```
- **Switching Back to Advanced Format**: If you are currently in the **Basic Prompt**, you can switch back to the **Advanced Prompt** by typing the command:
```bash
prompt$
```

## List of Commands that can be Executed

Here is a list of basic commands that can be executed in the shell after implementing **Task 1.1**:

1. **`ls`**  
   - Lists the files and directories in the current working directory.
   - Example:
     ```bash
     ls
     ```

2. **`exit`**  
   - Exits the shell program and returns to the terminal.
   - Example:
     ```bash
     exit
     ```

3. **`pwd`**  
   - Displays the current working directory.
   - Example:
     ```bash
     pwd
     ```

4. **`cd`**  
   - Changes the current working directory.
   - Example:
     ```bash
     cd /path/to/directory
     ```

5. **`echo`**  
   - Prints the provided string to the console.
   - Example:
     ```bash
     echo "Hello, World!"
     ```

6. **`switch`**  
   - Switches between **Basic Prompt** and **Advanced Prompt**.
   - Example:
     ```bash
     switch
     ```
## Task 1.2: Shell Commands

### Objective:
The goal of **Task 2.2** is to implement several useful shell commands and integrate them into the previously created shell. These commands allow users to perform file management tasks, view system information, and interact with the shell more effectively. Below are the functions that have been implemented as part of this task.

### List of Commands Implemented:

1. **`history`**  
   - Displays the history of commands that have been previously executed by the user.
   - Example:
     ```bash
     history
     ```

2. **`free`**  
   - Displays system memory usage, including used, free, and swap memory.
   - Example:
     ```bash
     free
     ```

3. **`fortune`**  
   - Displays a random quote. Several predefined quotes are shown randomly when this command is executed.
   - Example:
     ```bash
     fortune
     ```

4. **`touch <filename>`**  
   - Creates a new file with the specified name. If the file already exists, this command will not modify it.
   - Example:
     ```bash
     touch newfile.txt
     ```

5. **`cp <source> <destination>`**  
   - Copies a file or directory from the source to the destination.
   - Example:
     ```bash
     cp testfile.txt copyfile.txt
     ```

6. **`slist`**  
   - Creates a file named `slist.txt` containing the multiplication table from 1x1=1 to 100x100=10000.
   - Example:
     ```bash
     slist
     ```

7. **`checkGuardian`**  
   - Displays a random guardian from a predefined list. The guardians are fictional characters with humorous descriptions.
   - Example:
     ```bash
     checkGuardian
     ```

---

## Task 1.3: System Calls Implementation
IT 204 Operating Systems
Shell Project - March 2025

### Overview:
In this task, we explore system calls in C programming. The system calls implemented are:

- fork()
- wait()
- exec()
- execlp()
- rfork()
- forkbomb (exploration)

These system calls are used in the context of a shell program to create processes, wait for processes to finish, and execute new programs.

### Implementation:
Below is the terminal output with embedded comments explaining each command and its corresponding system call:

#### Terminal Output 1:
```bash
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/First Project$ bin/shell
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/First Project$ ls
This is the parent process, waiting for the child to finish.
This is the child process.
total 28
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang  301 Apr 10 00:09 Makefile
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 bin
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 03:53 copyfile.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:32 include
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:12 obj
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang   23 Apr 10 05:15 output.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 src
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 04:32 testfile.txt
drwxr-xr-x 3 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 04:18 tests
```

#### Explanation:
This is the initial output of running the shell. It displays the contents of the directory after performing a fork() call, where the child process is created.
The parent process waits for the child to finish, as indicated by the message "This is the parent process, waiting for the child to finish."
The child process also outputs the message "This is the child process." This confirms the successful creation of a child process using fork().

#### Terminal Output 2:
```bash
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/First Project$ fork_test
This is the parent process, waiting for the child to finish.
This is the child process.
total 28
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang  301 Apr 10 00:09 Makefile
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 bin
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 03:53 copyfile.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:32 include
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:12 obj
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang   23 Apr 10 05:15 output.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 src
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 04:32 testfile.txt
drwxr-xr-x 3 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 04:18 tests
Child process finished.
```

#### Explanation:
This output shows the result of calling the fork_test program. The parent process waits for the child to finish (as shown by the "waiting for the child to finish" message).
After the child finishes its task, it prints the "Child process finished." message.

#### Terminal Output 3:
```bash
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/First Project$ echo "Hello, World!"
This is the parent process, waiting for the child to finish.
This is the child process.
total 28
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang  301 Apr 10 00:09 Makefile
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 bin
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 03:53 copyfile.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:32 include
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:12 obj
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang   23 Apr 10 05:15 output.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 src
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 04:32 testfile.txt
drwxr-xr-x 3 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 04:18 tests
Child process finished.
"Hello, World!"
```

#### Explanation:
This output shows the usage of the echo command within a process. The parent waits for the child process to finish.
The child process prints "Hello, World!" to indicate successful execution. This demonstrates how exec() could replace the current process image with a new program.

#### Terminal Output 4:
```bash
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/First Project$ execlp
This is the parent process, waiting for the child to finish.
This is the child process.
total 28
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang  301 Apr 10 00:09 Makefile
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 bin
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 03:53 copyfile.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:32 include
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:12 obj
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang   23 Apr 10 05:15 output.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 src
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 04:32 testfile.txt
drwxr-xr-x 3 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 04:18 tests
Child process finished.
```

#### Explanation:
The execlp() system call executes a new program within the child process. The parent process waits for the child process to finish.
After execution, the child finishes and the "Child process finished" message is displayed.

#### Terminal Output 5:
```bash
ayi_qli_emilang@INBOOKX2GEN11:~/OS-IBU/OS-Project-IBU/First Project$ rfork
This is the parent process, waiting for the child to finish.
This is the child process.
total 28
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang  301 Apr 10 00:09 Makefile
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 bin
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang    0 Apr 10 03:53 copyfile.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:32 include
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr  9 23:12 obj
-rw-r--r-- 1 ayi_qli_emilang ayi_qli_emilang   23 Apr 10 05:15 output.txt
drwxr-xr-x 2 ayi_qli_emilang ayi_qli_emilang 4096 Apr 10 05:36 src
```

#### Explanation:
The rfork system call is used to create a new thread-like process. It shares the address space between the parent and child processes, which helps in implementing a lightweight thread process.
