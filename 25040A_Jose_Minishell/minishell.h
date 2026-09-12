#ifndef HEADER_H
#define HEADER_H


//MACROS
#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3


//FUNCTION PROTOTYPES

void separate_with_spaces(char* user_input);
int check_command_type(char* command);

void execute_external_command(void);
void extract_external_command(void);

void execute_internal_command(void);
void execute_cd_command(void);
void execute_pwd_command(void);
void execute_echo_command(void);
void signal_handler(int signum);
void execute_jobs_command(void);
void execute_fg_command(void);
void execute_bg_command(void);


void insert_to_linked_list(void);
void delete_from_linked_list(int terminated_pid);



//STRUCTURE DEFINITION
typedef struct jobs_store
{
    int pid;
    char** commands;
    struct jobs_store* next;
}store_job_data;


#endif