#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"
#include <fcntl.h>


int use_advanced_prompt = 1;  // 1 untuk Advanced, 0 untuk Basic

// Warna ANSI escape sequences
#define RESET_COLOR "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"


// ---- 1.4
// Fungsi untuk menampilkan prompt dengan warna dan nama
// Fungsi untuk menampilkan prompt dengan warna dan nama
void display_prompt() {
    char cmd[1024];
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
        // Format Advanced: machinename@username:~/subdir$ dengan warna
        if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
            // Jika direktori saat ini berada di dalam home, tampilkan ~
            printf("%s%s@%s:~%s$ %s", GREEN, username, hostname, cwd + strlen(home_dir), RESET_COLOR);  // Warna hijau untuk nama pengguna dan mesin
        } else {
            // Menampilkan path lengkap jika berada di luar direktori home
            printf("%s%s@%s:%s$ %s", BLUE, username, hostname, cwd, RESET_COLOR);  // Warna biru untuk nama pengguna dan mesin
        }
    } else {
        // Format Basic: prompt$ dengan warna
        if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
            // Menampilkan ~ jika berada di dalam direktori home
            printf("prompt$ %s~%s$ %s", YELLOW, cwd + strlen(home_dir), RESET_COLOR);  // Warna kuning untuk path relatif
        } else {
            // Menampilkan path lengkap jika berada di luar direktori home
            printf("prompt$ %s%s$ %s", YELLOW, cwd, RESET_COLOR);  // Warna kuning untuk path lengkap
        }
    }
}


void execute_redirect(char *cmd, char *output_file) {
    // Membuka file untuk menulis (akan membuat file jika tidak ada atau menimpa file yang ada)
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror("Error opening file for redirect");
        return;  // Menampilkan pesan kesalahan jika file gagal dibuka
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Menyalurkan output ke file
        if (dup2(fd, STDOUT_FILENO) == -1) {  // Mengalihkan output ke file
            perror("Error redirecting output");
            exit(1);
        }
        close(fd);  // Menutup file descriptor setelah digunakan

        // Pisahkan perintah dan argumen
        char *args[3];  // Maksimal 3 argumen: perintah, string pertama, string kedua
        args[0] = "echo";  // Perintah echo
        args[1] = cmd;  // Teks yang ingin dicetak
        args[2] = NULL;  // Argumen terakhir harus NULL

        execvp(args[0], args);  // Menjalankan perintah
        perror("Error executing command");  // Jika execvp gagal
        exit(1);
    } else {
        wait(NULL);  // Proses induk menunggu proses anak selesai
    }

    // Menambahkan notifikasi jika redirection berhasil
    printf("Output successfully redirected to '%s'.\n", output_file);
}
// Fungsi untuk menangani perintah yang dimasukkan pengguna
void handle_command(char *cmd) {
    // Menambahkan perintah ke dalam riwayat history
    add_to_history(cmd);

    pid_t pid = fork();  // Membuat proses anak

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Ini adalah kode yang dijalankan oleh proses anak
        printf("This is the child process.\n");

        // Menggunakan execvp untuk menjalankan perintah ls -l
        char *args[] = {"ls", "-l", NULL};  // Perintah yang akan dijalankan oleh anak
        execvp(args[0], args);  // Menjalankan perintah
        perror("execvp failed");  // Jika execvp gagal
        exit(1);
    } else {
        // Ini adalah kode yang dijalankan oleh proses induk
        printf("This is the parent process, waiting for the child to finish.\n");
        wait(NULL);  // Proses induk menunggu proses anak selesai
        printf("Child process finished.\n");
    }


    // Cek untuk redirection output ke file
    char *redirect_pos = strchr(cmd, '>');
    if (redirect_pos != NULL) {
        // Menangani redirection output
        *redirect_pos = '\0';  // Memotong perintah di tempat > ditemukan
        char *output_file = strtok(redirect_pos + 1, " ");  // Mendapatkan nama file
        execute_redirect(cmd, output_file);
        return;
    }

    // Cek untuk piping
    char *pipe_pos = strchr(cmd, '|');
    if (pipe_pos != NULL) {
        // Menangani piping
        *pipe_pos = '\0';  // Memotong perintah di tempat | ditemukan
        char *cmd1 = cmd;
        char *cmd2 = pipe_pos + 1;
        execute_pipe(cmd1, cmd2);
        return;
    }
    

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
    // Menangani perintah touch untuk membuat file baru
    else if (strncmp(cmd, "touch", 5) == 0) {
        // Mendapatkan nama file setelah perintah touch
        char *filename = strtok(cmd, " ");
        filename = strtok(NULL, " ");  // Mendapatkan nama file
        if (filename != NULL) {
            execute_touch(filename);  // Memanggil fungsi execute_touch untuk membuat file
        } else {
            printf("Error: Missing filename\n");
        }
    } 


    
    // Menangani perintah cp untuk menyalin file
    else if (strncmp(cmd, "cp", 2) == 0) {
        char *source = strtok(cmd, " ");
        char *destination = strtok(NULL, " ");
        if (source != NULL && destination != NULL) {
            execute_cp(source, destination);  // Menjalankan perintah cp
        } else {
            printf("Error: Missing source or destination\n");
        }
    } 


    //check guardian /check khodam
    else if (strcmp(cmd, "checkGuardian") == 0) {
        execute_checkGuardian();  // Menjalankan perintah checkGuardian
    }

    // Menangani perintah slist untuk membuat file perkalian
    else if (strcmp(cmd, "slist") == 0) {
        execute_slist();  // Menjalankan perintah slist
    }

    else if (strcmp(cmd, "forkbomb") == 0) {
        execute_forkbomb();  // Menjalankan forkbomb
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




// Fungsi untuk menangani piping
void execute_pipe(char *cmd1, char *cmd2) {
    int pipefd[2];
    pipe(pipefd);  // Membuat pipe

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Proses anak pertama (cmd1)
        close(pipefd[0]);  // Menutup read end dari pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Menyalurkan output ke pipe
        close(pipefd[1]);  // Menutup write end setelah digunakan

        char *args1[] = {cmd1, NULL};
        execvp(cmd1, args1);  // Menjalankan perintah pertama
        exit(1);
    } else {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Proses anak kedua (cmd2)
            close(pipefd[1]);  // Menutup write end dari pipe
            dup2(pipefd[0], STDIN_FILENO);  // Menyalurkan input dari pipe
            close(pipefd[0]);  // Menutup read end setelah digunakan

            char *args2[] = {cmd2, NULL};
            execvp(cmd2, args2);  // Menjalankan perintah kedua
            exit(1);
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            wait(NULL);  // Menunggu proses anak pertama dan kedua selesai
            wait(NULL);
        }
    }
}




int main() {
    char cmd[1024];
    
    while (1) {
        display_prompt();  // Menampilkan prompt dengan warna dan nama shell

        // Membaca input pengguna
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

        // Menangani perintah yang dimasukkan
        handle_command(cmd);
    }

    return 0;
}


// int main() {
//     char cmd[1024];
    
//     while (1) {

//         display_prompt();  // Menampilkan prompt dengan warna dan nama shell

//         char cwd[1024];  // Buffer untuk menyimpan path direktori saat ini
//         getcwd(cwd, sizeof(cwd));  // Ambil current working directory
        
//         // Mendapatkan nama pengguna dan nama mesin
//         char *username = getenv("USER");
//         char hostname[1024];
//         gethostname(hostname, sizeof(hostname));

//         // Mendapatkan direktori home
//         char *home_dir = getenv("HOME");

//         // Menampilkan prompt berdasarkan flag 'use_advanced_prompt'
//         if (use_advanced_prompt) {
//             // Format Advanced: machinename@username:~/subdir$
//             if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
//                 // Jika direktori saat ini berada di dalam home, tampilkan ~
//                 printf("%s@%s:~%s$ ", username, hostname, cwd + strlen(home_dir));
//             } else {
//                 // Menampilkan path lengkap jika berada di luar direktori home
//                 printf("%s@%s:%s$ ", username, hostname, cwd);
//             }
//         } else {
//             // Format Basic: prompt$
//             if (strncmp(cwd, home_dir, strlen(home_dir)) == 0) {
//                 // Menampilkan ~ jika berada di dalam direktori home
//                 printf("prompt$ ~%s$ ", cwd + strlen(home_dir));  // Menampilkan path relatif dari home
//             } else {
//                 // Menampilkan path lengkap jika berada di luar direktori home
//                 printf("prompt$ %s$ ", cwd);  // Menampilkan path lengkap
//             }
//         }

//         // Membaca input pengguna
//         fgets(cmd, sizeof(cmd), stdin);
//         cmd[strcspn(cmd, "\n")] = 0;  // Menghapus karakter newline

//         // Menangani perintah yang dimasukkan
//         handle_command(cmd);
//     }

//     return 0;
// }

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