#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"
#include <fcntl.h>


int use_advanced_prompt = 1;  // 1 for Advanced, 0 for Basic

// ANSI escape sequences for colors
#define RESET_COLOR "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"


// ---- 1.4
// Function to display the prompt with color and name
void display_prompt() {
    char cmd[1024];
    char cwd[1024];  // Buffer to store the current working directory path
    getcwd(cwd, sizeof(cwd));  // Get current working directory

    // Get the username and machine name
    char *username = getenv("USER");
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    // Get the home directory
    char *home_dir = getenv("HOME");

    // Show the prompt based on 'use_advanced_prompt' flag
    if (use_advanced_prompt) {
        // Advanced format: machinename@username:~/subdir$ with color
        if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
            // If current directory is inside home, show ~
            printf("%s%s@%s:~%s$ %s", GREEN, username, hostname, cwd + strlen(home_dir), RESET_COLOR);  // Green color for username and hostname
        } else {
            // Show full path if outside home directory
            printf("%s%s@%s:%s$ %s", BLUE, username, hostname, cwd, RESET_COLOR);  // Blue color for username and hostname
        }
    } else {
        // Basic format: prompt$ with color
        if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
            // Show ~ if inside home directory
            printf("prompt$ %s~%s$ %s", YELLOW, cwd + strlen(home_dir), RESET_COLOR);  // Yellow color for relative path
        } else {
            // Show full path if outside home directory
            printf("prompt$ %s%s$ %s", YELLOW, cwd, RESET_COLOR);  // Yellow color for full path
        }
    }
}


void execute_redirect(char *cmd, char *output_file) {
    // Open file for writing (will create the file if it doesn't exist or overwrite if it does)
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror("Error opening file for redirect");
        return;  // Show error message if file fails to open
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Redirect output to file
        if (dup2(fd, STDOUT_FILENO) == -1) {  // Redirect output to file
            perror("Error redirecting output");
            exit(1);
        }
        close(fd);  // Close file descriptor after use

        // Split command and arguments
        char *args[3];  // Maximum 3 arguments: command, first string, second string
        args[0] = "echo";  // echo command
        args[1] = cmd;     // Text to print
        args[2] = NULL;    // Last argument must be NULL

        execvp(args[0], args);  // Execute the command
        perror("Error executing command");  // If execvp fails
        exit(1);
    } else {
        wait(NULL);  // Parent waits for child process to finish
    }

    // Add a notification if redirection is successful
    printf("Output successfully redirected to '%s'.\n", output_file);
}

//function to handle command
void handle_command(char *cmd) {
    char *redirect_symbol = strchr(cmd, '>');  // Look for redirection symbol (>)
    if (redirect_symbol != NULL) {
        *redirect_symbol = '\0';  // Split string at '>', so we get command and file name
        redirect_symbol++;        // Move to the filename
        while (*redirect_symbol == ' ') redirect_symbol++;  // Skip spaces
        char *output_file = redirect_symbol;
        execute_redirect(cmd, output_file);  // Execute redirection logic
    } else {
        // Handle basic internal commands
        if (strcmp(cmd, "exit") == 0) {
            exit(0);  // Exit the shell
        } else if (strcmp(cmd, "basic") == 0) {
            use_advanced_prompt = 0;
        } else if (strcmp(cmd, "advance") == 0) {
            use_advanced_prompt = 1;
        } else if (strncmp(cmd, "cd ", 3) == 0) {
            // Change directory
            char *path = cmd + 3;
            if (chdir(path) != 0) {
                perror("cd error");  // Print error if directory doesn't exist
            }
        }
        else if (strcmp(cmd, "slist") == 0) {
            execute_slist();
            add_to_history(cmd);
        }
        else if (strcmp(cmd, "history") == 0) {
            execute_history();
            add_to_history(cmd);
        }
        else if (strcmp(cmd, "free") == 0) {
            execute_free();
            add_to_history(cmd);
        }
        else if (strcmp(cmd, "fortune") == 0) {
            execute_fortune();
            add_to_history(cmd);
        }
        else if (strcmp(cmd, "checkGuardian") == 0) {
            execute_checkGuardian();
            add_to_history(cmd);
        }
        else if (strcmp(cmd, "forkbomb") == 0) {
            execute_forkbomb();
            add_to_history(cmd);
        }

        else if (strncmp(cmd, "touch", 5) == 0) {
            // Mendapatkan nama file setelah perintah touch
            char *filename = strtok(cmd, " ");
            filename = strtok(NULL, " ");  // Mendapatkan nama file
            if (filename != NULL) {
                execute_touch(filename);  // Memanggil fungsi execute_touch untuk membuat file
            } else {
                printf("Error: Missing filename\n");
            }
            add_to_history(cmd);
        } 

        else if (strncmp(cmd, "cp", 2) == 0) {
            // Memotong perintah untuk mendapatkan argumen sumber dan tujuan
            char *source = strtok(cmd, " ");       // Memotong bagian pertama (cp)
            source = strtok(NULL, " ");            // Memotong sumber file
            char *destination = strtok(NULL, " "); // Memotong tujuan file
            
            // Jika kedua argumen ada (source dan destination), panggil fungsi execute_cp
            if (source != NULL && destination != NULL) {
                execute_cp(source, destination);  // Menyalin file
            } else {
                // Jika salah satu atau kedua argumen hilang
                printf("Usage: cp <source> <destination>\n");
            }
    
            add_to_history(cmd);  // Menambahkan perintah ke riwayat
        }
        else {
            // Handle other system commands using fork and exec
            pid_t pid = fork();
            if (pid == 0) {
                // Proses anak (child process)
                printf("This is the child process.\n");
                fflush(stdout);  // Memastikan bahwa stdout di-flush dan tampil di terminal
        
                // Pisahkan perintah dan argumen
                char *args[64];  // Buffer untuk command arguments
                int i = 0;
                args[i] = strtok(cmd, " ");  // Split by space
                while (args[i] != NULL) {
                    args[++i] = strtok(NULL, " ");
                }
                args[i] = NULL;  // End of arguments for execvp
        
                // Menjalankan perintah menggunakan execvp
                execvp(args[0], args);
                
                // Jika execvp gagal, tampilkan error
                perror("execvp failed");
                exit(1);  // Exit child process if execvp fails
            } else if (pid > 0) {
                // Proses induk (parent process)
                printf("This is the parent process, waiting for the child to finish.\n");
                wait(NULL);  // Wait for the child process to finish
                printf("Child process finished.\n");
            } else {
                // Jika fork gagal
                perror("Fork failed");
                exit(1);
            }
        }
    }
}



int main() {
    char input[1024];  // Buffer to store user input

    while (1) {
        display_prompt();  // Show the prompt (basic or advanced)
        fgets(input, sizeof(input), stdin);  // Read input from user
        input[strcspn(input, "\n")] = '\0';  // Remove newline character from input

        // If user entered something (not just pressed enter), handle the command
        if (strlen(input) > 0) {
            handle_command(input);
        }
    }

    return 0;
}


// Main function before Coloring
// int main() {
//     char cmd[1024];  // Buffer to store user input
    
//     while (1) {

//         char cwd[1024];  // Buffer to store the current working directory path
//         getcwd(cwd, sizeof(cwd));  // Get the current working directory
        
//         // Retrieve username and hostname
//         char *username = getenv("USER");
//         char hostname[1024];
//         gethostname(hostname, sizeof(hostname));

//         // Retrieve home directory path
//         char *home_dir = getenv("HOME");

//         // Display the prompt depending on the 'use_advanced_prompt' flag
//         if (use_advanced_prompt) {
//             // Advanced format: machinename@username:~/subdir$
//             if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
//                 // If inside the home directory, replace the path with ~
//                 printf("%s@%s:~%s$ ", username, hostname, cwd + strlen(home_dir));
//             } else {
//                 // Show full path if outside home directory
//                 printf("%s@%s:%s$ ", username, hostname, cwd);
//             }
//         } else {
//             // Basic format: prompt$
//             if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
//                 // Show relative path from home using ~
//                 printf("prompt$ ~%s$ ", cwd + strlen(home_dir));
//             } else {
//                 // Show full path if outside home directory
//                 printf("prompt$ %s$ ", cwd);
//             }
//         }

//         // Read user input
//         fgets(cmd, sizeof(cmd), stdin);
//         cmd[strcspn(cmd, "\n")] = 0;  // Remove newline character from input

//         // Handle the entered command
//         handle_command(cmd);
//     }

//     return 0;
// }
