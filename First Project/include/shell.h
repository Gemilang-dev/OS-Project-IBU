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
void execute_cp(char *source, char *destination);  // Deklarasikan fungsi execute_cp
void execute_fortune();
void execute_touch(char *filename);  // Deklarasikan fungsi execute_touch
void execute_checkGuardian();
void execute_slist();
void execute_redirect(char *cmd, char *output_file);  // Deklarasi fungsi execute_redirect
void execute_pipe(char *cmd1, char *cmd2);  // Deklarasi fungsi execute_pipe

#endif
