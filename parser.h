#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

/* Parser states */
typedef enum
{
    PSTATE_IDLE,
    PSTATE_PREPROCESSOR,
    PSTATE_HEADER_FILE,
    PSTATE_KEYWORD,
    PSTATE_NUMERIC_CONSTANT,
    PSTATE_STRING,
    PSTATE_SINGLE_LINE_COMMENT,
    PSTATE_MULTI_LINE_COMMENT,
    PSTATE_ASCII_CHAR
} pstate_e;


/* Parser events */
typedef enum
{
    PEVENT_NONE,
    PEVENT_PREPROCESSOR_DIRECTIVE,
    PEVENT_MULTI_LINE_COMMENT,
    PEVENT_SINGLE_LINE_COMMENT,
    PEVENT_STRING,
    PEVENT_HEADER_FILE,
    PEVENT_KEYWORD,
    PEVENT_NUMERIC_CONSTANT,
    PEVENT_ASCII_CHAR,
    PEVENT_REGULAR_EXP
} event_type_e;


/* Event structure */
typedef struct
{
    event_type_e type;
    char data[1024];
} event_t;


/* Parser function */
event_t *parse_source(FILE *fp, int ch);

#endif