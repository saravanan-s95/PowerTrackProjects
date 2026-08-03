#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

#define FAILURE -1
#define SUCCESS 0

#define EXTERNAL 1
#define BUILTIN 3
#define NOCMD 4

extern char prompt[50];
extern char input[50];
extern int pid;

void scan_input(char* prompt,char* input);
char* get_cmd(char* input);
int check_cmd_type(char* cmd);

void execute_external_command(char *input);
void execute_internal_command(char *input);

void n_pipe(char *argv[]);

void my_handler(int signo);


typedef enum
{
    RUNNING,
    STOPPED
} job_state;

typedef struct node
{
    int stp_pid;
    char data[100];
    job_state state;
    struct node *link;
}Slist;

extern Slist *head;

void jobs();
void fg();
void bg();

void insert_first(int pid, char *cmd);
void delete_first();