#include "minishell.h"
extern char *external_command[155];
extern char *input_string[20];
extern int input[20];
extern int index1;
extern int pid;
extern stop_p *head;
int pid2;

void execute_external_command(char *arr[])
{
    if(index1 == 0)                     // Execute single command
    {
        execvp(arr[input[0]],arr+(input[0]));
        return;
    }

    int fd[2];

    for(int i=0;i<=index1;i++)          // Handle pipeline commands
    {
        if(i != (index1))
        {
            int ret = pipe(fd);         // Create pipe
            if(ret==-1)
            {
                return;
            }
        }

        pid2 = fork();                  // Create child process

        if(pid2==0)
        {
            if(i != (index1))
            {
                dup2(fd[1],1);          // Redirect stdout
                close(fd[0]);
            }

            execvp(arr[input[i]],arr+(input[i])); // Execute command
        }
        else if(pid2>0)
        {
            wait(NULL);                 // Wait for child

            if(i != (index1))
            {
                dup2(fd[0],0);          // Redirect stdin
                close(fd[0]);
                close(fd[1]);
            }
        }
    }
}

void execute_internal_command(char *arr[])
{
    if((strcmp(arr[0],"exit"))==0)
    {
        exit(0);                        // Exit shell
    }
    else if((strcmp(arr[0],"cd"))==0)
    {
        chdir(arr[1]);                  // Change directory
    }
    else if((strcmp(arr[0],"pwd"))==0)
    {
        char str[200];
        printf("%s\n",getcwd(str,200)); // Print current directory
    }
    else if((strcmp(arr[0],"echo"))==0)
    {
        int i;

        if((strcmp(arr[1],"$SHELL"))==0)
        {
           char *ptr = getenv("SHELL"); // Get SHELL environment variable
           if(ptr==NULL)
           {
            printf("error\n");
           }else
            printf("%s\n",ptr);
        }
        else if((strcmp("$$",arr[1]))==0)
        {
            printf("%d\n",getpid());    // Print process ID
        }
        else if((strcmp("$?",arr[1]))==0)
        {
            waitpid(pid,&i,WNOHANG);    // Get exit status
            if (WIFEXITED(i))
            {
                printf("%d\n", WEXITSTATUS(i));
            }
        }
    }
    else if((strcmp(arr[0],"jobs"))==0)
    {
        stop_p *temp = head;
        while(temp != NULL)
        {
            printf("%d\t%s\n",temp->pid,temp->name);
            temp = temp->link;
        }
    }
    else if((strcmp(arr[0],"fg"))==0)
    {
        if(head !=NULL)
        {
            if(kill(head->pid,SIGCONT)== -1) // Resume foreground job
            {
                printf("ERROR\n");
            }
            else
            {
                waitpid(head->pid,NULL,WUNTRACED);
                stop_p *temp = head;
                head = head->link;
                free(temp);
            }
        }
    }
    else if((strcmp(arr[0],"bg"))==0)
    {
        if(head !=NULL)
        {
            if(kill(head->pid,SIGCONT)== -1) // Resume background job
            {
                printf("ERROR\n");
            }
            else
            {
                //waitpid(head->pid,NULL,WUNTRACED);
                stop_p *temp = head;
                head = head->link;
                free(temp);
            }
        }
    }
}