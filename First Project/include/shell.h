#ifndef SHELL_H
#define SHELL_H

// Declaration of global variables
extern int use_advanced_prompt;  // Declare the variable use_advanced_prompt

// Declaration of history and history_index variables
extern char *history[];
extern int history_index;

// Declaration of functions
void add_to_history(char *cmd);
void execute_history();
void execute_free();
void execute_cp(char *source, char *destination);  // Declare the execute_cp function
void execute_fortune();
void execute_touch(char *filename);  // Declare the execute_touch function
void execute_checkGuardian();
void execute_slist();
void execute_redirect(char *cmd, char *output_file);  // Declaration of the execute_redirect function
void execute_pipe(char *cmd1, char *cmd2);  // Declaration of the execute_pipe function
void execute_forkbomb();

#endif
