#include "lexer.h"

int is_keyword(int ch, token_t *tok)
{
    FILE *fp = tok->fp;

    /* Store the full word + termiation NULL character */
    int i = 0;
    while((isalnum(ch) || ch == '_') && i < MAX_LEXEME - 2)
    {
        tok->lexeme[i++] = ch;
        ch = fgetc(fp);
    }
    ungetc(ch, fp);

    tok->lexeme[i] = '\0';

    /* Check if it is a RESERVED KEYWORD */
    i = 0;
    while(type_keywords[i] != NULL)
    {
        if(strcmp(tok->lexeme,type_keywords[i]) == 0)
        {
            return TOK_TYPE_KEYWORD;
        }
        i++;
    }

    /* Check if it is a NON-RESERVED KEYWORD */
    i = 0;
    while(control_keywords[i] != NULL)
    {
        if(strcmp(tok->lexeme,control_keywords[i]) == 0)
        {
            return TOK_CONTROL_KEYWORD;
        }
        i++;
    }

    /* If neither is true, the word is an IDENTIFIER */
    return TOK_IDENTIFIER;
}

void is_array(int ch, token_t *tok)
{
    FILE *fp = tok->fp;

    ch = fgetc(fp);
    
    if(ch != '[')
    {
        ungetc(ch, fp);
        return;
    }

    int idx = strlen(tok->lexeme);

    while(ch != ']')
    {
        if((idx >= MAX_LEXEME - 2) || (ch == EOF) || (ch == '\n'))
        {
            tok->lexeme[idx] = '\0';
            tok->type = TOK_INVALID;

            return;
        }

        tok->lexeme[idx++] = ch;
        ch = fgetc(fp);
    }

    tok->lexeme[idx++] = ch;
    tok->lexeme[idx] = '\0';
    
    tok->type = TOK_ARRAY;

    return;
}

status_e is_numeric_constant(int ch, token_t *tok)
{
    FILE *fp = tok->fp;

    /* Store the number */
    int i = 0;
    int isPunc = 0;
    while((isdigit(ch) || (ch == '.')) && (i < MAX_LEXEME - 2))
    {
        /* Single '.' implies floating number */
        if(ispunct(ch))
            isPunc += 1;
        
        tok->lexeme[i++] = ch;
        ch = fgetc(fp);
    }
    ungetc(ch, fp);

    tok->lexeme[i] = '\0';

    if(isPunc == 0)
        tok->type = TOK_INT_CONSTANT;
    else if(isPunc == 1)
        tok->type = TOK_FLOAT_CONSTANT;
    else
        tok->type = TOK_INVALID;

    return SUCCESS;

}

status_e is_literal(int ch, token_t *tok)
{
    FILE *fp = tok->fp;
    int quote = ch;

    int i = 0;
    tok->lexeme[i++] = ch;

    /* Store until we find ending quotes */
    while((ch = fgetc(fp)) != quote)
    {
        if(ch == '\\')
        {
            tok->lexeme[i++] = ch;
            ch = fgetc(fp);
        }

        if((ch == '\n') || (ch == EOF) || (ch == '\r') || (i > MAX_LEXEME - 3))
        {
            tok->type = TOK_INVALID;
            tok->lexeme[i] = '\0';
            return FAILURE;
        }

        tok->lexeme[i++] = ch;
    }
    tok->lexeme[i++] = ch;

    tok->lexeme[i] = '\0';

    if(quote == '\'')
        tok->type = TOK_CHAR_LITERAL;
    else
        tok->type = TOK_STRING_LITERAL;

    return SUCCESS;

}

status_e is_operator_or_symbol(int ch1, token_t *tok)
{
    FILE *fp = tok->fp;

    tok->lexeme[0] = ch1;
    tok->lexeme[1] = '\0';

    /* Check symbols */
    if(strchr(symbol, ch1) != NULL)
    {
        tok->type = TOK_SYMBOL;
        return SUCCESS;
    }

    /* Read second character */
    int ch2 = fgetc(fp);

    if(ch2 == EOF)
    {
        if(strchr(operators, ch1) != NULL)
        {
            tok->type = TOK_OPERATOR;
            return SUCCESS;
        }

        tok->type = TOK_INVALID;
        return FAILURE;
    }

    char str2[3] = {ch1, ch2, '\0'};

    /* Check two-character operator */
    if(is_compound_helper(str2) == SUCCESS)
    {
        int ch3 = fgetc(fp);

        if(ch3 != EOF)
        {
            char str3[4] = {ch1, ch2, ch3, '\0'};

            /* Check three-character operator */
            if(is_compound_helper(str3) == SUCCESS)
            {
                strcpy(tok->lexeme, str3);
                tok->type = TOK_OPERATOR;
                return SUCCESS;
            }

            ungetc(ch3, fp);
        }

        strcpy(tok->lexeme, str2);
        tok->type = TOK_OPERATOR;
        return SUCCESS;
    }

    /* Second character doesn't form a compound operator */
    ungetc(ch2, fp);

    /* Check single-character operator */
    if(strchr(operators, ch1) != NULL)
    {
        tok->type = TOK_OPERATOR;
        return SUCCESS;
    }

    tok->type = TOK_INVALID;
    return FAILURE;
}

status_e is_compound_helper(char *str)
{
    int i = 0;

    while (compound_operators[i] != NULL)
    {
        if (strcmp(str, compound_operators[i]) == 0)
        {
            return SUCCESS;
        }

        i++;
    }

    return FAILURE;
}

status_e is_directive(int ch, token_t *tok)
{
    FILE *fp = tok->fp;

    /* Store the directive */
    int idx = 0;
    tok->lexeme[idx++] = ch;

    ch = fgetc(fp);
    while((isalpha(ch)) && (idx < MAX_LEXEME - 2))
    {
        tok->lexeme[idx++] = ch;
        ch = fgetc(fp); 
    }
    ungetc(ch, fp);

    tok->lexeme[idx] = '\0';


    /* Compare and check if it exists */
    int i = 0;
    while(directives[i] != NULL)
    {
        if(strcmp(tok->lexeme, directives[i]) == 0)
        {
            tok->type = TOK_DIRECTIVE;
            return SUCCESS;
        }
        i++;
    }

    tok->type = TOK_INVALID;
    return FAILURE;
}

status_e is_comment(int ch, token_t *tok)
{
    FILE *fp = tok->fp;

    int idx = 0;
    tok->lexeme[idx++] = ch;            

    int ch2 = fgetc(fp);
    if(ch2 != '/' && ch2 != '*')          /* division operator */
    {
        ungetc(ch2, fp);
        return FAILURE;
    }
    tok->lexeme[idx++] = ch2;

    /* single-line: read till end of line */
    if(ch2 == '/')
    {
        ch = fgetc(fp); 
        while((ch != '\n') && (ch != '\r') && (ch != EOF))
        {
            if(idx < MAX_LEXEME - 2)
                tok->lexeme[idx++] = ch;
        
            ch = fgetc(fp);
        }

        ungetc(ch, fp);
        tok->lexeme[idx] = '\0';
        tok->type = TOK_COMMENT;
        return SUCCESS;
    }

    /* multi-line: read until "*" then "/" */
    if(ch2 == '*')
    {
        int prev = 0;
        while((ch = fgetc(fp)) != EOF)
        {
            if(idx < MAX_LEXEME - 2)
                tok->lexeme[idx++] = ch;

            if((prev == '*') && (ch == '/'))
                break;

            prev = ch;
        }
    }

    tok->lexeme[idx] = '\0';
    tok->type = (ch == EOF) ? TOK_INVALID : TOK_COMMENT;
    return SUCCESS;
}