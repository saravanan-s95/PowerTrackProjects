#include "shell.h"
int pid;
int status;
char *external_cmd[154];
extern char prompt_str[20];
extern char input_str[25];
struct input *head = NULL;
void signal_handler(int signum)
{
	if(signum == SIGINT)
	{
		if(pid==0)
		{
            printf("\x1b[32m""\n%s""\x1b[0m", prompt_str);
            char buf[50];
            getcwd(buf,50);
            printf("\x1b[34m""%s$""\x1b[0m",buf);
            fflush(stdout);
        }
	}
    else if(signum == SIGTSTP)
	{
		if(pid == 0)
		{
            printf("\x1b[32m""\n%s""\x1b[0m", prompt_str);
            char buf[50];
            getcwd(buf,50);
            printf("\x1b[34m""%s$""\x1b[0m",buf);
            fflush(stdout);
        }
        else
            insert_first(); //store the stopped process
	}
    else if(signum == SIGCHLD)
    {
        //clear the resources
        if(head != NULL)
            waitpid(head->pid, &status, WNOHANG);
    }
}

void scan_input(char *prompt, char *input_str)
{
    
    extract_external_commands(external_cmd);
    signal(SIGINT,signal_handler);
	signal(SIGTSTP,signal_handler);
    while(1)
    {
        printf("\x1b[32m""%s""\x1b[0m", prompt);
        char buf[50];
        getcwd(buf,50);
        printf("\x1b[34m""%s$""\x1b[0m",buf);
        scanf(" %[^\n]", input_str);

        //check the input_str contains PS1= or not
        if(strstr(input_str, "PS1=") != NULL)
        {
            //check if input_string[4] is space or not
            if(input_str[4] != ' ')
            {     //no space
                strcpy(prompt, input_str + 4);
            }
            else 
            {   //space is present
                printf("ERROR : No space should be given after PS1=\n");
            }
        }
        else{   
      
            char *cmd = get_command(input_str);  //get the command
            int ret = check_command_type(cmd);

            if(ret == BUILTIN)
            {
                //logic for internal commands
                execute_internal_commands(input_str);
            }
            else if(ret == EXTERNAL)
            {
                //logic for External commands
                pid  = fork();
                if(pid > 0)
                {   //parent
                    waitpid(pid, &status, WUNTRACED);
                    pid = 0;
                }
                else if(pid == 0)
                {   //child
                    signal(SIGINT,SIG_DFL);
					signal(SIGTSTP,SIG_DFL);
                    execute_external_commands(input_str);
                }
            }
            else
            {
                //print error msg
                printf("%s: command not found\n",input_str);
            }    
        }
    }
}