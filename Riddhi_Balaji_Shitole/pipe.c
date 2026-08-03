#include "main.h"

void n_pipe(char *argv[])
{
    int stdin_bkp = dup(0);
    int stdout_bkp = dup(1);

    int pipe_count = 0;

    for(int i = 0; argv[i] != NULL; i++)
    {
        if(strcmp(argv[i], "|") == 0)
            pipe_count++;
    }

    int fd[pipe_count][2];

    for(int i = 0; i < pipe_count; i++)
    {
        if(pipe(fd[i]) == -1)
        {
            perror("pipe");
            return;
        }
    }

    char **cmd[20];
    int cmd_count = 0;

    cmd[cmd_count++] = argv;

    for(int i = 0; argv[i] != NULL; i++)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            argv[i] = NULL;
            cmd[cmd_count++] = &argv[i + 1];
        }
    }

    for(int i = 0; i < cmd_count; i++)
    {
        pid_t pid = fork();

        if(pid < 0)
        {
            perror("fork");
            exit(1);
        }

        if(pid == 0)
        {
            if(i == 0)
            {
                dup2(fd[i][1], 1);
            }
            else if(i == cmd_count - 1)
            {
                dup2(fd[i - 1][0], 0);
            }
            else
            {
                dup2(fd[i - 1][0], 0);
                dup2(fd[i][1], 1);
            }

            for(int j = 0; j < pipe_count; j++)
            {
                close(fd[j][0]);
                close(fd[j][1]);
            }

            execvp(cmd[i][0], cmd[i]);

            perror("execvp");
            exit(1);
        }
    }

    for(int i = 0; i < pipe_count; i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    for(int i = 0; i < cmd_count; i++)
    {
        wait(NULL);
    }

    dup2(stdin_bkp, 0);
    dup2(stdout_bkp, 1);

    close(stdin_bkp);
    close(stdout_bkp);
}