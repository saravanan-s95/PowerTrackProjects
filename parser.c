#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

static event_t event_data;
static pstate_e state = PSTATE_IDLE;

/* List of C keywords */
static const char *keywords[] =
{
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while"
};

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(keywords[0]))

/* Check whether a word is a C keyword */
static int is_keyword(const char *word)
{
    size_t i;

    for (i = 0; i < KEYWORD_COUNT; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

/* Clear previous event */
static void clear_event(void)
{
    event_data.type = PEVENT_NONE;
    event_data.data[0] = '\0';
}

/* Add one character to event data */
static void add_char(char ch, int *index)
{
    if (*index < (int)sizeof(event_data.data) - 1)
    {
        event_data.data[*index] = ch;
        (*index)++;
        event_data.data[*index] = '\0';
    }
}

/* Handle single-line comment */
static event_t *handle_single_line_comment(FILE *fp, int first_ch)
{
    int ch;
    int index = 0;

    state = PSTATE_SINGLE_LINE_COMMENT;
    event_data.type = PEVENT_SINGLE_LINE_COMMENT;

    add_char('/', &index);
    add_char(first_ch, &index);

    while ((ch = fgetc(fp)) != EOF)
    {
        add_char((char)ch, &index);

        if (ch == '\n')
        {
            break;
        }
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Handle multi-line comment */
static event_t *handle_multi_line_comment(FILE *fp, int first_ch)
{
    int ch;
    int previous;
    int index = 0;

    state = PSTATE_MULTI_LINE_COMMENT;
    event_data.type = PEVENT_MULTI_LINE_COMMENT;

    add_char('/', &index);
    add_char(first_ch, &index);

    previous = first_ch;

    while ((ch = fgetc(fp)) != EOF)
    {
        add_char((char)ch, &index);

        if (previous == '*' && ch == '/')
        {
            break;
        }

        previous = ch;
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Handle string */
static event_t *handle_string(FILE *fp)
{
    int ch;
    int index = 0;
    int escape = 0;

    state = PSTATE_STRING;
    event_data.type = PEVENT_STRING;

    add_char('"', &index);

    while ((ch = fgetc(fp)) != EOF)
    {
        add_char((char)ch, &index);

        if (escape)
        {
            escape = 0;
        }
        else if (ch == '\\')
        {
            escape = 1;
        }
        else if (ch == '"')
        {
            break;
        }
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Handle character constant */
static event_t *handle_ascii_char(FILE *fp)
{
    int ch;
    int index = 0;
    int escape = 0;

    state = PSTATE_ASCII_CHAR;
    event_data.type = PEVENT_ASCII_CHAR;

    add_char('\'', &index);

    while ((ch = fgetc(fp)) != EOF)
    {
        add_char((char)ch, &index);

        if (escape)
        {
            escape = 0;
        }
        else if (ch == '\\')
        {
            escape = 1;
        }
        else if (ch == '\'')
        {
            break;
        }
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Handle numeric constant */
static event_t *handle_number(FILE *fp, int first_ch)
{
    int ch;
    int index = 0;

    state = PSTATE_NUMERIC_CONSTANT;
    event_data.type = PEVENT_NUMERIC_CONSTANT;

    add_char((char)first_ch, &index);

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isalnum((unsigned char)ch) || ch == '.' || ch == '_')
        {
            add_char((char)ch, &index);
        }
        else
        {
            ungetc(ch, fp);
            break;
        }
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Handle identifier or keyword */
static event_t *handle_identifier(FILE *fp, int first_ch)
{
    int ch;
    int index = 0;

    state = PSTATE_KEYWORD;

    add_char((char)first_ch, &index);

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isalnum((unsigned char)ch) || ch == '_')
        {
            add_char((char)ch, &index);
        }
        else
        {
            ungetc(ch, fp);
            break;
        }
    }

    if (is_keyword(event_data.data))
    {
        event_data.type = PEVENT_KEYWORD;
    }
    else
    {
        event_data.type = PEVENT_REGULAR_EXP;
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Handle preprocessor directive */
static event_t *handle_preprocessor(FILE *fp)
{
    int ch;
    int index = 0;

    state = PSTATE_PREPROCESSOR;
    event_data.type = PEVENT_PREPROCESSOR_DIRECTIVE;

    add_char('#', &index);

    while ((ch = fgetc(fp)) != EOF)
    {
        add_char((char)ch, &index);

        if (ch == '\n')
        {
            break;
        }
    }

    state = PSTATE_IDLE;

    return &event_data;
}

/* Main parser function */
event_t *parse_source(FILE *fp, int ch)
{
    int next;

    clear_event();

    if (ch == EOF)
    {
        return NULL;
    }

    /* Preprocessor directive */
    if (ch == '#')
    {
        return handle_preprocessor(fp);
    }

    /* String */
    if (ch == '"')
    {
        return handle_string(fp);
    }

    /* Character constant */
    if (ch == '\'')
    {
        return handle_ascii_char(fp);
    }

    /* Comment or division operator */
    if (ch == '/')
    {
        next = fgetc(fp);

        if (next == '/')
        {
            return handle_single_line_comment(fp, next);
        }

        if (next == '*')
        {
            return handle_multi_line_comment(fp, next);
        }

        if (next != EOF)
        {
            ungetc(next, fp);
        }

        event_data.type = PEVENT_REGULAR_EXP;
        event_data.data[0] = '/';
        event_data.data[1] = '\0';

        return &event_data;
    }

    /* Numeric constant */
    if (isdigit((unsigned char)ch))
    {
        return handle_number(fp, ch);
    }

    /* Identifier or keyword */
    if (isalpha((unsigned char)ch) || ch == '_')
    {
        return handle_identifier(fp, ch);
    }

    /* Normal character */
    event_data.type = PEVENT_REGULAR_EXP;
    event_data.data[0] = (char)ch;
    event_data.data[1] = '\0';

    return &event_data;
}