#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH 1024

// Color macros
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_CYAN    "\033[1;36m"

// Prototypes
char *read_command();
char **parse_command(char *command_line);
void execute_command(char **args);
void redirect_io(char **args);
void restore_io();
void internal_cd(char **args);
void internal_echo(char **args);
void internal_environ();
void internal_help();
void internal_pause();
void internal_quit();
void internal_dir(char **args);
void internal_clr();
void show_pwd();
void free_args(char **args);

#endif
