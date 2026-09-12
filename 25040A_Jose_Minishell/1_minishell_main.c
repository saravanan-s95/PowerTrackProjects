#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "minishell.h"

char* ext_cmd_arr[153];

char* builtins[] = 
{	"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval", 
	"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
	"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", 
	"jobs", "bg", "fg", NULL 
};
							
char* input_commands[15];

char prompt_string[100];

store_job_data* head = NULL;



int main()
{
	extract_external_command();				//Convert external_commands from textfile to array.

	system("clear");						//Clear the terminal
	strcpy(prompt_string, "minishell $: ");
    printf("%s", prompt_string);            //Print the prompt_string


	signal(SIGINT, signal_handler);			//Registering the siganls
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);

	while(1)
	{
		char user_input[100];

		if(scanf("%[^\n]", user_input) <= 0)
		{
			clearerr(stdin);
			getchar(); 
            printf("%s", prompt_string);
			continue;
		}
		getchar();

		//Check whether the user input is having PS1
		//(To identify whether we have to replace the prompt_string)
		if(strncmp(user_input, "PS1=", 4) == 0)
		{
			if(user_input[4] == ' ')
			{
				printf("Invalid argument\n");
			}
			else
			{
				strcpy(prompt_string, (user_input+4));
			}
		}

		else 			//If not PS1 case
		{
			separate_with_spaces(user_input);		//Convert the user_input_string to a 2D array format

			int ret = check_command_type(input_commands[0]);		//Check whether the command is external or builtin
			if(ret == EXTERNAL)
			{
				execute_external_command();			//External command
			}
			else if(ret == BUILTIN)
			{
				execute_internal_command();			//Builtin command
			}
			else 
			{
				printf("Not a valid command\n");
			}
		}	

		printf("%s", prompt_string);
		
	}

	return 0;
}

