#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Variabel untuk menentukan jenis prompt
int use_advanced_prompt = 1;  // 1 untuk Advanced, 0 untuk Basic

void execute_command(char *cmd) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proses anak untuk menjalankan perintah
        char *args[] = {cmd, NULL};  // Menambahkan argumen jika perlu
        execvp(cmd, args);  // Menjalankan perintah
        exit(1);  // Jika execvp gagal
    } else {
        wait(NULL);  // Proses induk menunggu proses anak selesai
    }
}

int main() {
    char cmd[1024];

    while (1) {
        // Menampilkan prompt berdasarkan flag 'use_advanced_prompt'
        if (use_advanced_prompt) {
            // Format Advanced: machinename@username:~$
            char *username = getenv("USER");
            char hostname[1024];
            gethostname(hostname, sizeof(hostname));
            printf("%s@%s:~$ ", username, hostname);  // Menampilkan prompt dengan nama pengguna dan nama mesin
        } else {
            // Format Basic: prompt$
            printf("prompt$ ");
        }

        // Membaca input pengguna
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

        // Jika pengguna mengetik 'exit', keluar dari loop
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Jika pengguna mengetik 'switch', ganti antara basic dan advanced prompt
        if (strcmp(cmd, "switch") == 0) {
            use_advanced_prompt = !use_advanced_prompt;  // Toggle prompt type
            printf("Prompt switched to %s format.\n", use_advanced_prompt ? "Advanced" : "Basic");
        }

        // Menjalankan perintah yang dimasukkan pengguna
        execute_command(cmd);
    }

    return 0;
}
