#include "syntax_analyzer.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int brace_count = 0;
int paren_count = 0;
int error_found = 0;

void remove_comment(char line[])
{
    char *ptr = strstr(line, "//");
    if(ptr)
        *ptr = '\0';
}

void check_semicolon(char line[], int line_no)
{
    char temp[256];
    strcpy(temp, line);

    char *cmt = strstr(temp, "//");
    if(cmt)
    {
        *cmt = '\0';
    }

    int len = strlen(temp);

    if(len > 0 && temp[len - 1] == '\n')
    {
        temp[--len] = '\0';
    }

    char *p = temp;

    while(isspace(*p))
    {
        p++;
    }

    len = strlen(p);

    while(len > 0 && isspace(p[len - 1]))
    {
        p[--len] = '\0';
    }

    if(len == 0)
        return;

    if(p[0] == '#')
        return;

    if(strcmp(p, "{") == 0 || strcmp(p, "}") == 0)
        return;

    // if(p[len - 1] == '{')
    //     return;

    if(strncmp(p, "if(", 3) == 0 ||strncmp(p, "for(", 4) == 0 ||strncmp(p, "while(", 6) == 0 ||strncmp(p, "switch(", 7) == 0 ||strcmp(p, "else") == 0 ||strstr(p, "main()") != NULL)
    {
        return;
    }

    if(p[len - 1] != ';')
    {
        printf("Line %d : Missing semicolon\n", line_no);
        error_found = 1;
    }
}

void check_missing_identifier(char line[], int line_no)
{
    char datatype[20];
    char var[50];

    if(sscanf(line,"%s %s",datatype,var) == 2)
    {
        if(strcmp(datatype,"int") == 0 ||strcmp(datatype,"char") == 0 ||strcmp(datatype,"float") == 0 ||strcmp(datatype,"double") == 0)
        {
            if(strcmp(var,";") == 0)
            {
                printf("Line %d : Missing identifier\n",line_no);
                error_found = 1;
            }
        }
    }
}

void check_parenthesis(char line[], int line_no)
{
    if(strstr(line,"if(") ||strstr(line,"for(") ||strstr(line,"while(") ||strstr(line,"switch("))
    {
        int open = 0;
        int close = 0;

        for(int i = 0; line[i]; i++)
        {
            if(line[i] == '(')
            {
                open++;
            }

            if(line[i] == ')')
            {
                close++;
            }
        }

        if(open != close)
        {
            printf("Line %d : Missing closing parenthesis ')'\n",line_no);
            error_found = 1;
        }
    }
}

void check_symbols(char ch)
{
    if(ch == '{')
    {
        brace_count++;
    }
    else if(ch == '}')
    {
        brace_count--;

        if(brace_count < 0)
        {
            printf("Extra closing brace '}' found\n");
            error_found = 1;
            brace_count = 0;
        }
    }

    if(ch == '(')
    {
        paren_count++;
    }
    else if(ch == ')')
    {
        paren_count--;

        if(paren_count < 0)
        {
            printf("Extra closing parenthesis ')' found\n");
            error_found = 1;
            paren_count = 0;
        }
    }
}

void syntaxanalyzer(FILE *fp)
{
    char line[256];
    int line_no = 1;

    brace_count = 0;
    paren_count = 0;
    error_found = 0;

    while(fgets(line,sizeof(line),fp))
    {
        remove_comment(line);

        for(int i = 0; line[i]; i++)
        {
            check_symbols(line[i]);
        }

        check_missing_identifier(line,line_no);
        check_semicolon(line,line_no);
        check_parenthesis(line,line_no);

        line_no++;
    }

    if(paren_count > 0)
    {
        //printf("Missing closing parenthesis ')'\n");
        error_found = 1;
    }

    if(brace_count > 0)
    {
        printf("Missing closing brace '}'\n");
        error_found = 1;
    }

    if(!error_found)
    {
        printf("No syntax errors found\n");
    }
}