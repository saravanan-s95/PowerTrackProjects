/*Name : Riddhi B Shitole
  ECEP ID : 25034F_036
  Project Name : Minishell Implementation using Linux */
#include "main.h"

char prompt[50] = "minishell$:- ";
char input[50] = {'\0'};
int pid = 0;
Slist* head = NULL;
int main()
{

    system("clear");

    signal(SIGINT, my_handler);
    signal(SIGTSTP, my_handler);
    signal(SIGCHLD, my_handler);

    scan_input(prompt,input);
}