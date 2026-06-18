#ifndef BUFF_MANAGER
#define BUFF_MANAGER
#include <stdio.h>
#define BUFF_SIZE   512

// Move the lexeme_end to next position
void advance(void);

// Return the current char pointed by the lexeme_end
char peek(void);

// Function to open the file
void open_file(char *file_name);

// Function to refill the buffer
void refill_buffer();

#endif