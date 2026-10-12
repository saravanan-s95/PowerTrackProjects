#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <stdbool.h>

bool is_keyword(const char *token);
bool is_operator(const char *op);
bool is_symbol(const char *sym);

#endif // VALIDATIONS_H
