#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

const char *keywords[] ={"int","float","char","double","if","else","while","for","return","void"}; 
 
Status read_and_validate_args(char *argv[], Lexer *lexer)
{
    char *ext = strrchr(argv[1] , '.');
    if(ext == NULL)
    {
        printf("Invalid file format. Please provide an c (.c) file.\n");
        return e_failure;
    }
    else if(strstr(ext , ".c") != NULL)
    {
        lexer->C_file_name = argv[1];
    }
    else{
        printf("Invalid file format. Please provide an c (.c) file.\n");
        return e_failure;
    }
    return e_success;
}

Status analyze_file(Lexer *lexer)
{
    if(open_file(lexer) == e_failure)
    {
        printf("Unable to open the file %s. Make sure it is present in the current working directory.\n" , lexer->C_file_name);
        return e_failure;
    }

    if(extract_tokens(lexer) == e_failure)
    {
        fclose(lexer->fptr_file_c);
        return e_failure;
    }

    fclose(lexer->fptr_file_c);

    return e_success;
}

/*------------------------------------------------------*/
/*                FILE HANDLING                         */
/*------------------------------------------------------*/

Status open_file(Lexer *lexer)
{
    lexer->fptr_file_c = fopen(lexer->C_file_name,"r");

    if(lexer->fptr_file_c == NULL)
        return e_failure;

    return e_success;
}

/*------------------------------------------------------*/
/*                CLASSIFICATION                        */
/*------------------------------------------------------*/

int is_symbol(char ch)
{
    return ch=='(' ||
           ch==')' ||
           ch=='{' ||
           ch=='}' ||
           ch=='[' ||
           ch==']' ||
           ch==';' ||
           ch==',';
}

int is_operator(char ch)
{
    return ch=='+' ||
           ch=='-' ||
           ch=='*' ||
           ch=='/' ||
           ch=='=' ||
           ch=='<' ||
           ch=='>' ||
           ch=='!' ||
           ch=='&' ||
           ch=='|';
}

int is_keyword(char *token)
{
    int n = sizeof(keywords) / sizeof(keywords[0]);

    for(int i = 0; i < n; i++)
    {
        if(strcmp(token, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/*------------------------------------------------------*/
/*                  TOKEN SCANNER                       */
/*------------------------------------------------------*/

Status extract_tokens(Lexer *lexer)
{
    char ch;

    lexer->line_no = 1;

    while((ch = fgetc(lexer->fptr_file_c)) != EOF)
    {
        if(ch == '#')
        {
            if(skip_preprocessor(lexer) == e_failure)
                return e_failure;
        }
        else if(ch == '/')
        {
            if(skip_comments(lexer, ch) == e_failure)
                return e_failure;
        }
        else if(ch == '\n')
        {
            lexer->line_no++;
        }
        else if(ch == ' ' || ch == '\t' || ch == '\r')
        {
            continue;
        }
        else if(isalpha(ch) || ch == '_')
        {
            if(read_identifier_or_keyword(lexer, ch) == e_failure)
                return e_failure;
        }
        else if(isdigit(ch))
        {
            if(read_number(lexer, ch) == e_failure)
                return e_failure;
        }
        else if(ch == '\'')
        {
            if(read_character_literal(lexer, ch) == e_failure)
                return e_failure;
        }
        else if(ch == '"')
        {
            if(read_string_literal(lexer, ch) == e_failure)
                return e_failure;
        }
        else if(is_operator(ch))
        {
            if(read_operator(lexer, ch) == e_failure)
                return e_failure;
        }
        else if(is_symbol(ch))
        {
            if(classify_symbol(lexer, ch) == e_failure)
                return e_failure;
        }
        else
        {
            char token[2];
            token[0] = ch;
            token[1] = '\0';
           // printf("%-5d %-20c %-20s\n", lexer->line_no, ch, "UNKNOWN");
           store_token(lexer, token, "UNKNOWN");
        }
    }

    return e_success;
}

/*------------------------------------------------------*/
/*                 SKIP FUNCTIONS                       */
/*------------------------------------------------------*/

Status skip_preprocessor(Lexer *lexer)
{
    char ch;

    while((ch = fgetc(lexer->fptr_file_c)) != EOF && ch != '\n');

    lexer->line_no++;
    
    return e_success;
}

Status skip_comments(Lexer *lexer, char first)
{
    char ch;

    if(first != '/')
        return e_failure;

    ch = fgetc(lexer->fptr_file_c);

    if(ch == '/')
    {
        while((ch = fgetc(lexer->fptr_file_c)) != EOF)
        {
            if(ch == '\n')
            {
                lexer->line_no++;
                break;
            }
        }
    }

    else if(ch == '*')
    {
        char prev = 0;

        while((ch = fgetc(lexer->fptr_file_c)) != EOF)
        {
            if(ch == '\n')
                lexer->line_no++;

            if(prev == '*' && ch == '/')
                break;

            prev = ch;
        }
    }
    else
    {
        ungetc(ch, lexer->fptr_file_c);

        if(read_operator(lexer, first) == e_failure)
            return e_failure;
    }

    return e_success;
}

/*------------------------------------------------------*/
/*                  READ IDENTIFIER                     */
/*------------------------------------------------------*/

Status read_identifier_or_keyword(Lexer *lexer, char first)
{
    char token[100];
    int i = 0;
    char ch;

    token[i++] = first;

    while((ch = fgetc(lexer->fptr_file_c)) != EOF)
    {
        if(isalnum(ch) || ch == '_')
            token[i++] = ch;
        else
        {
            ungetc(ch, lexer->fptr_file_c);
            break;
        }
    }

    token[i] = '\0';
    classify_tokens(token, lexer);
    return e_success;
}

/*------------------------------------------------------*/
/*                    READ NUMBER                       */
/*------------------------------------------------------*/

Status read_number(Lexer *lexer, char first)
{
    char token[100];
    int i = 0;
    int dot_count = 0;
    char ch;

    token[i++] = first;
    
    if(first == '0')
    {
        ch = fgetc(lexer->fptr_file_c);

        if(ch == 'x' || ch == 'X')
        {
            token[i++] = ch;

            while((ch = fgetc(lexer->fptr_file_c)) != EOF)
            {
                if((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
                {
                    token[i++] = ch;
                }
                else
                {
                    ungetc(ch, lexer->fptr_file_c);
                    break;
                }
            }

            token[i] = '\0';

            if(i > 2)
                store_token(lexer, token, "HEXADECIMAL");
            else
                store_token(lexer, token, "INVALID HEX VALUE");

            return e_success;
        }
        else
        {
            ungetc(ch, lexer->fptr_file_c);
        }
    }
    while((ch = fgetc(lexer->fptr_file_c)) != EOF)
    {
        if(ch >= '0' && ch <= '9')
        {
            token[i++] = ch;
        }
        else if(ch == '.')
        {
            token[i++] = ch;
            dot_count++;
        }
        else
        {
            ungetc(ch, lexer->fptr_file_c);
            break;
        }
    }

    token[i] = '\0';
   
    if(dot_count == 0)
    {
        store_token(lexer, token, "INTEGER CONSTANT");
    }
    else if(dot_count == 1)
    {
         int len = strlen(token);

        if(token[0] == '.' || token[len - 1] == '.')
        {
          store_token(lexer, token, "INVALID FLOATING CONSTANT");
        }
        else
        {
            store_token(lexer, token, "FLOATING CONSTANT");
        }
    }
    else
    {
      store_token(lexer, token, "INVALID FLOATING CONSTANT");
    }
    return e_success;
}

Status read_character_literal(Lexer *lexer , char first)
{
    char token[100];
    int i = 0;
    char ch;

    token[i++] = first;

    while((ch = fgetc(lexer->fptr_file_c)) != EOF)
    {
        if(ch == '\r')
        {
            continue;
        }

        if(ch == '\n')
        {
            lexer->line_no++;
            ungetc(ch, lexer->fptr_file_c);
            break;
        }

        token[i++] = ch;

        if(ch == '\'')
            break;
    }

    token[i] = '\0';

    if(token[i - 1] != '\'')
    {
       store_token(lexer, token, "ERROR INVALID CHARACTER");
    }
    else if(strlen(token) == 3)
    {
       store_token(lexer, token, "CHARACTER");
    }
    else if(strlen(token) == 4 && token[1] == '\\')
    {
       store_token(lexer, token, "CHARACTER");
    }
    else
    {
       store_token(lexer, token, "ERROR INVALID CHARACTER");
    }
    return e_success;
}

/*------------------------------------------------------*/
/*               READ STRING LITERAL                    */
/*------------------------------------------------------*/

Status read_string_literal(Lexer *lexer, char first)
{
    char token[200];
    int i = 0;
    char ch;

    token[i++] = first;      // Store opening "

    while((ch = fgetc(lexer->fptr_file_c)) != EOF)
    {
        if(ch == '\r')
        {
            continue;
        }
       if(ch == '\n')
        {
            lexer->line_no++;
            ungetc(ch, lexer->fptr_file_c);
            break;
        }

        token[i++] = ch;
        
        if(ch == '"')
            break;
    }

    token[i] = '\0';

    if(token[i - 1] != '"')
    {
      //token[strcspn(token, "\n")] = '\0';
      store_token(lexer, token, "ERROR INVALID STRING");
    }
    else
    {
       // printf("%-5d %-20s %-20s\n", lexer->line_no, token, "STRING");
        store_token(lexer, token, "STRING LITERAL");
    }
    return e_success;
}

Status read_operator(Lexer *lexer, char first)
{
    char token[4];
    char next;

    token[0] = first;
    token[1] = '\0';

    next = fgetc(lexer->fptr_file_c);

    if((first == '+' && (next == '+' || next == '=')) ||
       (first == '-' && (next == '-' || next == '=')) ||
       (first == '*' && next == '=') ||
       (first == '/' && next == '=') ||
       (first == '%' && next == '=') ||
       (first == '=' && next == '=') ||
       (first == '!' && next == '=') ||
       (first == '<' && (next == '=' || next == '<')) ||
       (first == '>' && (next == '=' || next == '>')) ||
       (first == '&' && (next == '&' || next == '=')) ||
       (first == '|' && (next == '|' || next == '=')))
    {
        token[1] = next;
        token[2] = '\0';
    }
    else
    {
        ungetc(next, lexer->fptr_file_c);
    }

   // printf("%-5d %-20s %-20s\n", lexer->line_no, token, "OPERATOR");
    store_token(lexer, token, "OPERATOR");
    return e_success;
}

/*------------------------------------------------------*/
/*                 CLASSIFY SYMBOL                      */
/*------------------------------------------------------*/

Status classify_symbol(Lexer *lexer, char ch)
{
    char token[2];
    token[0] = ch;
    token[1] = '\0';
    store_token(lexer, token, "SYMBOL"); 
    return e_success;
}

/*------------------------------------------------------*/
/*              CLASSIFY IDENTIFIER                     */
/*------------------------------------------------------*/

Status classify_tokens(char *token , Lexer *lexer)
{
    if(is_keyword(token))
    {
       // printf("%-5d %-20s %-20s\n", lexer->line_no , token , "KEYWORD");
        store_token(lexer, token, "KEYWORD");
    }
    else
    {
       // printf("%-5d %-20s %-20s\n", lexer->line_no , token , "IDENTIFIER");
        store_token(lexer, token, "IDENTIFIER");
    }
    
    return e_success;
}

/*------------------------------------------------------*/
/*                 STORE TOKENS                      */
/*------------------------------------------------------*/

Status store_token(Lexer *lexer, char *token, char *type)
{
    if(lexer->token_count >= 1000)
    {
        printf("Error: Token storage full\n");
        return e_failure;
    }
    strcpy(lexer->tokens[lexer->token_count].token, token);
    strcpy(lexer->tokens[lexer->token_count].type, type);
    lexer->tokens[lexer->token_count].line_no = lexer->line_no;
    lexer->token_count++;
    
    return e_success;
}

/*------------------------------------------------------*/
/*                DISPLAY TOKENS                   */
/*------------------------------------------------------*/

void display_tokens(Lexer *lexer)
{
    for(int i = 0; i < lexer->token_count; i++)
    {
        printf("%-5d | %-20s | %-20s\n", lexer->tokens[i].line_no, lexer->tokens[i].token, lexer->tokens[i].type);
    }
}