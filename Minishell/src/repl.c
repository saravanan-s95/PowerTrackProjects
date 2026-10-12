#include "header.h"

const char *builtins[] = {
    "bg",      "bind",     "caller", "cd",     "declare", "dirs",    "echo",
    "eval",    "exec",     "exit",   "export", "fg",      "getopts", "hash",
    "help",    "jobs",     "let",    "popd",   "printf",  "pushd",   "pwd",
    "read",    "readonly", "set",    "shopt",  "source",  "true",    "type",
    "typeset", "unset",    NULL};

const char *cmdv[160];
int cmd_count;

int status;

void my_sigint_handler(int signum) {
    // only print prompt if there is no child process
    if (child_pid == 0) {
        printf("\n%s", prompt);
        fflush(stdout);
    }
}

void my_sigtstp_handler(int signum) {
    if (child_pid == 0) {
        printf("\n%s", prompt);
        fflush(stdout);
    }
}

void my_sigchld_handler(int signum) { waitpid(-1, &status, WNOHANG); }

int check_command_type(char *command) {
    // check builtins
    if (bin_search(builtins, sizeof(builtins) / sizeof(builtins[0]) - 1,
                   command) == 1)
        return BUILTIN;
    // check externals
    if (bin_search(cmdv, cmd_count, command) == 1)
        return EXTERNAL;
    // if none return non command
    return NO_COMMAND;
}

void scan_input() {
    extract_external_commands(cmdv, &cmd_count);
    signal(SIGINT, my_sigint_handler);
    signal(SIGTSTP, my_sigtstp_handler);
    signal(SIGCHLD, my_sigchld_handler);

    while (1) {
        printf("\n%s", prompt);
        // check if it is PS1
        // if it is copy_change prompt
        fgets(input_string, 100, stdin);
        input_string[strcspn(input_string, "\n")] = '\0';
        // Skip if input is empty or contains only spaces and tabs
        if (input_string[strspn(input_string, " \t")] == '\0')
            continue;
        if (check_prompt_change() == 0) {
            copy_change();
        }
        // if not get command
        else {
            char *command = get_command();
            // printf("cmd -> %s\n", command);
            // check for internal, external or no command
            char command_type = check_command_type(command);
            if (command_type == BUILTIN) {
                execute_internal_commands();
            } else if (command_type == EXTERNAL) {
                child_pid = fork();
                if (child_pid == -1) {
                    perror(NULL);
                } else if (child_pid == 0) {
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_commands();
                    exit(0);
                } else {
                    waitpid(child_pid, &status, WUNTRACED);
                    // record input_string and process id into job list
                    if (WIFSTOPPED(status)) {
                        if (insert_job(child_pid, input_string, &g_job_list) ==
                            1) {
                            printf("failed to insert job\n");
                            fflush(stdout);
                        }
                    }
                    // reset child_pid back to 0 for parent signal handling
                    child_pid = 0;
                }
            } else if (command_type == NO_COMMAND) {
                printf("%sNo such command found\n%s", ANSI_COLOR_RED,
                       ANSI_COLOR_RESET);
            }
            // printf("%d\n", command_type);
        }
    }
}
