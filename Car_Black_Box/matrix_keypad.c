#include <xc.h>
#include "matrix_keypad.h"

void init_matrix_keypad(void)
{
    TRISB = (TRISB & 0x1F) | 0x1E; 
    
    RBPU = 0;
}

unsigned char scan_key(void)
{
    ROW1 = 0;
    ROW2 = ROW3 = 1;
    
    if(COL1 == 0)
    {
        return SWITCH1;
    }
    else if(COL2 == 0)
    {
        return SWITCH4;
    }
    else if(COL3 == 0)
    {
        return SWITCH7;
    }
    else if(COL4 == 0)
    {
        return SWITCH10;
    }
    
    ROW2 = 0;
    ROW1 = ROW3 = 1;
    
    if(COL1 == 0)
    {
        return SWITCH2;
    }
    else if(COL2 == 0)
    {
        return SWITCH5;
    }
    else if(COL3 == 0)
    {
        return SWITCH8;
    }
    else if(COL4 == 0)
    {
        return SWITCH11;
    }
    
    ROW3 = 0;
    ROW1 = ROW2 = 1;
    
    if(COL1 == 0)
    {
        return SWITCH3;
    }
    else if(COL2 == 0)
    {
        return SWITCH6;
    }
    else if(COL3 == 0)
    {
        return SWITCH9;
    }
    else if(COL4 == 0)
    {
        return SWITCH12;
    }
    
    return ALL_RELEASED;
}

unsigned char read_matrix_keypad(unsigned char trigger)
{
    static unsigned char flag = 1;
    
    if(trigger == LEVEL)
    {
        return scan_key();
    }
    else if(trigger == EDGE)
    {
        unsigned char key = scan_key();
        
        if((key != ALL_RELEASED) && flag == 1)
        {
            flag = 0;
            return key;
        }
        else if(key == ALL_RELEASED)
        {
            flag = 1;
        }
        return ALL_RELEASED;
    }
}