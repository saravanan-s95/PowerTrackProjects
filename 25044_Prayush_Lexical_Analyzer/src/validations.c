#include "validations.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static const char *const C_KEYWORDS[] = {
    "auto",     "break",    "case",     "char",     "const",    "continue",
    "default",  "do",       "double",   "else",     "enum",     "extern",
    "float",    "for",      "goto",     "if",       "inline",   "int",
    "long",     "register", "restrict", "return",   "short",    "signed",
    "sizeof",   "static",   "struct",   "switch",   "typedef",  "union",
    "unsigned", "void",     "volatile", "while"
};

#define NUM_KEYWORDS (sizeof(C_KEYWORDS) / sizeof(C_KEYWORDS[0]))

static const char *const C_OPERATORS[] = {
    "!",    "!=",   "%",    "&",    "&&",   "*",    "+",    "++",
    "-",    "--",   "->",   ".",    "/",    ":",    "<",    "<<",
    "<=",   "=",    "==",   ">",    ">=",   ">>",   "?",    "^",
    "|",    "||",   "~"
};

#define NUM_OPERATORS (sizeof(C_OPERATORS) / sizeof(C_OPERATORS[0]))

static const char *const C_SYMBOLS[] = {
    "(",    ")",    ",",    ";",    "[",    "]",    "{",    "}"
};

#define NUM_SYMBOLS (sizeof(C_SYMBOLS) / sizeof(C_SYMBOLS[0]))

static bool binary_search_str(const char *const arr[], size_t size, const char *target) {
    if (target == NULL) return false;

    int low = 0;
    int high = (int)size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(target, arr[mid]);

        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return false;
}

bool is_keyword(const char *token) {
    return binary_search_str(C_KEYWORDS, NUM_KEYWORDS, token);
}

bool is_operator(const char *op) {
    return binary_search_str(C_OPERATORS, NUM_OPERATORS, op);
}

bool is_symbol(const char *sym) {
    return binary_search_str(C_SYMBOLS, NUM_SYMBOLS, sym);
}
