#include <unistd.h>
#include "lexer.h"

#define RESET "\033[0m"

/* One color per token type, same order as token_type_e */
static const char *type_color(token_type_e t)
{
    static const char *colors[] = {
        "\033[1;34m",   /* Type Keyword    - blue    */
        "\033[1;35m",   /* Control Keyword - magenta */
        "\033[1;33m",   /* Directive       - yellow  */
        "\033[0m",      /* Identifier      - default */
        "\033[1;36m",   /* Array           - cyan    */
        "\033[32m",     /* Int Constant    - green   */
        "\033[32m",     /* Float Constant  - green   */
        "\033[32m",     /* Char Literal    - green   */
        "\033[32m",     /* String Literal  - green   */
        "\033[0m",      /* Operator        - default */
        "\033[0m",      /* Symbol          - default */
        "\033[1;31m",   /* Invalid         - red     */
        "\033[2m",      /* Comment         - dim     */
        "\033[0m"       /* EOF             - default */
    };
    return colors[t];
}

const char *type_name(token_type_e t)
{
    static const char *names[] = {
        "Type Keyword (Reserved Keywords)", "Control Keyword (Non-Reserved Keywords)", "Directive", "Identifier",
        "Array", "Int Constant", "Float Constant", "Char Literal",
        "String Literal", "Operator", "Symbol", "Invalid", "Comment", "EOF"
    };
    return names[t];
}

void print_banner(FILE *out, const char *filename)
{
    fprintf(out, "==============================================\n");
    fprintf(out, "      LEXICAL ANALYZER  -  C Tokenizer\n");
    fprintf(out, "==============================================\n");
    fprintf(out, "Input file : %s\n\n", filename);
}

void print_header(FILE *out)
{
    fprintf(out, "%-20s %s\n", "LEXEME", "TYPE");
    fprintf(out, "%-20s %s\n", "--------------------", "----------------");
}

void print_token(FILE *out, const token_t *tok)
{
    /* Colors only when writing to a terminal, never into a saved file */
    if(isatty(fileno(out)))
        fprintf(out, "%s%-20s %s%s\n", type_color(tok->type), tok->lexeme,
                type_name(tok->type), RESET);
    else
        fprintf(out, "%-20s %s\n", tok->lexeme, type_name(tok->type));
}

void print_summary(FILE *out, const int count[], int total)
{
    fprintf(out, "%-20s %s\n", "--------------------", "----------------");
    fprintf(out, "\nSUMMARY\n");
    fprintf(out, "  Total tokens : %d\n", total);
    fprintf(out, "  Keywords     : %d\n", count[TOK_TYPE_KEYWORD] + count[TOK_CONTROL_KEYWORD]);
    fprintf(out, "  Identifiers  : %d\n", count[TOK_IDENTIFIER] + count[TOK_ARRAY]);
    fprintf(out, "  Constants    : %d\n", count[TOK_INT_CONSTANT] + count[TOK_FLOAT_CONSTANT]
                                        + count[TOK_CHAR_LITERAL] + count[TOK_STRING_LITERAL]);
    fprintf(out, "  Comments     : %d\n", count[TOK_COMMENT]);
    fprintf(out, "  Invalid      : %d\n", count[TOK_INVALID]);
}
