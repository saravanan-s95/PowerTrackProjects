#ifndef COMMON_H
#define COMMON_H

#define MAGIC_STRING "#*"

enum class Status
{
    FAILURE,
    SUCCESS
};

enum class OprType
{
    UNSUPPORTED,
    ENCODE,
    DECODE
};

#endif