#include <stdio.h>
#include "analyzer.h"

// Display an error message with its line number
void reportError(int line, const char *message)
{
    fprintf(stderr, "Error at line %d: %s\n", line, message);
}