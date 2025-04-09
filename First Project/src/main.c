#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>



#define MAX_INPUT_LENGTH 100

void execute_command(char* command) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execlp(command, command, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char* command;

    while (1) {
        printf("prompt$ ");
        fgets(input, MAX_INPUT_LENGTH, stdin);

        // Menghapus karakter newline dari input
        input[strcspn(input, "\n")] = 0;

        // Memeriksa apakah input kosong
        if (strlen(input) == 0) {
            continue;
        }

        // Memeriksa apakah input adalah perintah "exit"
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Memisahkan perintah dan argumen
        command = strtok(input, " ");

        // Mengeksekusi perintah
        execute_command(command);
    }

    return 0;
}