#include "main.h"

int parentheses_count = 0;  //variable to count opening and closing parentheses.
int curly_count = 0;    //variable to count opening and closing curly braces.

int is_operator(char ch, FILE *fptr,int line_number)    //check for operator function.
{
    char operator[5];   // Buffer to store operator.
    int j=0;    // Index for operator buffer.
    if(ch=='+') // Check for '+' operator.
    {
        operator[j++] = ch; // Store '+'.
        char ch1 = fgetc(fptr); // Read the next character.
        if(ch1 == '+')  // Check for increment operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tIncrement Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            ungetc(ch1,fptr);   // Put back extra character.
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            return SUCCESS;
        }
    }
    else if(ch=='-')    // Check for '-' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '-')  // Check for decrement operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tDecrement Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    else if(ch=='=')    // Check for '=' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '=')  // Check for equality operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tRelational Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    else if(ch=='>')    // Check for '>' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '=')  // Check for '>=' operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tRelational Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    else if(ch=='<')    // Check for '<' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '=')  // Check for '<=' operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tRelational Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    else if(ch=='!')    // Check for '!' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '=')  // Check for '!=' operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tRelational Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    else if(ch=='&')    // Check for '&' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '&')  // Check for logical AND operator.
        {
            operator[j++] = ch1;
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tLogical AND Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    else if(ch=='|')    // Check for '|' operator.
    {
        operator[j++] = ch;
        char ch1 = fgetc(fptr);
        if(ch1 == '|')  // Check for logical OR operator.
        {
            operator[j++] = ch1;
            operator[j] ='\0';
            printf(RED "%d\t\t%s\t\tLogical OR Operator\n" RESET,line_number, operator);
            return SUCCESS;
        }
        else
        {
            operator[j] = '\0';
            printf(RED "%d\t\t%s\t\tOperator\n" RESET,line_number, operator);
            ungetc(ch1,fptr);
            return SUCCESS;
        }
    }
    return FAILURE; // Character is not an operator.
}

int is_identifier(char *word)   // Function to check whether the token is a valid identifier.
{
    if(!(isalpha(word[0]) || word[0] == '_'))   //check if 1st character is an alphabet or underscore(_).
    {
        return FAILURE;   //if not, return 0.
    }
    for(int i = 1; word[i]; i++)    //loop through the word and check that the word has only alphabets, numbers or underscore(_).
    {
        if(!(isalnum(word[i]) || word[i] == '_'))
        {
            return FAILURE;   //if not then return failure.
        }
    }
    return SUCCESS;   //return success otherwise.
}

int is_float(char *word)    // Function to check whether the token is a floating-point number.
{
    int count = 0;  // Counts decimal points.
    int before_dot = 0;     // Counts digits before decimal point.
    int after_dot = 0;      // Counts digits after decimal point.

    for(int i=0;word[i] != '\0';i++)    // Traverse every character of the token.
    {
        if(isdigit(word[i]) && count == 1)  // Count digits after decimal point.
        {
            after_dot++;
            // continue;
        }
        else if(isdigit(word[i]) && count == 0)     // Count digits before decimal point.
        {
            before_dot++;
        }
        else if(word[i] == '.') // Check for decimal point.
        {
            if(count == 0)  // Allow only one decimal point.
            {
                count++;
            }
            else
            {
                return FAILURE;
            }
        }
        else if((word[i] == 'f' || word[i] == 'F') && word[i + 1] == '\0')  // Allow optional float suffix 'f' or 'F'.
        {
            continue;
        }
        else
        {
            return FAILURE; // Invalid character found.
        }
    }
    if(count == 0 || before_dot == 0 || after_dot == 0 || count>1)  // Float must contain one decimal point and digits on both sides.
    {
        return FAILURE;
    }
    return SUCCESS;
}

void is_preprocessor(char ch,FILE *fptr,int line_number)    // Function to identify and print preprocessor directives.
{
    char preprocessor[30];  // Buffer to store preprocessor directive.
    int i = 0;  // Index variable.
    preprocessor[i++] = ch; // Store '#' character.
    while(1)      // Read the complete preprocessor directive.
    {
        ch = fgetc(fptr);   // Read next character.
        if(ch != ' ' && ch != '<' && ch != '"')     // Continue until space or header starts.
        {
            preprocessor[i++] = ch;     // Store directive.
        }
        else
        {
            break;  // Stop reading directive.
        }
    }
    // ungetc(ch,fptr);
    preprocessor[i] = '\0'; // Null terminate the directive.
    printf(RED "%d\t\t%s\t  Pre-processor directive\n" RESET,line_number, preprocessor);
    if(strcmp(preprocessor,"#include") == 0)    // Check whether it is #include.
    {
        char header[30];    // Buffer to store header file.
        i=0;    // Reset index.
        while(ch == ' ')    // Skip spaces after #include.
        {
            ch = fgetc(fptr);
        }
        header[i++] = ch;   // Store opening '<' or '"'.
        if (ch == '<')  // Handle system header files
        {
            while ((ch = fgetc(fptr)) != '>')   // Read until closing '>'.
            {
                if (ch == EOF || ch == '\n')    // Check for invalid header.
                {
                    strcpy(errors[error_count], "Error : Invalid Header File.\n");
                    error_line[error_count] = line_number;
                    error_count++;
                    return;
                }
                header[i++] = ch;    // Store header name.
            }
            header[i++] = ch;   // Store closing '>'.
        }
        else if (ch == '"') // Handle user-defined header files.
        {
            while ((ch = fgetc(fptr)) != '"')       // Read until closing '"'.
            {
                if (ch == EOF || ch == '\n')    // Check for invalid header.
                {
                    strcpy(errors[error_count], "Error : Invalid Header File.\n");
                    error_line[error_count] = line_number;
                    error_count++;
                    return;
                }
                header[i++] = ch;   // Store header name.
            }

            header[i++] = ch;   // Store closing '"'.
        }
        header[i] = '\0';
        printf(BCYAN "%d\t\t%s\t\tHeader File\n" RESET,line_number, header);
    }
    else
    {
        return; // Return for other preprocessor directives.
    }
}

void check_brackets(char ch, int line_number)   // Function to validate matching brackets.
{
    if(ch == '(')   // Opening parenthesis found.
    {
        parentheses_count++;    // Increment parenthesis count.
    }
    else if(ch == ')')  // Closing parenthesis found.
    {
        if(parentheses_count > 0)   // Check if opening parenthesis exists.
            parentheses_count--;    // Match found, decrement count.
        else
        {
            strcpy(errors[error_count], "Error : Closing ')' without opening '('");
            error_line[error_count] = line_number;
            error_count++;
        }
    }
    else if(ch == '{')  // Opening curly brace found.
    {
        curly_count++;  // Increment curly brace count.
    }
    else if(ch == '}')  // Closing curly brace found.
    {
        if(curly_count > 0) // Check if opening brace exists.
            curly_count--;  // Match found, decrement count.
        else
        {
            strcpy(errors[error_count], "Error : Closing '}' without opening '{'");
            error_line[error_count] = line_number;
            error_count++;
        }
    }
}

void check_remaining_brackets(int line_number)  // Function to check for brackets left unclosed at EOF.
{
    while(parentheses_count > 0)    // Check for remaining '('.
    {
        strcpy(errors[error_count], "Error : '(' opened but not closed");
        // error_line[error_count] = line_number;
        error_count++;
        parentheses_count--;    // Reduce pending count.
    }

    while(curly_count > 0)  // Check for remaining '{'.
    {
        strcpy(errors[error_count], "Error : '{' opened but not closed");
        // error_line[error_count] = line_number;
        error_count++;
        curly_count--;  // Reduce pending count.
    }
}