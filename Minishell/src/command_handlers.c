#include "header.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int check_prompt_change() {
    if (strlen(input_string) > 4 && strncmp(input_string, "PS1", 3) == 0) {
        if (input_string[3] == '=' && input_string[4] != ' ')
            return 0;
        else
            printf("%serror in command\n%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    }
    return 1;
}

void copy_change() {
    char local_buffer[100];
    sprintf(local_buffer, "%s", input_string + 4);
    local_buffer[strcspn(local_buffer, " ")] = '\0';
    sprintf(prompt, "%s%s%s> %s", ANSI_COLOR_BLUE, local_buffer,
            ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
}

char *get_command() {
    char local_buffer[100];
    int char_count = strcspn(input_string, " \n");
    strncpy(local_buffer, input_string, char_count);
    local_buffer[char_count] = '\0';
    char *cmd = malloc((strlen(local_buffer) + 1) * sizeof(char));
    strcpy(cmd, local_buffer);
    return cmd;
}

void execute_internal_commands() {
    if (strcmp(input_string, "pwd") == 0) {
        char directory[200];
        getcwd(directory, sizeof(directory));
        printf("%s", directory);
    } else if (strncmp(input_string, "cd", 2) == 0 &&
               (input_string[2] == ' ' || input_string[2] == '\t')) {
        // strlen("cd ") is 3
        int path_ind = strspn(input_string + 3, " \t") + 3;
        if (chdir(input_string + path_ind) == -1) {
            perror(NULL);
            return;
        }
    } else if (strcmp(input_string, "exit") == 0) {
        exit(0);
    } else if (strncmp(input_string, "echo ", 5) == 0) {
        echo();
    } else if (strcmp(input_string, "jobs") == 0) {
        print_jobs(g_job_list);
    } else if (strcmp(input_string, "fg") == 0) {
        if (g_job_list == NULL) {
            printf("%sNo Process to resume\n%s", ANSI_COLOR_RED,
                   ANSI_COLOR_RESET);
        } else {
            int pid_to_resume = g_job_list->pid;
            printf("Resuming Process:\nProcess ID : %d | Process Name : %s\n",
                   g_job_list->pid, g_job_list->process_name);
            // resume process
            child_pid = pid_to_resume;
            kill(pid_to_resume, SIGCONT);
            // the process can be stopped again so WUNTRACED
            waitpid(pid_to_resume, &status, WUNTRACED);
            child_pid = 0;
            // if the process is terminated then remove job
            if (WIFSTOPPED(status) == 0)
                remove_job(&g_job_list);
        }
    } else if (strcmp(input_string, "bg") == 0) {
        if (g_job_list == NULL) {
            printf("%sNo Process to resume\n%s", ANSI_COLOR_RED,
                   ANSI_COLOR_RESET);
        } else {
            int pid_to_resume = g_job_list->pid;
            kill(pid_to_resume, SIGCONT);
            remove_job(&g_job_list);
        }
    } else {
        printf("%sCommand not found%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    }
}

void echo() {
    char *arg = input_string + 5;
    if (strcmp(arg, "$$") == 0) {
        printf("Process id -> %d", getpid());
    } else if (strcmp(arg, "$?") == 0) {
        printf("Exit status -> %d", WEXITSTATUS(status));
    } else if (strcmp(arg, "$SHELL") == 0) {
        printf("%s", getenv("SHELL"));
    } else {
        printf("%sInvalid argument%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    }
}

int execute_n_pipe(char *tokens[], int pipe_count, int *cmd_ind) {
    int fd[2];
    // prev pipe read
    int prev_pipe_rd = STDIN_FILENO;
    for (int i = 0; i <= pipe_count; i++) {
        // create command - 1 pipes
        if (i < pipe_count) {
            if (pipe(fd) == -1) {
                perror(NULL);
                return 1;
            }
        }
        int child = fork();
        if (child == -1) {
            perror(NULL);
            return 1;
        }
        if (child == 0) {
            // child process
            // for all children except first redirect stdin to prev pipe
            if (i > 0) {
                dup2(prev_pipe_rd, STDIN_FILENO);
                close(prev_pipe_rd);
            }
            // for all children except last close read end
            // redirect stdout to their write end
            if (i < pipe_count) {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]); // duplicate write end
            }
            execvp(tokens[cmd_ind[i]], tokens + cmd_ind[i]);
            perror(NULL);
            exit(1);
        } else {
            // parent process
            // for all process except first close prev_pipe_rd
            if (i > 0) {
                close(prev_pipe_rd);
            }
            // for all processes close the write end
            // and save the read end to prev_pipe_rd (baton)
            if (i < pipe_count) {
                close(fd[1]);
                prev_pipe_rd = fd[0];
            }
        }
    }
    for (int i = 0; i <= pipe_count; i++)
        wait(NULL);
    return 0;
}

void execute_external_commands() {
    char *tokens[100];
    int token_count;
    getwords(tokens, &token_count);
    int pipe_count = 0;
    int cmd_ind[100];
    setup_pipe_commands(tokens, &pipe_count, cmd_ind);
    // printf("pipe_count -> %d\n", pipe_count);
    if (pipe_count == 0) {
        if (execvp(tokens[0], tokens) == -1) {
            perror(NULL);
        }
    } else {
        if (execute_n_pipe(tokens, pipe_count, cmd_ind)) {
            printf("%sError in executing command%s", ANSI_COLOR_RED,
                   ANSI_COLOR_RESET);
        }
        // printf("%sInvalid argument%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    }
    // free tokens
    free_tokens(tokens, token_count);
}
