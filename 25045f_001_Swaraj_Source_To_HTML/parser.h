#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "keyword.h"

#define SIZE 100

// Event types
typedef enum
{
    NORMAL,
    KEYWORD,
    PREPROCESSOR,
    NUMBER,
    STRING,
    CHARACTER,
    COMMENT,
    MULTI_COMMENT,
    END_FILE

} EventType;

// Store token and type
typedef struct
{
    EventType type;
    char word[SIZE];

} Event;

// Read next event
Event get_event(FILE *fp);

#endif