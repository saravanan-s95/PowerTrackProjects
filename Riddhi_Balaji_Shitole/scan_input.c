#include "main.h"

char shell_path[100];
int status = 0;

void scan_input(char* prompt,char* input)
{
    getcwd(shell_path, sizeof(shell_path));

    while (1)
    {
        printf("%s", prompt);

        scanf("%[^\n]",input);
        if(strncmp(input, "PS1=", 4) == 0)
        {
            if(input[4] != ' ')
            {
                strcpy(prompt , &input[4]);
            }
            else
            {
                printf("PS1: command not found\n");
            }
        }

       char* cmd = get_cmd(input);

      int check_cmd_type_ret = check_cmd_type(cmd);

        if(check_cmd_type_ret == EXTERNAL)
        {
            pid = fork();

            if(pid < 0)
            {
                perror("fork");
            }
            else if(pid == 0)
            {
                execute_external_command(input);
            }
            else
            {
                waitpid(pid, &status, WUNTRACED);
                if(WIFSTOPPED(status))
                {
                    printf("\n[%d]+ Stopped\t%s\n", pid, input);
                    insert_first(pid, input);
                }
                pid = 0;
            }
        }
        else if(check_cmd_type_ret == BUILTIN)
        {
            execute_internal_command(input);
        }
        else if(strcmp(input,"jobs") == 0)
        {
            jobs();
        }
        else if(strcmp(input,"fg") == 0)
        {
            fg();
        }
        else if(strcmp(input,"bg") == 0)
        {
            bg();
        }
        else
        {
            printf("%s: command not found\n", cmd);
        }

        getchar();
    }
}

int check_cmd_type(char *cmd)
{
    int fd;
    int i, ret;
    char ch;
    char command[50];

    fd = open("bultin_cmd.txt", O_RDONLY);
    if(fd != -1)
    {
        while(1)
        {
            i = 0;

            while((ret = read(fd, &ch, 1)) > 0)
            {
                if(ch != '\n' && ch != '\r') 
                {
                    command[i++] = ch;
                }
                else
                {
                    break;
                }
            }

            command[i] = '\0';

            if(strcmp(command, cmd) == 0)
            {
                close(fd);
                return BUILTIN;
            }

            if(ret == 0)
            {
                break;
            }
        }

        close(fd);
    }

    fd = open("External_cmd.txt", O_RDONLY);
    if(fd == -1)
        return FAILURE;

    while(1)
    {
        i = 0;

        while((ret = read(fd, &ch, 1)) > 0)
        {
            if(ch != '\n' && ch != '\r') 
            {
                command[i++] = ch;
            }
            else
            {
                break;
            }
        }

        command[i] = '\0';

        if(strcmp(command, cmd) == 0)
        {
            close(fd);
            return EXTERNAL;
        }

        if(ret == 0)
        {
            break;
        }
    }

    close(fd);
    return FAILURE;
}
char* get_cmd(char* input)
{
     int i = 0;
    static char cmd [15];
     while(input[i] != ' ' && input[i] != '\0')
     {
        cmd[i] = input[i];
        i++;
     }
     cmd[i]= '\0';
     return cmd;
}
void execute_external_command(char *input_string)
{
    char *argv[20];
    char str[100];

    int i = 0;
    int j = 0;
    int count = 0;

    while(input_string[i] != '\0')
    {
        if(input_string[i] != ' ')
        {
            str[j++] = input_string[i];
        }
        else
        {
            str[j] = '\0';

            argv[count] = malloc(strlen(str) + 1);
            strcpy(argv[count], str);

            count++;
            j = 0;
        }

        i++;
    }


    str[j] = '\0';

    argv[count] = malloc(strlen(str) + 1);
    strcpy(argv[count], str);

    count++;
    argv[count] = '\0';
    i = 0;
    while(argv[i])
    {
        if(strcmp(argv[i], "|") == 0)
        {
            n_pipe(argv);
            return;
        }
        i++;
    } 

    execvp(argv[0], argv);

    perror("execvp");
    exit(1);

}

void execute_internal_command(char *input)
{
    if(strcmp(input, "exit") == 0)
    {
        exit(0);
    }
    else if(strcmp(input, "pwd") == 0)
    {
        char buf[100];

        getcwd(buf, sizeof(buf));
        printf("%s\n", buf);
    }
    else if(strncmp(input, "cd ", 3) == 0)
    {
        chdir(input + 3);
    }
    else if(strncmp(input, "echo ", 5) == 0)
    {
        printf("%s\n", input + 5);
    }
    else if(strcmp(input, "echo $$") == 0)
    {
        printf("%d\n", getpid());
    }
    else if(strcmp(input, "echo $?") == 0)
    {
        printf("%d\n", status);
    }
    else if(strcmp(input, "echo $SHELL") == 0)
    {
        printf("%s\n", shell_path);
    }
    else if(strcmp(input, "clear") == 0)
    {
        printf("\033[H\033[J");
        fflush(stdout);
    }
}
