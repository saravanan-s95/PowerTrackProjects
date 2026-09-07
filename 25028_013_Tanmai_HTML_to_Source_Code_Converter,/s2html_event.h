#ifndef S2HTML_EVENT_H
#define S2HTML_EVENT_H

#include <stdio.h>

#define MAX_TOKEN_SIZE 512

/* Parser States */
typedef enum
{
    PSTATE_IDLE,
    PSTATE_SINGLE_LINE_COMMENT,
    PSTATE_MULTI_LINE_COMMENT,
    PSTATE_PREPROCESSOR_DIRECTIVE,
    PSTATE_SUB_PREPROCESSOR_MAIN,
    PSTATE_HEADER_FILE,
    PSTATE_STRING,
    PSTATE_ASCII_CHAR,
    PSTATE_NUMERIC_CONSTANT,
    PSTATE_RESERVE_KEYWORD
} pstate_e;

/* Parser Events */
typedef enum
{
    PEVENT_REGULAR_EXP,
    PEVENT_RESERVED_KEY1,
    PEVENT_RESERVED_KEY2,
    PEVENT_SINGLE_LINE_COMMENT,
    PEVENT_MULTI_LINE_COMMENT,
    PEVENT_PREPROCESSOR,
    PEVENT_HEADER_FILE,
    PEVENT_STRING,
    PEVENT_ASCII_CHAR,
    PEVENT_NUMERIC_CONSTANT,
    PEVENT_EOF
} pevent_e;

/* Event Structure */
typedef struct
{
    char data[MAX_TOKEN_SIZE];
    int length;
    pevent_e type;

} pevent_t;

/* Function Prototype */
pevent_t *get_parser_event(FILE *fp);
pevent_t *get_parser_event(FILE *fp);
void set_parser_event(pstate_e state, pevent_e event);
#endif