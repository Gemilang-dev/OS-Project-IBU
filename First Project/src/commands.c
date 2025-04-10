#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Untuk fork(), execvp(), wait()
#include <sys/wait.h>  // Untuk wait()
#include "shell.h"

#define HISTORY_SIZE 10

// Definisi variabel global
char *history[HISTORY_SIZE];
int history_index = 0;

// Fungsi untuk menambahkan perintah ke dalam riwayat history
void add_to_history(char *cmd) {
    // Memastikan perintah yang dimasukkan akan disalin dalam history[]
    history[history_index % HISTORY_SIZE] = strdup(cmd);  // Salin perintah ke dalam array history[]
    history_index++;
}

// Fungsi untuk menampilkan riwayat perintah
void execute_history() {
    printf("History Commend:\n");
    // Tampilkan semua perintah yang tersimpan di history[]
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (history[i] != NULL) {
            printf("%s\n", history[i]);  // Menampilkan setiap perintah yang ada di history[]
        }
    }
}

// Fungsi untuk menjalankan perintah free (menampilkan penggunaan memori)
void execute_free() {
    if (fork() == 0) {
        char *args[] = {"free", NULL};
        execvp("free", args);
        exit(1);  // Jika execvp gagal
    } else {
        wait(NULL);  // Proses induk menunggu proses anak
    }
}

// Fungsi untuk menyalin file/direktori (cp)
void execute_cp(char *source, char *destination) {
    if (fork() == 0) {
        char *args[] = {"cp", source, destination, NULL};
        execvp("cp", args);  // Menjalankan perintah cp
        exit(1);  // Jika execvp gagal
    } else {
        wait(NULL);  // Proses induk menunggu proses anak selesai
    }
}

// Fungsi untuk menjalankan perintah fortune (menampilkan kutipan acak)
void execute_fortune() {
    // Array yang berisi 3 kutipan
    const char *quotes[] = {
        "No pain no gain",
        "The best way out is always through",
        "The journey of a thousand miles begins with one step."
    };

    // Mengambil indeks acak antara 0 dan 2
    int random_index = rand() % 3;

    // Menampilkan kutipan acak
    printf("%s\n", quotes[random_index]);
}