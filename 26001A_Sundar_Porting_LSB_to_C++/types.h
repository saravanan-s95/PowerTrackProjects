#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
/* User defined types */
using uint = unsigned int;

/* Status will be used in fn. return type */
enum Status
{
    e_success,
    e_failure
}; 

enum OperationType
{
    e_encode,
    e_decode,
    e_unsupported
};

#endif
