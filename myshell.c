#include "myshell.h"

int main(int argc, char *argv[]) {
    char *command = NULL;
    char **args = NULL;
    FILE *batch_file = NULL;
    size_t len = 0;

    if (argc == 2) {
        // Open batch file
        batch_file = fopen(argv[1], "r");
        if (!batch_file) {
            perror("myshell: cannot open batch file");
            return 1;
        }

        while (getline(&command, &len, batch_file) != -1) {
            command[strcspn(command, "\n")] = 0;  // Strip newline
            printf(COLOR_GREEN "Batch> %s\n" COLOR_RESET, command);
            args = parse_command(command);
            execute_command(args);
            free_args(args);
        }

        fclose(batch_file);
        free(command);
    } else {
        // Interactive shell
        while ((command = read_command()) != NULL) {
            args = parse_command(command);
            execute_command(args);
            free_args(args);
            free(command);
        }
    }

    return 0;
}