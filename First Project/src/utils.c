#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      // Tambahkan untuk fork(), dup2(), wait(), STDOUT_FILENO
#include <sys/wait.h>  // Tambahkan untuk wait()
#include <fcntl.h>       // Tambahkan untuk O_WRONLY, O_CREAT, open()
#include "shell.h"


void execute_with_redirection(char *cmd, char *file) {
    int fd = open(file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error membuka file");
        return;
    }

    if (fork() == 0) {
        // Redirect stdout ke file
        dup2(fd, STDOUT_FILENO);
        close(fd);

        // Menjalankan perintah
        char *args[] = {cmd, NULL};
        execvp(cmd, args);
        exit(1);  // Jika execvp gagal
    } else {
        wait(NULL);  // Proses induk menunggu proses anak
    }
}
