#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint;

/* Status of functions */
enum Status
{
    e_success,
    e_failure
};

/* Operation Type */
enum OperationType
{
    e_encode,
    e_decode,
    e_unsupported
};

#endif