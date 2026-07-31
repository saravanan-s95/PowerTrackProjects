#ifndef MINISHELL
#define MINISHELL


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
typedef struct stop_p
{
    int pid;
    char name[100];
    struct stop_p  *link;
    

}stop_p;

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3
void execute_external_command(char *arr[]);
int check_command(char *str);
void extract_external_command();
void convert_input_string(char *,char *input_string[]);
void signal_handler(int signum);
void execute_internal_command(char *arr[]);
#endif
