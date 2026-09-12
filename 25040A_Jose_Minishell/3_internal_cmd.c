#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"

extern char* input_commands[15];
extern int exit_status;
extern store_job_data* head;
extern int pid;
extern char prompt_string[100];


//Function to execute innternal_commands
void execute_internal_command(void)
{
    if(strcmp(input_commands[0], "exit") == 0)  
    {
        exit(0);    
    }
    else if(strcmp(input_commands[0], "cd") == 0)
    {
        execute_cd_command();
    }
    else if(strcmp(input_commands[0], "pwd") == 0)
    {
        execute_pwd_command();
    }
    else if(strcmp(input_commands[0], "echo") == 0)
    {
        execute_echo_command();
    }
    else if(strcmp(input_commands[0], "jobs") == 0)
    {
        execute_jobs_command();
    }
    else if(strcmp(input_commands[0], "fg") == 0)
    {
        execute_fg_command();
    }
    else if(strcmp(input_commands[0], "bg") == 0)
    {
        execute_bg_command();
    }

    return;
}


//Function to execute "cd" command
void execute_cd_command(void)
{
    char* path = NULL;

    if(input_commands[1] == NULL)       //If path not specified, go to home directory
    {
        path = getenv("HOME");
    }
    else                                //If path specified, go to the specified directory
    {
        path = input_commands[1];   
    }

    int ret = chdir(path);
    if(ret != 0)
    {
        perror("cd");
    }
}


//Function to execute "pwd" command
void execute_pwd_command(void)
{
    char pwd_path[100];
    getcwd(pwd_path, 100);              //Get the path of present_working_diretory using "getcwd" fn
    printf("%s\n", pwd_path);
}


//Function to execute "echo" command
void execute_echo_command(void)
{
    if(strcmp(input_commands[1], "$SHELL") == 0)        //Case of "echo $SHELL"
    {
        printf("%s\n", getenv("SHELL"));
    }
    else if(strcmp(input_commands[1], "$$") == 0)       //Case of "echo $$"
    {
        printf("%d\n", getpid());                       //Prints the pid of current process/shell
    }
    else if(strcmp(input_commands[1], "$?") == 0)       //Case of "echo $?"
    {
        printf("%d\n", exit_status);                    //Prints the exit status of last-terminated process
    }
}


//Function to execute "jobs" command
void execute_jobs_command(void)
{
    store_job_data* temp = head;

    while(temp != NULL)         //Print all the processes stored in the linked-list
    {
        printf("[%d]\t", temp->pid);
        
        for(int i=0; temp->commands[i]!= NULL; i++)
        {
            printf("%s ", temp->commands[i]);
        }
        printf("\n");

        temp = temp->next;
    }
}


//Signal_handler Function
void signal_handler(int signum)
{
    if(signum == SIGINT)
    {
        if(pid == 0)                    //Without command
        {
            printf("\n%s", prompt_string);
            fflush(stdout);
        }

    }

    if(signum == SIGTSTP)
    {
        if(pid > 0)                        //With command
        {
            insert_to_linked_list();       //When process is stopped, insert to linked-list (insert first)
        }

        if(pid == 0)                        //Without command
        {
            printf("\n%s", prompt_string);
            fflush(stdout);
        }
    }

    if(signum == SIGCHLD)
    {
        int terminated_pid = 0;

        if((terminated_pid = waitpid(-1, NULL, WNOHANG)) > 0)
        {
            delete_from_linked_list(terminated_pid);        //Delete a node only after the process is completed.
        }
    }

}


//Function to insert the process details to the linked-list when a process is stopped
void insert_to_linked_list(void)
{
    store_job_data* new_node = malloc(sizeof(store_job_data));
    if(new_node == NULL)
    {
        perror("malloc");
        return;
    }

    new_node->pid = pid;        //Store the pid to the new_node
    new_node->commands = malloc(15 * sizeof(char*));        //Declare heap_segment memory to store the commands

    int i=0;
    for(i-=0; input_commands[i]!=NULL; i++)
    {
        new_node->commands[i] = strdup(input_commands[i]);      //Store the input_commands to heap, and assign the heap_address to the node
    }
    new_node->commands[i] = NULL;

    new_node->next = NULL;
    
    //Insert first logic
    if(head == NULL)
    {
        head = new_node;
        return;
    }
    
    store_job_data* temp = head;
    head = new_node;
    new_node->next = temp;

    return;
}


//Function to execute the "fg" command
void execute_fg_command(void)
{
    if(head == NULL)
    {
        return;
    }

    pid = head->pid;            //Assign head->pid value to global pid, so that program knows a process is running

    kill(head->pid, SIGCONT);       //Continue the stopped process
    
    for(int i=0; head->commands[i]!=NULL; i++)
    {
        printf("%s ", head->commands[i]);       //Prit the details of restarted process
    }
    printf("\n");
    
    int terminated_pid = 0;

    terminated_pid = waitpid(head->pid, NULL, WUNTRACED);       //Store the pid of terminated process to terminated_pid

    delete_from_linked_list(terminated_pid);        //Delete the node which contains terminated_pid.

    pid = 0;        //Reset global pid back to zero
}


////Function to execute the "bg" command
void execute_bg_command(void)
{
    if(head == NULL)
    {
        return;         //If no process is present in the linked-list, return
    }
    kill(head->pid, SIGCONT);       //Continue the stopped process
    
    for(int i=0; head->commands[i]!=NULL; i++)
    {
        printf("%s ", head->commands[i]);       //Print the details of restarted process
    }
    printf("&\n");    

}


//Function to delete the terminated process details from the linked_list
void delete_from_linked_list(int terminated_pid)
{
    if(head == NULL)
    {
        return;         //If no process is present in the linked-list, return
    }


    if(head->pid ==  terminated_pid)            //If the terminated pid is present at the first node
    {
        store_job_data* temp = head->next;

        for(int i = 0; head->commands[i] != NULL; i++) 
        {
            free(head->commands[i]);            //free the memory individually assigned for each command
        }
        free(head->commands);

        free(head);
        head = temp;

        return;
    }



    //If the terminated pid is present at the some other node

    store_job_data* prev = NULL;
    store_job_data* temp = head;

    while((temp != NULL) && (temp->pid != terminated_pid))
    {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL)        //If the pid is not found, return
    {
        return;
    }


    //Logic to delete the node

    prev->next = temp->next;
    for(int i=0; temp->commands[i]!=NULL; i++)
    {
        free(temp->commands[i]);        //free the memory individually assigned for each command
    }
    free(temp->commands);

    free(temp);

    return;
}