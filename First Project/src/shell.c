// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// // Variabel untuk menentukan jenis prompt
// int use_advanced_prompt = 1;  // 1 untuk Advanced, 0 untuk Basic

// void execute_command(char *cmd) {
//     pid_t pid = fork();
//     if (pid == 0) {
//         // Proses anak untuk menjalankan perintah
//         char *args[] = {cmd, NULL};  // Menambahkan argumen jika perlu
//         execvp(cmd, args);  // Menjalankan perintah
//         exit(1);  // Jika execvp gagal
//     } else {
//         wait(NULL);  // Proses induk menunggu proses anak selesai
//     }
// }

// int main() {
//     char cmd[1024];

//     while (1) {
//         // Menampilkan prompt berdasarkan flag 'use_advanced_prompt'
//         if (use_advanced_prompt) {
//             // Format Advanced: machinename@username:~$
//             char *username = getenv("USER");
//             char hostname[1024];
//             gethostname(hostname, sizeof(hostname));
//             printf("%s@%s:~$ ", username, hostname);  // Menampilkan prompt dengan nama pengguna dan nama mesin
//         } else {
//             // Format Basic: prompt$
//             printf("prompt$ ");
//         }

//         // Membaca input pengguna
//         fgets(cmd, sizeof(cmd), stdin);
//         cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

//         // Jika pengguna mengetik 'exit', keluar dari loop
//         if (strcmp(cmd, "exit") == 0) {
//             break;
//         }

//         // Jika pengguna mengetik 'switch', ganti antara basic dan advanced prompt
//         if (strcmp(cmd, "switch") == 0) {
//             use_advanced_prompt = !use_advanced_prompt;  // Toggle prompt type
//             printf("Prompt switched to %s format.\n", use_advanced_prompt ? "Advanced" : "Basic");
//         }

//         // Menjalankan perintah yang dimasukkan pengguna
//         execute_command(cmd);
//     }

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Variabel untuk menentukan jenis prompt
int use_advanced_prompt = 1;  // 1 untuk Advanced, 0 untuk Basic

// Fungsi untuk menangani perintah yang dimasukkan pengguna
void handle_command(char *cmd) {
    if (strncmp(cmd, "cd", 2) == 0) {
        // Memisahkan perintah cd dengan argumen direktori
        char *dir = strtok(cmd, " ");
        dir = strtok(NULL, " ");  // Mengambil direktori yang ingin dituju

        if (dir == NULL) {
            // Jika tidak ada argumen, kembali ke direktori home
            dir = getenv("HOME");
        }

        if (chdir(dir) != 0) {
            // Menampilkan error jika direktori tidak ditemukan
            perror("cd");
        }
    } else if (strncmp(cmd, "exit", 4) == 0) {
        exit(0); // Keluar dari shell
    } else if (strcmp(cmd, "switch") == 0) {
        // Jika pengguna mengetik 'switch', ganti antara basic dan advanced prompt
        use_advanced_prompt = !use_advanced_prompt;  // Toggle prompt type
        printf("Prompt switched to %s format.\n", use_advanced_prompt ? "Advanced" : "Basic");
    } else {
        // Menjalankan perintah lain
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
}

int main() {
    char cmd[1024];
    char cwd[1024];  // Buffer untuk menyimpan path direktori saat ini

    while (1) {
        // Mendapatkan direktori saat ini
        getcwd(cwd, sizeof(cwd));  // Ambil current working directory

        // Menampilkan prompt berdasarkan flag 'use_advanced_prompt'
        if (use_advanced_prompt) {
            // Format Advanced: machinename@username:~$ /current/dir
            char *username = getenv("USER");
            char hostname[1024];
            gethostname(hostname, sizeof(hostname));
            printf("%s@%s:%s$ ", username, hostname, cwd);  // Menampilkan prompt dengan nama pengguna, nama mesin, dan direktori saat ini
        } else {
            // Format Basic: prompt$ /current/dir
            printf("prompt$ %s$ ", cwd);  // Menampilkan prompt basic dan direktori saat ini
        }

        // Membaca input pengguna
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

        // Menangani perintah yang dimasukkan
        handle_command(cmd);
    }

    return 0;
}
