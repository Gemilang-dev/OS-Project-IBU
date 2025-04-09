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

