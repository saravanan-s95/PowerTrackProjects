#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "4_lexical.h"

stack_t stk;

int flower_open_count = 0;
int flower_close_count = 0;

int error_count = 0;        //Incremented whenever an error is detected (To print total no.of errors)


//Function to find errors in the input c-file
void read_data_from_file_find_error(FILE* fptr)
{
    char ch = 0;
    char str[50];

    int line_count = 0;

    stk.capacity = 100;     //stack for bracket checking
    stk.top = -1;
    stk.arr = malloc(100 * sizeof(char));


    int fscanf_ret = 0;

    //while(fscanf(fptr, " %[^\n]", str) > 0)
    while((fscanf_ret = fscanf(fptr, "%[^\n]", str)) != EOF)     //Read line-by-line from file and store to str
    {
        line_count++;

        if(fscanf_ret == 0)
        {
            fgetc(fptr);
            continue;
        }

        int length = strlen(str);
        if(length>0 && str[length-1] == '\r')
        {
            str[length-1] = '\0';
        }
        fgetc(fptr);

        flower_bracket_stack(str);              //To push and pop '{' brackets to stack

        if(check_quotes(str) == FAILURE)        //Check all qoutes " opened are closed or not
        {
            printf("Error - Line %d : Quotes Error!\n", line_count);
            error_count++;
        }

        if(check_curved_bracket(str) == FAILURE)        //Check all curved brackets '(' ')' opened are closed or not
        {
            printf("Error - Line %d : '('  ')'  Bracket Error!\n", line_count);
            error_count++;
        }


        if(is_hexa_valid(str) == FAILURE)           //Check if an integer assigned in hexadecimal is valid or not
        {
            printf("Error - Line %d : Invalid Hexadecimal value!\n", line_count);
            error_count++;
        }
        else if(is_octal_valid(str) == FAILURE)     //Check if an integer assigned in octal is valid or not
        {
            printf("Error - Line %d : Invalid Octal value!\n", line_count);
            error_count++;
        }
        

    }

    check_flower_bracket();         //Check whether all flower brackets '{' opened are close dor not

    printf("Total Errors: %d\n", error_count++);        //Print total number of errors
    
    
}


//Function to check whether all quotes " opened are closed or not
int check_quotes(char* str)
{
    int start_index = -1;           //Initilize start_index to -1
    int length = strlen(str);       

    for(int i=0; str[i]!='\0'; i++)
    {
        if(str[i] == '"')       //When finding the first ", store the index value.
        {
            start_index = i;
            break;
        }
    }

    if(start_index > -1)       
    {
        for(int j = start_index+1; str[j]!='\0'; j++)       //Run the loop till end of string
        {
            if(str[j]=='"')             //If the closing quote is found, then the quote " is closed
            {
                return SUCCESS;
            }
        }
    }
    else if(start_index == -1)          //If startig index value was not modified, then no string literals present
    {
        //printf("Not string literals\n");
        return SUCCESS;
    }

    return FAILURE;     //Otherwise, failure -> ie., the " is not closed
}


//Function to check whether all curved_brackets '(' opened are closed or not
int check_curved_bracket(char* str)
{
    int start_index = -1;       
    int length = strlen(str);

    int open_count = 0;
    int close_count = 0;

    for(int i=0; str[i]!='\0'; i++)
    {
        if(str[i] == '(')
        {
            open_count++;       //When an '(' is detected, increment open_count
        }
        else if(str[i] == ')')
        {
            close_count++;      //When an ')' is detected, increment close_count
        }
    }

    if(open_count != close_count)       //If count of open_bracket and close_bracket is not equal, then failure
    {
        return FAILURE;
    }

    for(int i=0; str[i]!='\0'; i++)         
    {
        if(str[i] == '(')
        {
            start_index = i;        //When finding the first '(', store the index value.
            break;
        }
    }

    if(start_index > -1)
    {
        for(int j = start_index+1; str[j]!='\0'; j++)       //Run the loop till end of string
        {
            if(str[j]==')')         //If the closing bracket ')' is found, then the quote " is closed
            {
                return SUCCESS;
            }
        }
    }
    else if(start_index == -1)      //If startig index value was not modified, then no brackets present
    {
        return SUCCESS;
    }

    return FAILURE;             //Otherwise, failure -> ie., the ')' is not closed
}


//Function to add add & remove flower brackets '{' into stack
int flower_bracket_stack(char* str)
{

    for(int i=0; str[i]!='\0'; i++)
    {
        if(str[i] == '{')
        {
            push(str[i]);       //When a '{' is detected, push to stack; Also increment count
            flower_open_count++;
        }
        if(str[i] == '}')
        {
            pop();              //When a '}' is detected, pop from stack; Also increment count
            flower_close_count++;
        }
    }
}


//Function to push a character to stack
void push(char ch)
{
    if(stk.top == stk.capacity-1)
    {
        //printf("Stack full\n");
        return;
    }
    ++stk.top;
    stk.arr[stk.top] = ch;
}


//Function to pop a character from stack
void pop(void)
{
    if(stk.top == -1)
    {
        //printf("Stack empty\n");
        return;
    }
    stk.top--;
}


//Function to find whether all flower brackets '{' are closed
int check_flower_bracket(void)
{ 
    int open_count = 0;
    int close_count = 0;
    
    if(stk.top == -1)           //If no brackets in the stack means, either all brackets opened are closed, or close_brackets > open_brackets
    {
        if(flower_close_count > flower_open_count)     //If closed_bracket is more, means brackets is not opened
        {
            printf("Error - '{' flower brackets not opened\n");
            error_count++;
            return FAILURE;
        }

        return SUCCESS;
    }

    if(stk.top > -1)        //If stack is not empty means, some brackets are not closed.
    {
        printf("Error - %d '}' flower brackets not closed\n", (stk.top)+1);    
        error_count++;
        return FAILURE;
    }
}


//Function to check whether a hexadecimal value assigned is valid or not
int is_hexa_valid(char* str)
{
    char* token = NULL;
    token = strstr(str, "int");     //Check whether the datatype declared is int

    if(token != NULL)
    {
        token = strstr(token, "=");         //Check for assignment operator
        
        if(token != NULL)
        {
            if(token[1] == ' ')     //Considering the space case (eg: = 0x39F)
            {
                if(token[2]=='0' && (token[3]=='X' || token[3]=='x'))       //Checking for '0X' or '0x'
                {
                    for(int i=4; token[i]!=';'; i++)
                    {
                        char ch = token[i];
                        if(isdigit(ch) || (ch>=65 && ch<=70) || (ch>=97 && ch<=102))        //Check for digit or characters from (A-F) or (a-f)
                        {
                            continue;
                        }
                        else
                        {
                            return FAILURE;
                        }
                    }
                }
            }

            else if((token[1] == '0') && (token[2]=='X' || token[2]=='x'))      //Considering the case without space (eg: =0x39F)
            {
                for(int i=3; token[i]!=';'; i++)
                {
                    char ch = token[i];
                    if(isdigit(ch) || (ch>=65 && ch<=70) || (ch>=97 && ch<=102))     //Check for digit or characters from (A-F) or (a-f)
                    {
                        continue;
                    }
                    else
                    {
                        return FAILURE;
                    }
                }
            }
        }
    }

    return SUCCESS;
}


//Function to check whether an octal value assigned is valid or not
int is_octal_valid(char* str)
{
    char* token = NULL;
    token = strstr(str, "int");     //Check whether the datatype declared is int

    if(token != NULL)
    {
        token = strstr(token, "=");     //Check for assignment operator

        if(token != NULL)
        {
            if(token[1] == ' ')     //Considering the space case (eg: = 0744)
            {
                if(token[2]=='0' && token[3]!='X' && token[3]!='x')     
                {
                    for(int i=3; token[i]!=';'; i++)
                    {
                        char ch = token[i];
                        if(ch>=48 && ch<=55)        //Check for digits from (0-7)
                        {
                            continue;
                        }
                        else
                        {
                            return FAILURE;
                        }
                    }
                }
            }

            else if(token[1] == '0' && token[2]!='X' && token[2]!='x')      //Considering the case without space (eg: =0744)
            {
                for(int i=2; token[i]!=';'; i++)
                {
                    char ch = token[i];
                    if(ch>=48 && ch<=55)        //Check for digits from (0-7)
                    {
                        continue;
                    }
                    else
                    {
                        return FAILURE;
                    }
                }
            }
        }
    }

    return SUCCESS;
}