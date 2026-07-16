#include<stdio.h>
#include "shell.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","jobs", "fg", "bg", NULL};

extern char*external_cmd[154];
extern int status;
char command[10];
extern struct input *head;

char *get_command(char *input_string)
{
	//get the first word => input_str ==> ls -l
		//get the ls and return it
	int i=0;
	while(input_string[i] != ' ' && input_string[i] != '\0'){
		command[i] = input_string[i];
		i++;
	}
	command[i] = 0;

	return command;
}

void extract_external_commands(char **external_commands)
{
	//step 1: open file & read content till EOF
		//step 2: read till \n
		//step 3: allocate memory for col with strlen size
		//step 4: store the cmd in external_comands[][]
	//step 2 to 4 

	int fd = open("external_commands.txt", O_RDONLY);
	if(fd == -1){
		perror("open");
		return;
	}
	char ext_cmd[20];
	char buf[1];
	int i=0, idx=0;
	while(read(fd , buf, 1)>0)
	{
		if(buf[0] != '\n')
		{
			ext_cmd[i++] = buf[0];
		}
		else
		{
			ext_cmd[i] = '\0';
			external_cmd[idx] = malloc(strlen(ext_cmd) + 1);
			strcpy(external_cmd[idx], ext_cmd);

			idx++;
			i = 0;
		}
	}
	//last command if no newline at EOF
	if(i > 0)
	{
		ext_cmd[i] = '\0';
		external_cmd[idx] = malloc(strlen(ext_cmd) + 1);
		strcpy(external_cmd[idx], ext_cmd);
		idx++;
	}
	external_cmd[idx] = NULL;
}

int check_command_type(char *command)
{
	//compare the cmd with bultins[]
	for(int i=0; builtins[i] != NULL; i++)
	{	
		if(strcmp(builtins[i],command) == 0)
			return BUILTIN;
	}
	
	//compare the cmd with external_cmd[][]
	for(int i=0; external_cmd[i] != NULL; i++)
	{	
		if(strcmp(external_cmd[i],command) == 0)
			return EXTERNAL;
	}
		
	//else
		return NO_COMMAND;
}

void execute_internal_commands(char *input_string)
{
	//1) cd
	//2) pwd
	//3) exit

	//check the input_str contains cd or not
	if(strncmp(input_string, "cd ", 3) == 0)
		chdir(input_string + 3);

	//check input_str contains pwd or not
	else if(strncmp(input_string, "pwd", 3) == 0)
	{
		char buf[50];
		getcwd(buf,50);
		printf("%s\n",buf);
	}

	//check the input_str contains exit or not
	else if(strcmp(input_string, "exit") == 0)
		exit(0);
	
	//check the input_str contains echo $$ or not
	else if(strcmp(input_string, "echo $$") == 0)
		printf("%d\n",getpid());	//print pid of current process
	
	//check the input_str contains echo $? or not
	else if(strcmp(input_string, "echo $?") == 0)
		printf("%d\n", WEXITSTATUS(status));		//print exit status of previous command

	//check the input_str contains echo $SHELL or not
	else if(strcmp(input_string, "echo $SHELL") == 0)
		printf("%s\n",getenv("SHELL"));				//print shell path
	

	//check the input_str contains jobs or not
	else if(strcmp(input_string, "jobs") == 0)
		print_list();
	
	//check the input_str contains fg or not
	else if(strcmp(input_string, "fg") == 0)
	{
		if(head == NULL)
		{
			printf("No jobs\n");
			return;
		}
		kill(head->pid , SIGCONT);
		waitpid(head->pid, &status, WUNTRACED);
		delete_first();
	}
	
	//check the input_str contains bg or not
	else if(strcmp(input_string, "bg") == 0)
	{
		if(head == NULL)
		{
			printf("No jobs\n");
			return;
		}
		kill(head->pid , SIGCONT);
		delete_first();
		signal(SIGCHLD , signal_handler);
	}
		
}

void execute_external_commands(char *input_string)
{
	//convert the input_str[] to 2d arr[][]
	char *arr[20];	//to store each command string separately
	int idx=0, j=0;
	char buf[30];	//to get command string
	int len = strlen(input_string);
	for(int i=0 ; i<=len ; i++)
	{
		if(input_string[i] != ' ' && input_string[i] != '\0'){
			buf[j++] = input_string[i];
		}
		else{
			if(j!=0){
				buf[j] = '\0';
				arr[idx] = malloc(strlen(buf)+1);
				strcpy(arr[idx] , buf);
				idx++;
				j=0;
			}
		}
	}
	arr[idx] = NULL;
	//check the pipe is present or not
	
	int cmd_count = 1;
    int cmd_pos[idx];  //to store starting index pos of each command
    cmd_pos[0] = 0;     //first command starts after ./a.out i.e. index=1
    int idx1 = 1;
    for(int i = 0; arr[i] != NULL; i++)
    {
        if(strcmp(arr[i], "|") == 0)
        {
            arr[i] = NULL;
            cmd_pos[idx1++] = i+1;
            cmd_count++;
        }
    }
   
	if(cmd_count == 1)
	{	//if pipe is not present
		execvp(arr[0],arr);
	}

	//if pipe is present
		//logic for npipe
    for(int i=0 ; i<cmd_count ;i++)
    {
        int fd[2];
        pipe(fd);

        int pid = fork();
        if(pid > 0)
        {   //parent
            if(i < cmd_count-1)
            {
				dup2(0,5);		//backup of STDIN
                close(fd[1]);   //close pipe write end
                dup2(fd[0],0);  //Replace STDIN by pipe read end
                close(fd[0]);   //close pipe read end for next child
            }
            for(int i=0; i<cmd_count ;i++)
                wait(NULL);
			dup2(5,0);	//Bring back STDIN to its place
        }
        else if(pid == 0)
        {   //child
            if(i < cmd_count-1)
            {
                close(fd[0]);   //close pipe read end
                dup2(fd[1],1);  //redirect output to pipe write end
            }
            execvp(arr[cmd_pos[i]], arr+cmd_pos[i]);
        }
        else{
            perror("fork");
            return;
        }
    }
}