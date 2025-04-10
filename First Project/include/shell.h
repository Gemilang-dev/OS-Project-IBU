#ifndef SHELL_H
#define SHELL_H

// Deklarasi variabel global
extern int use_advanced_prompt;  // Deklarasikan variabel use_advanced_prompt

// Deklarasi variabel history dan history_index
extern char *history[];
extern int history_index;

// Deklarasi fungsi-fungsi
void add_to_history(char *cmd);
void execute_history();
void execute_free();
void execute_cp(char *source, char *destination);
void execute_fortune();
void handle_command(char *cmd);  // Deklarasi fungsi handle_command

#endif
