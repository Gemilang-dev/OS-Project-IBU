#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Tambahkan untuk fork(), execvp(), wait()
#include <sys/wait.h>  // Tambahkan untuk wait()
#include "shell.h"

#define HISTORY_SIZE 10

char *history[HISTORY_SIZE];
int history_index = 0;

void add_to_history(char *cmd) {
    // Menambahkan perintah ke dalam history
    history[history_index % HISTORY_SIZE] = cmd;
    history_index++;
}

void execute_history() {
    printf("History perintah:\n");
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (history[i] != NULL) {
            printf("%s\n", history[i]);
        }
    }
}


void execute_free() {
    if (fork() == 0) {
        // Proses anak untuk menjalankan perintah free
        char *args[] = {"free", NULL};
        execvp("free", args);
        exit(1);  // Jika execvp gagal
    } else {
        wait(NULL);  // Proses induk menunggu proses anak
    }
}
