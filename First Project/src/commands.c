#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // Untuk open()
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



// Fungsi untuk menyalin file menggunakan perintah cp
void execute_cp(char *source, char *destination) {
    // Mengecek apakah source dan destination tidak kosong
    if (source == NULL || destination == NULL) {
        printf("Source or destination file is missing.\n");
        return;
    }

    // Menjalankan perintah cp dengan execvp di dalam proses anak
    if (fork() == 0) {
        // Menyusun argumen untuk perintah cp
        char *args[] = {"cp", source, destination, NULL};  // Perintah cp diikuti dengan source dan destination
        execvp("cp", args);  // Menjalankan perintah cp
        perror("Error executing cp");  // Jika execvp gagal
        exit(1);
    } else {
        wait(NULL);  // Proses induk menunggu proses anak selesai
    }
}


// Fungsi untuk membuat file menggunakan perintah touch
void execute_touch(char *filename) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);  // 0644 adalah hak akses default untuk file
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    close(fd);  // Menutup file descriptor setelah file dibuat
    printf("File '%s' created successfully.\n", filename);  // Memberi konfirmasi file telah dibuat
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

// Fungsi untuk menjalankan perintah checkGuardian (menampilkan guardian acak)
void execute_checkGuardian() {
    // Array yang berisi daftar guardian dan deskripsinya
    const char *guardians[] = {
        "Goatikins – (A tiny goat spirit that thinks it's royalty and refuses to walk on grass.) Si Kambing Manja",
        "Quacknado – (A duck that spins like a tornado when angry, usually over stale bread.) Bebek Angin Puyuh",
        "Moozart – (A cow that composes symphonies by mooing in perfect pitch.) Sapi Jenius Musik",
        "Slothzilla – (A giant sloth that moves extremely slowly but causes earthquakes with every step.) Kukang Raksasa",
        "Sir Hopsalot – (A fancy rabbit who never stops hopping and always wears a monocle.) Tuan Lompat Terus",
        "Pawpaya – (A tropical fruit-loving cat who summons snacks instead of spirits.) Kucing Pecinta Pepaya",
        "Clucktopus – (An octopus with a chicken head that lays scrambled eggs underwater.) Ayam Gurita",
        "Oinksterious – (A mysterious pig who appears in dreams to give life advice... in riddles.) Babi Misterius",
        "Meowtini – (A stylish cat spirit who sips non-alcoholic drinks and judges your fashion sense.) Kucing Elegan Minum Mocktail",
        "Fartlebee – (A bumblebee that propels itself with tiny toots, often leaving a glitter trail.) Lebah Kentut"
    };

    // Mengambil indeks acak antara 0 dan 9
    int random_index = rand() % 10;

    // Menampilkan guardian yang dipilih
    printf("Your guardian/ Kodam Kamu:\n%s\n", guardians[random_index]);
}

// Fungsi untuk membuat file slist.txt berisi hasil perkalian
void execute_slist() {
    FILE *file = fopen("slist.txt", "w");  // Membuka file untuk menulis (akan membuat file baru jika tidak ada)

    if (file == NULL) {
        perror("Error opening file");  // Menampilkan pesan kesalahan jika file gagal dibuka
        return;
    }

    // Menulis hasil perkalian 1x1 sampai 100x100 ke dalam file
    for (int i = 1; i <= 100; i++) {
        fprintf(file, "%dx%d=%d\n", i, i, i * i);  // Menulis ke dalam file dalam format "1x1=1"
    }

    fclose(file);  // Menutup file setelah selesai menulis
    printf("File 'slist.txt' has been created successfully.\n");
}

