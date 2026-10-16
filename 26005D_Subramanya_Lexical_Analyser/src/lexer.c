#include "lexer.h"

status_e get_token(token_t *tok)
{
    FILE *fp = tok->fp;
    int ch;

    /* Skip whitespaces */
    while(((ch = fgetc(fp)) != EOF) && isspace(ch));

    /* fp reached EOF, no more tokens left */
    if(ch == EOF)
    {
        tok->type = TOK_EOF;
        return FAILURE;
    }


    /* words can be keyword or identifier */
    if(isalpha(ch) || ch == '_')
    {
        tok->type = is_keyword(ch, tok);

        if(tok->type == TOK_IDENTIFIER)
        {
            is_array(ch, tok);
        }
        return SUCCESS;
    }

    if(isdigit(ch))
    {
        is_numeric_constant(ch, tok);
        return SUCCESS;
    }

    /* Character and string literals */
    if(ch == '"' || ch == '\'')
    {
        is_literal(ch, tok);
        return SUCCESS;
    }

    /* Single and multiline commenting */
    if(ch == '/' && is_comment(ch, tok) == SUCCESS)
    return SUCCESS;

    /* Operators and symbols */
    if(strchr(symbol, ch) || strchr(operators, ch))
    {
        is_operator_or_symbol(ch, tok);
        return SUCCESS;
    }

    /* Directives */
    if(ch == '#')
    {
        is_directive(ch, tok);
        return SUCCESS;
    }

    /* placeholder for everything else, replaced in later steps */
    tok->lexeme[0] = ch;
    tok->lexeme[1] = '\0';
    tok->type = TOK_INVALID;
    return SUCCESS;

}

