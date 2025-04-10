#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // For open()
#include <unistd.h>  // For fork(), execvp(), wait()
#include <sys/wait.h>  // For wait()
#include "shell.h"

#define HISTORY_SIZE 10

// Definition of global variables
char *history[HISTORY_SIZE];
int history_index = 0;

// Function to add a command to the history
void add_to_history(char *cmd) {
    // Ensure the entered command is copied into history[]
    history[history_index % HISTORY_SIZE] = strdup(cmd);  // Copy the command into the history[] array
    history_index++;
}

// Function to display the command history
void execute_history() {
    printf("History Command:\n");
    // Display all commands stored in history[]
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (history[i] != NULL) {
            printf("%s\n", history[i]);  // Display each command in history[]
        }
    }
}

// Function to execute the free command (displays memory usage)
void execute_free() {
    if (fork() == 0) {
        char *args[] = {"free", NULL};
        execvp("free", args);
        exit(1);  // Exit if execvp fails
    } else {
        wait(NULL);  // Parent process waits for child process to finish
    }
}

// Function to copy files using the cp command
void execute_cp(char *source, char *destination) {
    // Check if source and destination are not null
    if (source == NULL || destination == NULL) {
        printf("Source or destination file is missing.\n");
        return;
    }

    // Run the cp command with execvp in the child process
    if (fork() == 0) {
        // Prepare arguments for the cp command
        char *args[] = {"cp", source, destination, NULL};  // cp command followed by source and destination
        execvp("cp", args);  // Execute the cp command
        perror("Error executing cp");  // If execvp fails
        exit(1);
    } else {
        wait(NULL);  // Parent process waits for child process to finish
    }
}

// Function to create a file using the touch command
void execute_touch(char *filename) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);  // 0644 is the default file permission
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    close(fd);  // Close file descriptor after creating the file
    printf("File '%s' created successfully.\n", filename);  // Confirm that the file has been created
}

// Function to execute the fortune command (displays a random quote)
void execute_fortune() {
    // Array containing 3 quotes
    const char *quotes[] = {
        "No pain no gain",
        "The best way out is always through",
        "The journey of a thousand miles begins with one step."
    };

    // Get a random index between 0 and 2
    int random_index = rand() % 3;

    // Display the random quote
    printf("%s\n", quotes[random_index]);
}

// Function to execute the checkGuardian command (displays a random guardian)
void execute_checkGuardian() {
    // Array containing a list of guardians and their descriptions
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

    // Get a random index between 0 and 9
    int random_index = rand() % 10;

    // Display the chosen guardian
    printf("Your guardian/ Kodam Kamu:\n%s\n", guardians[random_index]);
}

// Function to create the slist.txt file containing multiplication results
void execute_slist() {
    FILE *file = fopen("slist.txt", "w");  // Open the file for writing (will create the file if it doesn't exist)

    if (file == NULL) {
        perror("Error opening file");  // Display error message if file can't be opened
        return;
    }

    // Write multiplication results from 1x1 to 100x100 into the file
    for (int i = 1; i <= 100; i++) {
        fprintf(file, "%dx%d=%d\n", i, i, i * i);  // Write in the format "1x1=1"
    }

    fclose(file);  // Close the file after writing
    printf("File 'slist.txt' has been created successfully.\n");
}

void execute_execlp() {
    pid_t pid = fork();  // Create a child process

    if (pid == 0) {
        // Child process
        execlp("ls", "ls", "-l", NULL);  // Run the ls -l command
        perror("execlp failed");  // If execlp fails
        exit(1);
    } else {
        wait(NULL);  // Parent process waits for child process to finish
    }
}

void execute_forkbomb() {
    while (1) {
        fork();  // Continuously create processes
    }
}
