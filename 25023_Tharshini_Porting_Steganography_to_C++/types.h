/*NAME        : THARSHINI S
  DATE        : 27-05-2026
  DESCRIPTION : PORTING STEGANOGRAPHY TO C++ - Implemented image steganography in C++ to hide and retrieve secret data inside BMP images.*/
  
#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

//alias for unsigned int
using uint=unsigned int; //using->replacement for typedef

//status values
enum Status
{
    e_success,
    e_failure,
    d_success,
    d_failure
};

//operation type
enum OperationType
{
    e_encode,
    e_decode,
    e_unsupported
};

/* Check operation type */
OperationType check_operation_type(char *argv[]);

#endif
