/*
 * File:   matrix_keypad.c
 * Author: Swaraj
 *
 * Created on 23 June, 2026, 3:55 PM
 */


#include <xc.h>
#include "matrix_keypad.h"

void keypad_delay(void)
{
    for(unsigned int i = 0; i < 500; i++);
}

void init_matrix_keypad(void)
{
    MATRIX_KEYPAD_COLUMN_PORT_DDR |= 0x07;   // RB0-RB2 Input
    MATRIX_KEYPAD_ROW_PORT_DDR &= 0xF0;      // RD0-RD3 Output

    nRBPU = 0;

    ROW1 = HI;
    ROW2 = HI;
    ROW3 = HI;
    ROW4 = HI;
}

static unsigned char scan_keypad(void)
{
    ROW1 = LOW;
    ROW2 = HI;
    ROW3 = HI;
    ROW4 = HI;
    keypad_delay();

    if(COL1 == LOW) return 1;
    if(COL2 == LOW) return 2;
    if(COL3 == LOW) return 3;

    ROW1 = HI;
    ROW2 = LOW;
    ROW3 = HI;
    ROW4 = HI;
    keypad_delay();

    if(COL1 == LOW) return 4;
    if(COL2 == LOW) return 5;
    if(COL3 == LOW) return 6;

    ROW1 = HI;
    ROW2 = HI;
    ROW3 = LOW;
    ROW4 = HI;
    keypad_delay();

    if(COL1 == LOW) return 7;
    if(COL2 == LOW) return 8;
    if(COL3 == LOW) return 9;

    ROW1 = HI;
    ROW2 = HI;
    ROW3 = HI;
    ROW4 = LOW;
    keypad_delay();

    if(COL1 == LOW) return '*';
    if(COL2 == LOW) return 0;
    if(COL3 == LOW) return '#';

    return ALL_RELEASED;
}

unsigned char read_matrix_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    unsigned char key;

    key = scan_keypad();

    if(mode == LEVEL)
    {
        return key;
    }

    if((key != ALL_RELEASED) && once)
    {
        once = 0;
        return key;
    }
    else if(key == ALL_RELEASED)
    {
        once = 1;
    }

    return ALL_RELEASED;
}