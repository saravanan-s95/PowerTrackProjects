#ifndef TYPES_H
#define TYPES_H

#include<stdio.h>

typedef enum
{
    e_success,
    e_failure
}Status;

typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_error
}OperationType;

typedef enum
{
    e_title,
    e_artist,
    e_album,
    e_year,
    e_music,
    e_comment,
    e_unknown
}EditType;

#endif