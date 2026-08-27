#include "minishell.h"
extern int pid;
stop_p *head=NULL;
extern char mini[100];
extern char backup_input[100];
extern int pid2;
void signal_handler(int signum)
{
    if(signum == SIGINT)
    {
        if(pid==0)
        {
            //print the string
            printf("\n%s",mini);
            fflush(stdout);
        }
    }
    if(signum == SIGTSTP)
    {
        if(pid==0)
        {
            printf("\r%s",mini);
            fflush(stdout);
        }
        else if(pid>0)
        {
            stop_p *new = malloc(1*sizeof(stop_p));
            new->pid = pid;
            new->link = head;
            strcpy(new->name,backup_input);
            head = new;
        }
    }
    if(signum == SIGCHLD)
    {
        waitpid(-1,NULL,WNOHANG);
    }
}