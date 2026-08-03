#include "main.h"

void my_handler(int signo) 
{ 
    if(signo == SIGINT) 
    { 
        if(pid == 0) 
        { 
            printf("\n%s", prompt); 
            fflush(stdout); 
        } 
        else 
        {
            kill(pid, SIGINT); 
        } 
    } 
    else if(signo == SIGTSTP) 
    { 
        if(pid == 0) 
        { 
            printf("\n%s", prompt); 
            fflush(stdout); 
        } 
        else 
        {
            kill(pid, SIGTSTP); 
        } 
    } 
    else if(signo == SIGCHLD) 
    { 
        while(waitpid(-1, NULL, WNOHANG) > 0); 
    } 
}

void jobs()
{
    Slist *temp = head;
    int job = 1;

    while(temp)
    {
        printf("[%d] ", job++);

        if(temp->state == RUNNING)
        {
            printf("Running\t");
        }
        else
        {
            printf("Stopped\t");
        }

        printf("%s\n", temp->data);

        temp = temp->link;
    }
}

void fg()
{
    if(head == NULL)
    {
        printf("There is no stopped process\n");
        return;
    }

    pid = head->stp_pid;

    printf("%s\n", head->data);

    head->state = RUNNING;

    if(kill(pid, SIGCONT) == -1)
    {
        perror("kill");
        return;
    }

    int status;

    int ret = waitpid(pid, &status, WUNTRACED);

    printf("waitpid returned = %d\n", ret);

    if(ret == -1)
    {
        perror("waitpid");
        pid = 0;
        return;
    }

    if(WIFSTOPPED(status))
    {
        head->state = STOPPED;
    }
    else
    {
        delete_first();
    }

    pid = 0;
}

void bg()
{
    if(head == NULL)
    {
        printf("No jobs\n");
        return;
    }

    kill(head->stp_pid, SIGCONT);
    head->state = RUNNING;
}

void insert_first(int pid, char *cmd)
{
    Slist *new = malloc(sizeof(Slist));

    new->stp_pid = pid;
    strcpy(new->data, cmd);

    new->state = STOPPED;  

    new->link = head;
    head = new;
}

void delete_first()
{
    if(head == NULL)
        return;

    Slist *temp = head;
    head = head->link;
    free(temp);
}