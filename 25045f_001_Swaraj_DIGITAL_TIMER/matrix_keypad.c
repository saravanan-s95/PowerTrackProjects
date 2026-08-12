/*
 * File:   matrix_keypad.c
 * Author: Dell
 *
 * Created on 10 August, 2026, 9:35 PM
 */


#include "main.h"
#include "matrix_keypad.h"

void init_matrix_keypad(void)
{
    // Make RB0 to RB3 as input
    TRISB0 = 1;
    TRISB1 = 1;
    TRISB2 = 1;
    TRISB3 = 1;

    // Enable PORTB internal pull-ups
    OPTION_REGbits.nRBPU = 0;
}

unsigned char read_switches(void)
{
    // Check UP key
    if(RB0 == 0)
    {
        __delay_ms(20);

        if(RB0 == 0)
        {
            return UP;
        }
    }

    // Check DOWN key
    if(RB1 == 0)
    {
        __delay_ms(20);

        if(RB1 == 0)
        {
            return DOWN;
        }
    }

    // Check RIGHT key
    if(RB2 == 0)
    {
        __delay_ms(20);

        if(RB2 == 0)
        {
            return RIGHT;
        }
    }

    // Check LEFT key
    if(RB3 == 0)
    {
        __delay_ms(20);

        if(RB3 == 0)
        {
            return LEFT;
        }
    }

    return 0;
}