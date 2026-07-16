#include<stdio.h>
#include "shell.h"
char prompt_str[20] = "minishell:";
char input_str[25];
int main()
{
    system("clear");
    scan_input(prompt_str, input_str);
}