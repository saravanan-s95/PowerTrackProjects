#if 0
Project Name:Mini Shell
Name: Shubham Maroti Jagtap
Date: 31/07/2026
description:Mini shell is a command processor, typically run in a text window, allowing the user to type commands which cause actions. 
Every Operating System provides this Command Line Interface (CLI) which takes commands from the user and provides required output. 
BASH can also read commands from a file, called a script. Like all Unix shells, it supports piping and variables as well. 
#endif




#include<stdio.h>                        
#include<string.h>                        
#include "minishell.h"                    

extern char *external_command[155];       // External commands list
extern int input[20];                     // Global input array
char mini[100]="minishell :-$";           // Default shell prompt
int index1;                               // Global index variable
int pid;                                  // Stores child process ID
char backup_input[100];                   // Backup of input command
char *builtins[] = {"echo","fg","bg", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","jobs" ,NULL}; // Built-in commands
int main()
{
    char *input_string[20];               // Stores tokenized input
    /// extract_external_command();
    system("clear");                      // Clear terminal
    extract_external_command();           // Load external commands
    signal(2,signal_handler);             // Register SIGINT handler
    signal(SIGTSTP,signal_handler);       // Register SIGTSTP handler
    signal(SIGCHLD,signal_handler);       // Register SIGCHLD handler
    while (1)
    {
        char str[100];                    // User input buffer
        printf("%s ",mini);               // Display shell prompt
        fflush(stdout);                   // Flush output
        scanf("%[^\n]",str);              // Read input until newline
       strcpy(backup_input,str);          // Backup user input
        __fpurge(stdin);                  // Clear input buffer
        if((strlen(str))==0)              // Ignore empty input
        {
            continue;
        }
        int i=0;
        if(str[0]=='P' && str[1]=='S' &&str[2]=='1' && str[3]=='=') // Check for PS1 assignment
        {
            if((strchr(str,' '))!=NULL)   // Validate prompt format
            {
                printf("Invalid\n");      // Print error
            }
            else
            {
                char *ptr = strchr(str,'='); // Find '=' character
                strcpy(mini,ptr+1);       // Update shell prompt
            }
        }
        convert_input_string(str,input_string); // Tokenize input
        int cmd = check_command(input_string[0]); // Identify command type
        if(cmd == EXTERNAL)
        {
            pid = fork();                 // Create child process
            if(pid==0)
            {
                signal(2,SIG_DFL);        // Restore default SIGINT
                signal(SIGTSTP,SIG_DFL);  // Restore default SIGTSTP
                signal(SIGCHLD,SIG_DFL);  // Restore default SIGCHLD
                execute_external_command(input_string); // Execute external command
                kill(getpid(),SIGINT);    // Terminate child if exec fails
                //exit(0);
            }
            else if(pid>0)
            {
                waitpid(pid,NULL,WUNTRACED); // Wait for child process
                //waitpid(-1,NULL,WCONTINUED);
                pid=0;                    // Reset PID
               //wait(NULL);
            }
        }
        else if(cmd==BUILTIN)
        {
            execute_internal_command(input_string); // Execute built-in command
        }
    }

}
int check_command(char *str)
{
    int i=0;
    while(external_command[i])            // Search external commands
    {
        int a = strcmp(external_command[i],str); // Compare command
        //printf("command: %s\n%s\n%d\n",external_command[i],str,a);
        if(a==0)
        {
            return EXTERNAL;              // External command found
        }
        i++;
    }
     i=0;
    while(builtins[i])                    // Search built-in commands
    {
        if((strcmp(builtins[i],str))==0)
        {
            return BUILTIN;               // Built-in command found
        }
        i++;
    }
    return NO_COMMAND;                    // Command not found
}