#include "myshell.h"

extern char **environ;

int original_stdin, original_stdout;
FILE *input_file = NULL, *output_file = NULL;

void redirect_io(char **args) {
    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);
    input_file = output_file = NULL;

    for (int i = 0; args[i]; ++i) {
        if (strcmp(args[i], "<") == 0) {
            input_file = fopen(args[i + 1], "r");
            if (!input_file) {
                fprintf(stderr, COLOR_RED "myshell: input error\n" COLOR_RESET);
                return;
            }
            dup2(fileno(input_file), STDIN_FILENO);
            for (int j = i; args[j]; ++j)
                args[j] = args[j + 2];
            i--;
        } else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
            const char *mode = (strcmp(args[i], ">>") == 0) ? "a" : "w";
            output_file = fopen(args[i + 1], mode);
            if (!output_file) {
                fprintf(stderr, COLOR_RED "myshell: output error\n" COLOR_RESET);
                return;
            }
            dup2(fileno(output_file), STDOUT_FILENO);
            for (int j = i; args[j]; ++j)
                args[j] = args[j + 2];
            i--;
        }
    }
}

void restore_io() {
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    if (input_file) fclose(input_file);
    if (output_file) fclose(output_file);
}

char *read_command() {
    char *line = NULL;
    size_t len = 0;
    printf(COLOR_BLUE "%s>" COLOR_RESET, getenv("PWD") );
    if (getline(&line, &len, stdin) == -1) {
        printf("\n");
        return NULL;
    }
    line[strcspn(line, "\n")] = 0;
    return line;
}

char **parse_command(char *line) {
    int bufsize = 64, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token = strtok(line, " \t\r\n");

    while (token) {
        tokens[pos++] = token;
        if (pos >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[pos] = NULL;
    return tokens;
}
void show_pwd () {
    fprintf(stdout, "PWD=%s\n", getenv("PWD"));
    return;
}

void execute_command(char **args) {
    if (!args[0]) return;

    int background = 0;
    for (int i = 0; args[i]; ++i) {
        if (strcmp(args[i], "&") == 0) {
            background = 1;
            args[i] = NULL;
            break;
        }
    }

    redirect_io(args);

    if (strcmp(args[0], "cd") == 0) {
        internal_cd(args);
    } else if (strcmp(args[0], "echo") == 0) {
        internal_echo(args);
    } else if (strcmp(args[0], "environ") == 0) {
        internal_environ();
    } else if (strcmp(args[0], "help") == 0) {
        internal_help();
    } else if (strcmp(args[0], "pause") == 0) {
        internal_pause();
    } else if (strcmp(args[0], "pwd") == 0) {
    show_pwd();
    }else if (strcmp(args[0], "quit") == 0) {
        internal_quit();
    } else if (strcmp(args[0], "clr") == 0) {
        internal_clr();
    } else if (strcmp(args[0], "dir") == 0) {
        internal_dir(args);
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            setenv("PATH", "/usr/bin:/bin:/usr/local/bin", 1);
            if (execvp(args[0], args) == -1) {
                fprintf(stderr, COLOR_RED "myshell: command failed\n" COLOR_RESET);
                exit(1);
            }
        } else if (pid > 0) {
            if (!background)
                waitpid(pid, NULL, 0);
        } else {
            perror("myshell");
        }
    }

    restore_io();
}

void internal_cd(char **args) {
    char path[MAX_COMMAND_LENGTH];

    if (input_file == NULL) {
        // Interactive mode
        if (args[1] == NULL) {
            printf("%s\n", getenv("PWD"));
        } else {
            if (chdir(args[1]) != 0)
                perror("cd");
        }
    } else {
        // Redirected input
        if (fgets(path, sizeof(path), stdin)) {
            path[strcspn(path, "\n")] = 0;
            if (strlen(path) == 0) {
                // Empty line in input file
                printf("%s\n", getenv("PWD"));
            } else {
                if (chdir(path) != 0)
                    perror("cd");
            }
        } else {
            // Nothing read (EOF) — empty file
            printf("%s\n", getenv("PWD"));
        }
    }

    // Always update PWD environment variable
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)))
        setenv("PWD", cwd, 1);
}

void internal_echo(char **args) {
    if (input_file == NULL) {
        for (int i = 1; args[i]; i++) {
            printf("%s", args[i]);
            if (args[i + 1]) printf(" ");
        }
        printf("\n");
    } else {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), stdin)) {
            fputs(buffer, stdout);
        }
    }
}

void internal_environ() {
    for (char **env = environ; *env; ++env) {
        printf(COLOR_GREEN "%s\n" COLOR_RESET, *env);
    }
}

void internal_help() {
    FILE *file = fopen("README.md", "r");
    if (!file) {
        perror("help");
        return;
    }
    pid_t pid = fork();
    if (pid == 0) {
        dup2(fileno(file), STDIN_FILENO);
        execlp("more", "more", NULL);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }
    fclose(file);
}

void internal_pause() {
    printf(COLOR_CYAN "Press Enter to continue..." COLOR_RESET);
    while (getchar() != '\n');
}

void internal_quit() {
    printf(COLOR_GREEN "Goodbye!\n" COLOR_RESET);
    exit(0);
}

void internal_dir(char **args) {
    DIR *dir;
    const char *path = (args[1]) ? args[1] : ".";
    struct dirent *entry;

    if (!(dir = opendir(path))) {
        perror("dir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf(COLOR_CYAN "%s\n" COLOR_RESET, entry->d_name);
    }

    closedir(dir);
}

void internal_clr() {
    system("clear");
}

void free_args(char **args) {
    free(args);
}
