#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

int use_advanced_prompt = 1;  // 1 untuk Advanced, 0 untuk Basic

// Fungsi untuk menangani perintah yang dimasukkan pengguna
void handle_command(char *cmd) {
    // Menambahkan perintah ke dalam riwayat history
    add_to_history(cmd);

    // Menangani perintah cd (change directory)
    if (strncmp(cmd, "cd", 2) == 0) {
        char *dir = strtok(cmd, " ");
        dir = strtok(NULL, " ");  // Mengambil direktori yang ingin dituju

        if (dir == NULL) {
            dir = getenv("HOME");  // Jika tidak ada argumen, kembali ke direktori home
        }

        if (chdir(dir) != 0) {
            perror("cd");  // Menampilkan error jika direktori tidak ditemukan
        }
    } 
    // Menangani perintah echo
    else if (strncmp(cmd, "echo", 4) == 0) {
        char *text = strtok(cmd, " ");  // Mengabaikan 'echo'
        text = strtok(NULL, "");        // Mendapatkan teks yang ingin ditampilkan
        if (text != NULL) {
            printf("%s\n", text);  // Menampilkan hasil echo
        }
    } 
    // Menangani perintah exit
    else if (strncmp(cmd, "exit", 4) == 0) {
        exit(0); // Keluar dari shell
    } 
    // Menangani perintah switch untuk berganti prompt format
    else if (strcmp(cmd, "switch") == 0) {
        use_advanced_prompt = !use_advanced_prompt;  // Toggle prompt type
        printf("Prompt switched to %s format.\n", use_advanced_prompt ? "Advanced" : "Basic");
    } 
    // Menangani perintah history
    // Menangani perintah history
    else if (strcmp(cmd, "history") == 0) {
        execute_history();
    } 
    // Menangani perintah free
    else if (strcmp(cmd, "free") == 0) {
        execute_free();
    } 
    // Menangani perintah fortune
    else if (strcmp(cmd, "fortune") == 0) {
        execute_fortune();
    } 
    // Menangani perintah cp untuk menyalin file/direktori
    else if (strncmp(cmd, "cp", 2) == 0) {
        char *source = strtok(cmd, " ");
        char *destination = strtok(NULL, " ");
        execute_cp(source, destination);  // Menjalankan perintah cp
    } 
    // Untuk perintah lainnya, menjalankan perintah menggunakan execvp
    else {
        pid_t pid = fork();
        if (pid == 0) {
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
    
    while (1) {
        char cwd[1024];  // Buffer untuk menyimpan path direktori saat ini
        getcwd(cwd, sizeof(cwd));  // Ambil current working directory
        
        // Mendapatkan nama pengguna dan nama mesin
        char *username = getenv("USER");
        char hostname[1024];
        gethostname(hostname, sizeof(hostname));

        // Mendapatkan direktori home
        char *home_dir = getenv("HOME");

        // Menampilkan prompt berdasarkan flag 'use_advanced_prompt'
        if (use_advanced_prompt) {
            // Format Advanced: machinename@username:~/subdir$
            if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
                // Jika direktori saat ini berada di dalam home, tampilkan ~
                printf("%s@%s:~%s$ ", username, hostname, cwd + strlen(home_dir));
            } else {
                // Menampilkan path lengkap jika berada di luar direktori home
                printf("%s@%s:%s$ ", username, hostname, cwd);
            }
        } else {
            // Format Basic: prompt$
            if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
                // Menampilkan ~ jika berada di dalam direktori home
                printf("prompt$ ~%s$ ", cwd + strlen(home_dir));  // Menampilkan path relatif dari home
            } else {
                // Menampilkan path lengkap jika berada di luar direktori home
                printf("prompt$ %s$ ", cwd);  // Menampilkan path lengkap
            }
        }

        // Membaca input pengguna
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

        // Menangani perintah yang dimasukkan
        handle_command(cmd);
    }

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int use_advanced_prompt = 1;  // 1 untuk Advanced, 0 untuk Basic

// // Fungsi untuk menangani perintah yang dimasukkan pengguna
// void handle_command(char *cmd) {
//     // Menangani perintah cd (change directory)
//     if (strncmp(cmd, "cd", 2) == 0) {
//         char *dir = strtok(cmd, " ");
//         dir = strtok(NULL, " ");  // Mengambil direktori yang ingin dituju

//         if (dir == NULL) {
//             dir = getenv("HOME");  // Jika tidak ada argumen, kembali ke direktori home
//         }

//         if (chdir(dir) != 0) {
//             perror("cd");  // Menampilkan error jika direktori tidak ditemukan
//         }
//     } 

//     // Menangani perintah history
//     else if (strcmp(cmd, "history") == 0) {
//     // Menambahkan perintah yang dijalankan ke history
//     add_to_history(cmd);  // Menambahkan perintah ke dalam history
//     execute_history();
//     }

    
//     // Menangani perintah echo
//     else if (strncmp(cmd, "echo", 4) == 0) {
//         char *text = strtok(cmd, " ");  // Mengabaikan 'echo'
//         text = strtok(NULL, "");        // Mendapatkan teks yang ingin ditampilkan
//         if (text != NULL) {
//             // Menampilkan teks tanpa tanda kutip
//             printf("%s\n", text);  
//         }
//     }
//     // Menangani perintah exit
//     else if (strncmp(cmd, "exit", 4) == 0) {
//         exit(0); // Keluar dari shell
//     } 
//     // Menangani perintah switch untuk berganti prompt format
//     else if (strcmp(cmd, "switch") == 0) {
//         use_advanced_prompt = !use_advanced_prompt;  // Toggle prompt type
//         printf("Prompt switched to %s format.\n", use_advanced_prompt ? "Advanced" : "Basic");
//     } 
//     // Untuk perintah lainnya, menjalankan perintah menggunakan execvp
//     else {
//         pid_t pid = fork();
//         if (pid == 0) {
//             char *args[] = {cmd, NULL};  // Menambahkan argumen jika perlu
//             execvp(cmd, args);  // Menjalankan perintah
//             exit(1);  // Jika execvp gagal
//         } else {
//             wait(NULL);  // Proses induk menunggu proses anak selesai
//         }
//     }
// }
// int main() {
//     char cmd[1024];
    
//     while (1) {
//         char cwd[1024];  // Buffer untuk menyimpan path direktori saat ini
//         getcwd(cwd, sizeof(cwd));  // Ambil current working directory
        
//         // Mendapatkan nama pengguna dan nama mesin
//         char *username = getenv("USER");
//         char hostname[1024];
//         gethostname(hostname, sizeof(hostname));

//         // Menampilkan prompt berdasarkan flag 'use_advanced_prompt'
//         if (use_advanced_prompt) {
//             // Format Advanced: machinename@username:~$ atau machinename@username:/path/to/dir$
//             char *home_dir = getenv("HOME");

//             // Menampilkan format advanced dengan path relatif atau ~ untuk direktori home
//             if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
//                 // Jika direktori saat ini berada di dalam home, tampilkan ~
//                 printf("%s@%s:~%s$ ", username, hostname, cwd + strlen(home_dir));
//             } else {
//                 // Menampilkan path lengkap
//                 printf("%s@%s:%s$ ", username, hostname, cwd);
//             }
//         } else {
//             // Format Basic: prompt$
//             printf("prompt$ ");
//         }

//         // Membaca input pengguna
//         fgets(cmd, sizeof(cmd), stdin);
//         cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

//         // Menangani perintah yang dimasukkan
//         handle_command(cmd);
//     }

//     return 0;
// }