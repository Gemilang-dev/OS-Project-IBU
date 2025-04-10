# OS Shell Project

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
## Task 2.2: Shell Commands

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