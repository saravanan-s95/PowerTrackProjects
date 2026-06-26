#ifndef TYPES_HPP
#define TYPES_HPP

typedef unsigned int uint;

/* Status */
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