#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "s2html_event.h"
static pevent_t *pstate_idle_handler(FILE *fp, int ch);
static pevent_t *pstate_single_line_comment_handler(FILE *fp, int ch);
static pevent_t *pstate_multi_line_comment_handler(FILE *fp, int ch);
static pevent_t *pstate_string_handler(FILE *fp, int ch);
static pevent_t *pstate_ascii_char_handler(FILE *fp, int ch);
static pevent_t *pstate_numeric_constant_handler(FILE *fp, int ch);
static pevent_t *pstate_preprocessor_directive_handler(FILE *fp, int ch);
static pevent_t *pstate_header_file_handler(FILE *fp, int ch);
static pevent_t *pstate_reserved_keyword_handler(FILE *fp, int ch);
/* Parser event data */
static pevent_t pevent_data;

/* Current parser state */
static pstate_e state = PSTATE_IDLE;

/* Token index */
static int event_data_idx = 0;
static int is_keyword(char *str);
/* Set parser event */
void set_parser_event(pstate_e next_state, pevent_e event)
{
    pevent_data.data[event_data_idx] = '\0';
    pevent_data.length = event_data_idx;
    pevent_data.type = event;

    event_data_idx = 0;
    state = next_state;
}

/* Get next parser event */
pevent_t *get_parser_event(FILE *fp)
{
    int ch;

    while (1)
    {
        ch = fgetc(fp);

        if (ch == EOF)
        {
            pevent_data.type = PEVENT_EOF;
            pevent_data.length = 0;
            return &pevent_data;
        }

        switch(state)
        {
            case PSTATE_IDLE:
                if (pstate_idle_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_SINGLE_LINE_COMMENT:
                if (pstate_single_line_comment_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_MULTI_LINE_COMMENT:
                if (pstate_multi_line_comment_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_STRING:
                if (pstate_string_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_ASCII_CHAR:
                if (pstate_ascii_char_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_NUMERIC_CONSTANT:
                if (pstate_numeric_constant_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_PREPROCESSOR_DIRECTIVE:
                if (pstate_preprocessor_directive_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_HEADER_FILE:
                if (pstate_header_file_handler(fp, ch))
                    return &pevent_data;
                break;

            case PSTATE_RESERVE_KEYWORD:
                if (pstate_reserved_keyword_handler(fp, ch))
                    return &pevent_data;
                break;
        }
    }
}
/*----------------------------------------------------------*/
/* IDLE STATE HANDLER                                       */
/*----------------------------------------------------------*/
static pevent_t *pstate_idle_handler(FILE *fp, int ch)
{
    int next_ch;

    switch (ch)
    {
        case '/':
            next_ch = fgetc(fp);

            if (next_ch == '/')
            {
                pevent_data.data[event_data_idx++] = '/';
                pevent_data.data[event_data_idx++] = '/';
                state = PSTATE_SINGLE_LINE_COMMENT;
                return NULL;
            }
            else if (next_ch == '*')
            {
                pevent_data.data[event_data_idx++] = '/';
                pevent_data.data[event_data_idx++] = '*';
                state = PSTATE_MULTI_LINE_COMMENT;
                return NULL;
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, fp);

                pevent_data.data[event_data_idx++] = '/';
                set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
                return &pevent_data;
            }

        case '"':
            pevent_data.data[event_data_idx++] = ch;
            state = PSTATE_STRING;
            return NULL;

        case '\'':
            pevent_data.data[event_data_idx++] = ch;
            state = PSTATE_ASCII_CHAR;
            return NULL;

        case '#':
            pevent_data.data[event_data_idx++] = ch;
            state = PSTATE_PREPROCESSOR_DIRECTIVE;
            return NULL;

        default:

            if (isdigit(ch))
            {
                pevent_data.data[event_data_idx++] = ch;
                state = PSTATE_NUMERIC_CONSTANT;
                return NULL;
            }

            if (isalpha(ch) || ch == '_')
            {
                pevent_data.data[event_data_idx++] = ch;
                state = PSTATE_RESERVE_KEYWORD;
                return NULL;
            }

            pevent_data.data[event_data_idx++] = ch;
            set_parser_event(PSTATE_IDLE, PEVENT_REGULAR_EXP);
            return &pevent_data;
    }
}
static pevent_t *pstate_single_line_comment_handler(FILE *fp, int ch)
{
    (void)fp;

    pevent_data.data[event_data_idx++] = ch;

    if (ch == '\n')
    {
        set_parser_event(PSTATE_IDLE, PEVENT_SINGLE_LINE_COMMENT);
        return &pevent_data;
    }

    return NULL;
}

static pevent_t *pstate_multi_line_comment_handler(FILE *fp, int ch)
{
    static int prev_ch = 0;

    (void)fp;

    pevent_data.data[event_data_idx++] = ch;

    if (prev_ch == '*' && ch == '/')
    {
        prev_ch = 0;
        set_parser_event(PSTATE_IDLE, PEVENT_MULTI_LINE_COMMENT);
        return &pevent_data;
    }

    prev_ch = ch;

    return NULL;
}

static pevent_t *pstate_string_handler(FILE *fp, int ch)
{
    static int escape = 0;

    (void)fp;

    pevent_data.data[event_data_idx++] = ch;

    if (ch == '\\' && !escape)
    {
        escape = 1;
        return NULL;
    }

    if (ch == '"' && !escape)
    {
        set_parser_event(PSTATE_IDLE, PEVENT_STRING);
        return &pevent_data;
    }

    escape = 0;

    return NULL;
}

static pevent_t *pstate_ascii_char_handler(FILE *fp, int ch)
{
    static int escape = 0;

    (void)fp;

    pevent_data.data[event_data_idx++] = ch;

    if (ch == '\\' && !escape)
    {
        escape = 1;
        return NULL;
    }

    if (ch == '\'' && !escape)
    {
        set_parser_event(PSTATE_IDLE, PEVENT_ASCII_CHAR);
        return &pevent_data;
    }

    escape = 0;

    return NULL;
}

static pevent_t *pstate_numeric_constant_handler(FILE *fp, int ch)
{
    if (isdigit(ch) || ch == '.' || ch == 'x' || ch == 'X' ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F'))
    {
        pevent_data.data[event_data_idx++] = ch;
        return NULL;
    }

    ungetc(ch, fp);

    set_parser_event(PSTATE_IDLE, PEVENT_NUMERIC_CONSTANT);

    return &pevent_data;
}

static pevent_t *pstate_preprocessor_directive_handler(FILE *fp, int ch)
{
    if (ch == '<')
    {
        /* '<' belongs to the next event */
        ungetc(ch, fp);

        /* Remove '\0' */
        pevent_data.data[event_data_idx] = '\0';
        pevent_data.length = event_data_idx;
        pevent_data.type = PEVENT_PREPROCESSOR;

        event_data_idx = 0;
        state = PSTATE_HEADER_FILE;

        return &pevent_data;
    }

    pevent_data.data[event_data_idx++] = ch;

    if (ch == '\n')
    {
        set_parser_event(PSTATE_IDLE, PEVENT_PREPROCESSOR);
        return &pevent_data;
    }

    return NULL;
}

static pevent_t *pstate_header_file_handler(FILE *fp, int ch)
{
    pevent_data.data[event_data_idx++] = ch;

    if (ch == '>')
    {
        set_parser_event(PSTATE_IDLE, PEVENT_HEADER_FILE);
        return &pevent_data;
    }

    return NULL;
}

static pevent_t *pstate_reserved_keyword_handler(FILE *fp, int ch)
{
    if (isalnum(ch) || ch == '_')
    {
        pevent_data.data[event_data_idx++] = ch;
        return NULL;
    }

    ungetc(ch, fp);

    pevent_data.data[event_data_idx] = '\0';
    pevent_data.length = event_data_idx;

    if (is_keyword(pevent_data.data))
        pevent_data.type = PEVENT_RESERVED_KEY1;
    else
        pevent_data.type = PEVENT_REGULAR_EXP;

    event_data_idx = 0;
    state = PSTATE_IDLE;

    return &pevent_data;
}
static int is_keyword(char *str)
{
    static const char *keywords[] =
    {
        "auto","break","case","char","const",
        "continue","default","do","double","else",
        "enum","extern","float","for","goto",
        "if","inline","int","long","register",
        "restrict","return","short","signed",
        "sizeof","static","struct","switch",
        "typedef","union","unsigned","void",
        "volatile","while"
    };

    int i;

    for(i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++)
    {
        if(strcmp(str, keywords[i]) == 0)
            return 1;
    }

    return 0;
}