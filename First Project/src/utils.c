#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      // Added for fork(), dup2(), wait(), STDOUT_FILENO
#include <sys/wait.h>  // Added for wait()
#include <fcntl.h>       // Added for O_WRONLY, O_CREAT, open()
#include "shell.h"


void execute_with_redirection(char *cmd, char *file) {
    int fd = open(file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    if (fork() == 0) {
        // Redirect stdout to the file
        dup2(fd, STDOUT_FILENO);
        close(fd);

        // Execute the command
        char *args[] = {cmd, NULL};
        execvp(cmd, args);
        exit(1);  // If execvp fails
    } else {
        wait(NULL);  // Parent process waits for the child process to finish
    }
}
