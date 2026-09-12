#include <stdio.h>
#include <string.h>
#include "minishell.h"

extern char* ext_cmd_arr[153];
extern char* builtins[];
extern char* input_commands[15];


//Fuunction to separate the user_input strings to 2D array format
void separate_with_spaces(char* user_input)
{
	char* ret;
	int index = 0;
	ret = strtok(user_input, " ");		//Extract using strtok function
	while(ret != NULL)
	{
		input_commands[index] = ret;
		index++;
		ret = strtok(NULL, " ");
	}
	input_commands[index] = NULL;

	// printf("INPUT COMMANDS AFTER SPACE SEPARATION:\n");
	// for(int i=0; input_commands[i]!= NULL; i++)
	// {
	// 	printf("%s\n", input_commands[i]);
	// }	
}


//Function to check whether the input is external or builtin command
int check_command_type(char* command)
{
	for(int i=0; builtins[i]!=NULL; i++)		//Check for builtin command
	{
		if(strcmp(command, builtins[i]) == 0)
		{
			return BUILTIN;
		}		
	}
	
	for(int i=0; ext_cmd_arr[i]!=NULL; i++)		//Check for external command
	{
		if(strcmp(command, ext_cmd_arr[i]) == 0)
		{
			return EXTERNAL;
		}
	}

	return NO_COMMAND;
}