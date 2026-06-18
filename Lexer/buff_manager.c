#include "buff_manager.h"
#include <stdlib.h>

unsigned int lexeme_start;
unsigned int lexeme_end;

FILE *fptr;
char buffer[BUFF_SIZE];


// Move the lexeme_end to next position
void advance(void){
    lexeme_end++;

    // If reached end of the buffer
    // I lose the last synchronized token
    if(lexeme_end == BUFF_SIZE){
        lexeme_start = 0;
        lexeme_end = 0;

        refill_buffer();
    }
}


// Return the current char pointed by the lexeme_end
char peek(void){
    return buffer[lexeme_end];
}


// Function to open the file
void open_file(char *file_name){

    fptr = fopen(file_name, "r");

    if(fptr == NULL){
        printf("E: Unable to open file.\n");
        return;
    }

    // Initial Buffer fill
    refill_buffer();
}


// Function to refill the buffer
void refill_buffer(){
    fread(buffer, 1, BUFF_SIZE, fptr);
}