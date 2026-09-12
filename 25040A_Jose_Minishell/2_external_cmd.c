#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"

extern char* ext_cmd_arr[153];
extern char* builtins[];

extern char* input_commands[15];

int command_array[20];
int command_index = 0;

int exit_status = 0;

int pid = 0;


//Function to execute external commands using n-pipe logic
void execute_external_command(void)
{

    command_index = 0;                      //Reset for every start of execution 
    command_array[command_index] = 0;       //Storing the first command_index to array
    command_index++;


    int pipe_count = 0;
    for(int i=1; input_commands[i]!=NULL; i++)
    {

        if((strcmp(input_commands[i],"|")==0) && (strcmp(input_commands[i+1],"|")==0))      //Validation: whether two pipes come nearby
        {
            printf("Invalid: Two pipes together\n");
            return;
        }

        if(strcmp(input_commands[i],"|") == 0)
        {
            pipe_count++;                           //When a pipe is found in the argv, increment pipe_count

            if(input_commands[i+1] != NULL)
            {
                command_array[command_index] = i+1;     //When pipe is found in argv, update command_array with the next index
                command_index++;
            }
            input_commands[i] = NULL;
        }
    }

    int stdin_backup = dup(0);              //Backup for stdin

    int pipe_1[2];

    for(int i=0; i<command_index; i++)
    {
        if(i < (command_index-1))
        {
            if(pipe(pipe_1) == -1)      //Create pipe
            {
                perror("pipe");
                return;
            }
        }


        pid = fork();

        if(pid > 0)
        {
            //PARENT PROCESS

            int status;

            if(i < (command_index-1))
            {
                dup2(pipe_1[0], 0);
                close(pipe_1[0]);
                close(pipe_1[1]);
            }

            waitpid(pid, &status, WUNTRACED);   //Wait for child to finish or stopped by Ctrl+Z (WUNTRACED).
                                                
            if(WIFEXITED(status))
            {
                exit_status = WEXITSTATUS(status);      //If finished normally, store the exit code for 'echo $?'
            }

            pid = 0;            //Reset pid back to 0 after child terminates
        }

        else if(pid == 0)
        {
            //CHILD PROCESS

            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);

            if(i < (command_index-1))
            {
                dup2(pipe_1[1], 1);
                close(pipe_1[0]);
                close(pipe_1[1]);
            }
            execvp(input_commands[command_array[i]], input_commands+command_array[i]);      //Execute the commands
        }

    }

    dup2(stdin_backup, 0);      //Restoring stdin
    close(stdin_backup);

}


//Function to extract the external commands from text_file and store in 2D array format
void extract_external_command(void)
{
    FILE* fptr = fopen("external_cmd.txt", "r");
    if(fptr == NULL)
    {
        perror("fopen");
        return;
    }
    
    char buffer[24];
    int i = 0;
    for(i=0; i<152; i++)
    {
        fscanf(fptr, "%[^\n]", buffer);     
        fgetc(fptr);

        ext_cmd_arr[i] = malloc(strlen(buffer)+1);
        strcpy(ext_cmd_arr[i], buffer);
    }
    ext_cmd_arr[i] = NULL;

    // printf("The external commands printed from 2d array\n");
    // for(int i=0; ext_cmd_arr[i]!=NULL; i++)
    // {
    //     printf("%s\n", ext_cmd_arr[i]);
    // }

}