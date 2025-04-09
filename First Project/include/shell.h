#ifndef SHELL_H
#define SHELL_H

// Deklarasi fungsi-fungsi yang digunakan di shell.c dan commands.c
void execute_command(char *cmd);
void add_to_history(char *cmd);
void execute_history();
void execute_free();
void execute_with_redirection(char *cmd, char *file);

#endif
